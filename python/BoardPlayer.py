import RPi.GPIO as GPIO
import time
from thirdparty import vlc
import random
from os import listdir
from os.path import isfile, join, isdir
import asyncio



verbose = True


def currentTimestamp():
	return time.time()


class Button:
	def __init__(self, channel, board):
		self.channel = channel
		self.board = board
		self.state = False
		self.durationShortPress = 0.5
		self.durationBetweenPresses = 0.001
		self.vlc_instance_button = board.vlc_instance
		self.player_button = self.vlc_instance_button.media_player_new()
		self.lastAction = currentTimestamp()
		self.lastLong = currentTimestamp()
		self.clicSounds = ["data/clic.mp3", "data/clic2.mp3"]
		if verbose: 
			print("Register button on channel " + str(self.channel))
		GPIO.setup(self.channel, GPIO.IN, pull_up_down = GPIO.PUD_UP)
		GPIO.add_event_detect(self.channel, GPIO.BOTH, callback=self.stateChanged) #, bouncetime=1

	def stateChanged(self, channel):
		if GPIO.input(self.channel):
			if self.state:
				self.state = False
				return self.rising()
		else:
			if not self.state:
				self.state = True
				return self.falling()

	def playClic(self, nb = 0):
		if self.player_button and self.player_button.is_playing():
			self.player_button.stop()
		self.media_button = self.vlc_instance_button.media_new(self.board.directory + "/" + self.clicSounds[nb])
		self.player_button.set_media(self.media_button)
		self.player_button.play()
	
	def rising(self):
		if verbose:
			print("Rising on channel"  + str(self.channel))
		timestamp2 = currentTimestamp()
		if timestamp2 - self.timestamp < self.durationShortPress and timestamp2 - self.lastAction > self.durationBetweenPresses and self.lastLong != self.timestamp:
			self.lastAction = timestamp2
			self.shortPress()
		else:
			if timestamp2 - self.timestamp > self.durationShortPress and self.lastLong < timestamp2 and self.lastLong != self.timestamp:
				self.lastAction = timestamp2
				self.lastLong = self.timestamp
				self.longPress()

	def afterShortPress(self, initialTimestamp):
		timestamp2 = currentTimestamp()
		if self.lastLong != self.timestamp:
			if self.state == True and self.timestamp == initialTimestamp and timestamp2 - self.lastAction > self.durationBetweenPresses:
				if verbose:
					print("LONG PRESS detected on channel" + str(self.channel))
				self.lastAction = timestamp2
				self.lastLong = self.timestamp
				self.longPress()

	def falling(self):
		self.timestamp = currentTimestamp()
		self.board.registerCallback(self.durationShortPress, self.afterShortPress, self.timestamp)
		if verbose:
			print("\nFalling on channel " + str(self.channel))


class ButtonPlayer(Button):
	debugSound = "data/default.mp3"
	
	def __init__(self, channel, alias, group, directories, board):
		Button.__init__(self, channel, board)
		self.vlc_instance = board.vlc_instance
		self.player = self.vlc_instance.media_player_new()
		self.group = group
		self.player.event_manager().event_attach(vlc.EventType.MediaPlayerEndReached, self.soundFinished, 1)
		self.alias = alias
		
		self.loadSoundListFromDirs(directories)
	

	def loadSoundListFromDirs(self, directories):
		# load sounds
		self.sounds = []
		for directory in directories:
			self.loadSoundListFromDir(self.board.directory + "/" + directory)
		if verbose:
			print("LOADING " + str(len(self.sounds)) + " in channel " + self.alias)
		if len(self.sounds) == 0:
			self.sounds.append(self.board.directory + "/" + self.debugSound)

	def loadSoundListFromDir(self, directory):
		if isdir(directory):
			self.sounds = self.sounds + [ ff for ff in [directory + "/" + f for f in listdir(directory)] if isfile(ff) and ff.endswith(".mp3")]
			
	def playing(self):
		return self.player and self.player.get_state() in [vlc.State.Playing, vlc.State.Opening]

	def getNewSound(self):
		if self.board.isDebugMode():
			print ("en mode debug")
			return self.debugSound
		else:
			print ("en mode normal")
			return random.choice(self.sounds)
	
	def setVolume(self, v):
		if verbose:
			print("SETVOLUME (" + str(v) + ") on channel " + str(self.alias))
		if self.playing():
			self.player.audio_set_volume(v)
		
	def stop(self):
		if self.player and self.player.is_playing():
			if verbose:
				print("STOP on channel " + str(self.alias))
			self.player.stop()
			self.board.adjustVolumes()
		pass
		
	def soundFinished(self, data, media):
		if verbose:
			print("ENDOFSOUND on channel " + str(self.alias))
		self.board.adjustVolumes()
		
	
	def playNewSound(self):
		self.board.stopBeforePlaying(self.group)
		if verbose:
			print("PLAY on channel " + str(self.alias))
		if self.player and self.player.is_playing():
			self.player.stop()
		self.media = self.vlc_instance.media_new(self.getNewSound())
		self.player.set_media(self.media)
		self.player.play()
		self.board.adjustVolumes()

	def shortPress(self):
		self.playClic(0)
		self.playNewSound()
		
	def longPress(self):
		self.playClic(1)
		self.stop()
	


class ButtonMainControl(Button):
	def __init__(self, channel, board):
		Button.__init__(self, channel, board)
		self.durationShortPress = 3

	def shortPress(self):
		self.playClic(1)
		self.board.stopAllSounds()
		
	def longPress(self):
		self.board.nextMode()

		
class Board:
	def __init__(self):
		self.buttons = []
		self.autoVolumes = False
		self.debugMode = False
		self.use_groups = False
		self.use_single_track = True
		self.directory = ""
		GPIO.setmode(GPIO.BOARD)
		self.vlc_instance = vlc.Instance()
		self.player = self.vlc_instance.media_player_new()

  
	def nextMode(self):
		self.stopAllSounds()
		if self.use_single_track:
			self.use_single_track = False
			self.use_groups = True
			print("GROUP MODE")
			self.play(self.directory + "/data/group-mode.mp3")
		else:
			if self.use_groups:
				self.use_groups = False
				self.use_single_track = False
				print("FULL MODE")
				self.play(self.directory + "/data/full-mode.mp3")
			else:
				self.use_groups = False
				self.use_single_track = True
				print("SINGLE MODE")
				self.play(self.directory + "/data/single-mode.mp3")

	def setDirectory(self, d):
		self.directory = d
		
	def addButtonMainControl(self, channel):
		self.mainControl = ButtonMainControl(channel, self)
		
	def addButton(self, channel, alias, group, directories):
		self.buttons.append(ButtonPlayer(channel, alias, group, directories, self))

	def stopBeforePlaying(self, group):
		if self.use_single_track:
			for button in self.buttons:
					button.stop()
		else:
			if self.use_groups:
				for button in self.buttons:
					if button.group == group:
						button.stop()

		
	def stopAllSounds(self):
		for button in self.buttons:
			button.stop()
	
	def nbPlayingButtons(self):
		return len([b for b in self.buttons if b.playing()])

	def volumeLevelNButtons(self, nbButtons):
		if nbButtons <= 0:
			return 100
		else:
			return int(round(100. / nbButtons))

	def registerCallback(self, delay, function, timestamp):
		self.loop.call_soon_threadsafe(self.loop.call_later, delay, function, timestamp)

	def adjustVolumes(self):
		# by default, pulseaudio automatically adjust multiple volumes
		if self.autoVolumes:
			nb = self.nbPlayingButtons()
			volume = self.volumeLevelNButtons(nb)
			for b in self.buttons:
				if b.playing():
					b.setVolume(volume)

	def isDebugMode(self):
		return self.debugMode
	
	def setDebugMode(self, status = True):
		self.debugMode = status
		
	def toggleDebugMode(self):		
		self.debugMode = not self.debugMode
		if self.debugMode:
			print("DEBUG MODE ON")
		else:
			print("DEBUG MODE OFF")

	def play(self, filename):
		media = self.vlc_instance.media_new(filename)
		self.player.set_media(media)
		self.player.play()

	def run(self):
		
		self.play(self.directory + "/data/startup.mp3")
		
		self.loop = asyncio.get_event_loop()
		self.loop.set_debug(True)
		self.loop.run_forever()
		self.loop.close()
		GPIO.cleanup()


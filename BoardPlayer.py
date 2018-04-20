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
		if verbose: 
			print("Register button on channel " + str(self.channel))
		GPIO.setup(self.channel, GPIO.IN, pull_up_down = GPIO.PUD_UP)
		GPIO.add_event_detect(self.channel, GPIO.BOTH, callback=self.stateChanged)

	def stateChanged(self, channel):
		if GPIO.input(self.channel):
			if self.state:
				self.state = False
				return self.rising()
		else:
			if not self.state:
				self.state = True
				return self.falling()


	def rising(self):
		if verbose:
			print("Rising on channel"  + str(self.channel))
		timestamp2 = currentTimestamp()
		if timestamp2 - self.timestamp < self.durationShortPress:
			self.shortPress()
		else:
			self.longPress()

	def afterShortPress(self, initialTimestamp):
		if self.state == True and self.timestamp == initialTimestamp:
			if verbose:
				print("LONG PRESS detected on channel" + str(self.channel))
			self.longPress()

	def falling(self):
		self.timestamp = currentTimestamp()
		self.board.registerCallback(self.durationShortPress, self.afterShortPress, self.timestamp)
		if verbose:
			print("\nFalling on channel " + str(self.channel))


class ButtonPlayer(Button):
	def __init__(self, channel, alias, directories, board):
		Button.__init__(self, channel, board)
		self.player = None
		self.alias = alias
		
		self.loadSoundListFromDirs(directories)
		
	def loadSoundListFromDirs(self, directories):
		# load sounds
		self.sounds = []
		for directory in directories:
			self.loadSoundListFromDir(directory)
		if verbose:
			print("LOADING " + str(len(self.sounds)) + " in channel " + self.alias)
		if len(self.sounds) == 0:
			self.sounds.append("data/default.mp3")

	def loadSoundListFromDir(self, directory):
		if isdir(directory):
			self.sounds = self.sounds + [ ff for ff in [directory + "/" + f for f in listdir(directory)] if isfile(ff) and ff.endswith(".mp3")]
			
	def playing(self):
		return self.player and self.player.get_state() in [vlc.State.Playing, vlc.State.Opening]

	def getNewSound(self):
		return random.choice(self.sounds)
	
	def setVolume(self, v):
		if verbose:
			print("SETVOLUME (" + str(v) + ") on channel " + str(self.alias))
		if self.playing():
			self.player.audio_set_volume(v)
	
	def stop(self):
		if self.player:
			if verbose:
				print("STOP on channel " + str(self.alias))
			self.player.stop()
			self.player.release()
			self.player = None
			self.board.adjustVolumes()
		pass
		
	def soundFinished(self, data, media):
		if verbose:
			print("ENDOFSOUND on channel " + str(self.alias))
		self.player = None
		self.board.adjustVolumes()
		
	
	def playNewSound(self):
		if verbose:
			print("PLAY on channel " + str(self.alias))
		if self.player:
			self.player.stop()
			self.player.release()
		self.player = vlc.MediaPlayer(self.getNewSound())
		self.player.event_manager().event_attach(vlc.EventType.MediaPlayerEndReached, self.soundFinished, 1)
		self.player.play()
		self.board.adjustVolumes()

	def shortPress(self):
		self.playNewSound()
		
	def longPress(self):
		self.stop()
	


class ButtonMainControl(Button):
	def __init__(self, channel, board):
		Button.__init__(self, channel, board)

	def shortPress(self):
		self.board.stopAllSounds()
		
	def longPress(self):
		# TODO: another action?
		self.board.stopAllSounds()

		
class Board:
	def __init__(self):
		self.buttons = []
		self.autoVolumes = False
		GPIO.setmode(GPIO.BOARD)
		
	def addButtonMainControl(self, channel):
		self.mainControl = ButtonMainControl(channel, self)
		
	def addButton(self, channel, alias, directories):
		self.buttons.append(ButtonPlayer(channel, alias, directories, self))
		
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

	def run(self):
		
		self.loop = asyncio.get_event_loop()
		self.loop.set_debug(True)
		self.loop.run_forever()
		self.loop.close()
		GPIO.cleanup()


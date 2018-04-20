import RPi.GPIO as GPIO
import time
from thirdparty import vlc
import random
from os import listdir
from os.path import isfile, join, isdir



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
		print("Rising on channel" + str(self.channel))
		timestamp2 = currentTimestamp()
		if timestamp2 - self.timestamp < self.durationShortPress:
			self.shortPress()
		else:
			self.longPress()

	def falling(self):
		self.timestamp = currentTimestamp()
		print("\nFalling on channel" + str(self.channel))


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
		if len(self.sounds) == 0:
			self.sounds.append("data/default.mp3")

	def loadSoundListFromDir(self, directory):
		if isdir(directory):
			self.sounds = self.sounds + [f for f in listdir(directory) if isfile(join(directory, f)) and f.endswith(".mp3")]
			
	def playing(self):
		return self.player and self.player.get_state() == libvlc_Playing

	def getNewSound(self):
		return random.choice(self.sounds)
			
	def stop(self):
		if verbose:
			print("STOP on button " + str(self.alias))
		if self.player:
			self.player.stop()
			self.player = None
		pass
	def playNewSound(self):
		if verbose:
			print("PLAY on button " + str(self.alias))
		self.player = vlc.MediaPlayer(self.getNewSound())
		self.player.play()

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
		GPIO.setmode(GPIO.BOARD)
		
	def addButtonMainControl(self, channel):
		self.mainControl = ButtonMainControl(channel, self)
		
	def addButton(self, channel, alias, directories):
		self.buttons.append(ButtonPlayer(channel, alias, directories, self))
		
	def stopAllSounds(self):
		for button in self.buttons:
			button.stop()
	
	def nbPlayingButtons(self):
		return len([b for b in buttons if b.playing()])

	def run(self):
		import asyncio
		
		loop = asyncio.get_event_loop()
		loop.run_forever()
		loop.close()

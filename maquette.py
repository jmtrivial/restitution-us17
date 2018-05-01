#!/usr/bin/env python3

from BoardPlayer import Board
from BoardPlayer import ButtonPlayer
from BoardPlayer import ButtonMainControl

import os

board = Board()

board.addButtonMainControl(19)

board.setDirectory(os.path.dirname(os.path.realpath(__file__)))

buttons = {}
buttons[3] = "cachot"
buttons[5] = "cour-loge"
buttons[7] = "tour"
buttons[8] = "cour-bar"
buttons[10] = "cour-canapes"
buttons[11] = "cour-animaux"
buttons[12] = "cuisine"
buttons[13] = "lac"
buttons[15] = "cour-chapelle"
buttons[16] = "chapelle"
buttons[18] = "ruine-revolution"


for channel, alias in buttons.items():
	board.addButton(channel, alias, [ "data/playlist-" + str(channel), "data/playlist-" + alias])
	
board.run()


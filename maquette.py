#!/usr/bin/env python3

from BoardPlayer import Board
from BoardPlayer import ButtonPlayer
from BoardPlayer import ButtonMainControl



board = Board()

board.addButtonMainControl(3)


buttons = {}
buttons[5] = "cachot"
buttons[7] = "tour"
buttons[8] = "chapelle"
buttons[10] = "cour-chapelle"
buttons[11] = "cour-loge"
buttons[12] = "cour-cuisine"
buttons[13] = "ruine-revolution"


for channel, alias in buttons.items():
	board.addButton(channel, alias, [ "data/playlist-" + str(channel), "data/playlist-" + alias])
	

board.run()


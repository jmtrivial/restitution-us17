#!/usr/bin/env python3

from BoardPlayer import Board
from BoardPlayer import ButtonPlayer
from BoardPlayer import ButtonMainControl



board = Board()

board.addButtonMainControl(3)

for channel in [5, 7, 8, 10, 11, 12, 13]:
	board.addButton(channel, "data/playlist-" + str(channel))
	

board.run()

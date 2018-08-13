#!/usr/bin/env python3

from BoardPlayer import Board
from BoardPlayer import ButtonPlayer
from BoardPlayer import ButtonMainControl

import os

board = Board()

board.addButtonMainControl(19)

board.setDirectory(os.path.dirname(os.path.realpath(__file__)))

buttons = {}
buttons[3] = ["cachot", "jaune"]
buttons[5] = ["cour-loge", "jaune"]
buttons[7] = ["tour", "noir"]
buttons[8] = ["cour-bar", "noir"]
buttons[10] = ["cour-canapes", "noir"]
buttons[11] = ["cour-animaux", "noir"]
buttons[12] = ["cuisine", "noir"]
buttons[13] = ["lac", "noir"]
buttons[15] = ["cour-chapelle", "jaune"]
buttons[16] = ["chapelle", "jaune"]
buttons[18] = ["ruine-revolution", "jaune"]


for channel, alias in buttons.items():
	board.addButton(channel, alias[0], alias[1], [ "data/playlist-" + str(channel), "data/playlist-" + alias[0]])
	
board.run()


#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ ! `screen -list|grep maquette|wc -l` -eq 0 ]; then
	echo "FORCE TO QUIT EXISTING SCRIPT"
	screen -S "maquette" -X quit
fi

screen -S "maquette" -d -m $DIR/maquette.py



#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ ! `screen -list|grep maquette|wc -l` -eq 0 ]; then
	echo "FORCE TO QUIT EXISTING SCRIPT"
	screen -S "maquette" -X quit
fi

echo "START AUDIO"
#echo "screen -S maquette -d -m $DIR/python/maquette.py"
echo "screen -S maquette -d -m $DIR/qt/build/maquette /home/pi/restitution-us17/"
#screen -S "maquette" -d -m $DIR/maquette.py
screen -S "maquette" -d -m $DIR/qt/build/maquette /home/pi/restitution-us17/



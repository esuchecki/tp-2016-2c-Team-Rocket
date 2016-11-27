#!/bin/bash

#revisar que esto tenga permisos de ejecucion..
fusermount -u /tmp/pokedex/
rm -rd /tmp/pokedex/

#esto de abajo es por si las dudas....
sudo umount -f /tmp/pokedex/
rm -rd /tmp/pokedex/

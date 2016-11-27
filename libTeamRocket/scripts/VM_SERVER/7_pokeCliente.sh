#!/bin/bash

#revisar que esto tenga permisos de ejecucion..
mkdir  /tmp/pokedex

# declare POKEDEX variable
POKEDEX="/tmp/pokedex"

# declare EXEC variable
EXEC="~/tp-2016-2c-Team-Rocket/Build/PROC_POKEDEX_CLIENTE"


#limpio pantalla
reset


eval $EXEC  $POKEDEX -s



#!/bin/bash

#revisar que esto tenga permisos de ejecucion..

# declare MAPA_NAME variable
ENTRENADOR_NAME="Ash"

# declare POKEDEX variable
POKEDEX="/mnt/pokedex/prueba_completa"

# declare EXEC variable
EXEC="~/home/utnso/tp-2016-2c-Team-Rocket/Build/"


#limpio pantalla
reset

#print variable on a screen
echo "abro el terminal del entrenador"
echo "nombreEntrenador:  "$ENTRENADOR_NAME
echo "ubicacion_pokedex:  "$POKEDEX
echo "***********************************"
eval $EXEC" "$ENTRENADOR_NAME" "$POKEDEX

#!/bin/bash

#***************
#instrucciones:
#revisar que esto tenga permisos de ejecucion..
#copiar en /home/utnso/
#abrir el terminal y escribir "./iniciarMapa.sh"
#listo....
#***************



# declare MAPA_NAME variable
MAPA_NAME="prueba1"

# declare POKEDEX variable
POKEDEX="~/git/tp-2016-2c-Team-Rocket/PROC_MAPA/test_files"

# declare EXEC variable
EXEC="~/git/tp-2016-2c-Team-Rocket/PROC_MAPA/Debug/mapa"


#limpio pantalla
reset

#print variable on a screen
echo "abro el terminal del mapa"
echo "nombreMapa:  "$MAPA_NAME
echo "ubicacion_pokedex:  "$POKEDEX
echo "***********************************"
eval $EXEC" "$MAPA_NAME" "$POKEDEX
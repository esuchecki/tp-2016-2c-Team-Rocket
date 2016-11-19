#!/bin/bash

#revisar que esto tenga permisos de ejecucion..
mkdir  /mnt/pokedexPrueba

# declare POKEDEX variable
POKEDEX="/mnt/pokedex"

# declare EXEC variable
EXEC="~/tp-2016-2c-Team-Rocket/Build/PROC_POKEDEX_CLIENTE"


#limpio pantalla
reset


#para consultar si ejecutar en modo debug
read -r -p "Desea ejecutar el pokedex cliente en modo ***DEBUG***? [y/N] " response
if [[ $response =~ ^([yY][eE][sS]|[yY])$ ]]
then
   	#print variable on a screen
	echo "ejecuto el pokedex cliente en modo debug"
	echo "ubicacion_pokedex:  "$POKEDEX
	echo "***********************************"
	eval $EXEC  $POKEDEX -d -s
else
   	#print variable on a screen
	echo "Elegiste ¡¡NO!!, queda abierto en segundo plano"
	echo "ubicacion_pokedex:  "$POKEDEX
	echo "***********************************"
	eval $EXEC  $POKEDEX -s
fi




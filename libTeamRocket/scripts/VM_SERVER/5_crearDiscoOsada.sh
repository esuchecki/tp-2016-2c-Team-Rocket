#!/bin/bash

# declare  variable
BUILD_PATH="~/tp-2016-2c-Team-Rocket/Build/"



echo "Ingrese el tamanio del disco"
echo ">>  [65K]....[512K]..[1M]..[2M]..[5M]..[10M]....[2G]  <<"
echo
read -r TAMANIO


echo 

cd ~/tp-2016-2c-Team-Rocket/Build/
truncate --size $TAMANIO disco.bin
cd ~/tp-2016-2c-Team-Rocket/Build/Pruebas/osada-utils-master
./osada-format ~/tp-2016-2c-Team-Rocket/Build/disco.bin

echo
echo



echo "Se creo el disco de: truncate --size $TAMANIO disco.bin"




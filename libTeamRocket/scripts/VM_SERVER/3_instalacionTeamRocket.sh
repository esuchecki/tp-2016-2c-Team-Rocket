#!/bin/bash

# declare  variable
BUILD_PATH="~/tp-2016-2c-Team-Rocket/Build/"

#comenzamos la instalacion.............
eval "mkdir -p" $BUILD_PATH

#variable de entorno
cd 
LD_LIBRARY_PATH="$LD_LIBRARY_PATH:~/tp-2016-2c-Team-Rocket/Build"
export LD_LIBRARY_PATH
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/tp-2016-2c-Team-Rocket/Build


#shared library
cd ~/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/libTeamRocket/Build/
make clean
make all
eval "cp -f ~/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/libTeamRocket/Build/liblibTeamRocket.so "  $BUILD_PATH

#entrenador
cd ~/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/PROC_ENTRENADOR/Build/
make clean
make all
eval "cp -f ~/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/PROC_ENTRENADOR/Build/entrenador " $BUILD_PATH

#mapa
cd ~/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/PROC_MAPA/Build/
make clean
make all
eval "cp -f ~/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/PROC_MAPA/Build/mapa " $BUILD_PATH

#osada
cd ~/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/OSADA_FS/Build
make clean
make all
eval "cp -f ~/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/OSADA_FS/Build/OSADA_FS" $BUILD_PATH

#poke server
cd ~/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/PROC_POKEDEX_SERVIDOR/Build
make clean
make all
eval "cp -f ~/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/PROC_POKEDEX_SERVIDOR/Build/PROC_POKEDEX_SERVIDOR " $BUILD_PATH

#poke cliente
cd ~/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/PROC_POKEDEX_CLIENTE/Build
make clean
make all
eval "cp -f ~/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/PROC_POKEDEX_CLIENTE/Build/PROC_POKEDEX_CLIENTE " $BUILD_PATH


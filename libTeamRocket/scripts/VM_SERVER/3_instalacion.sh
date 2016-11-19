#!/bin/bash

# declare  variable
BUILD_PATH="~/tp-2016-2c-Team-Rocket/Build/"

#comenzamos la instalacion.............
eval "mkdir -p" $BUILD_PATH

#variable de entorno
cd 
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/tp-2016-2c-Team-Rocket/Build


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

sudo apt-get update
sudo apt-get install openssl
sudo apt-get install libssl-dev

eval "cp-rf ~/tp-2016-2c-Team-Rocket/libTeamRocket/Pruebas " $BUILD_PATH

cd $BUILD_PATH/massive-file-creator-master
make
#!/bin/bash

# declare  variable
BUILD_PATH="~/tp-2016-2c-Team-Rocket/Build/"


#Vamos a trabajar en este directorio x default.
#mkdir ~/tp-2016-2c-Team-Rocket
cd ~/tp-2016-2c-Team-Rocket

#empezamos con las commons
git clone https://github.com/sisoputnfrba/so-commons-library.git
cd ./so-commons-library
make
sudo make install
cd ..

#instalamos la batalla pokemon
git clone https://github.com/sisoputnfrba/so-pkmn-utils
cd so-pkmn-utils
cd src
make all
sudo make install
cd ..

#instalamos osada
git clone https://github.com/sisoputnfrba/osada-utils

#dependencias de la gui
#sudo apt-get source libncurses5-dev
sudo apt-get install libncurses5-dev

#instalamos la gui
git clone https://github.com/sisoputnfrba/so-nivel-gui-library
cd so-nivel-gui-library
make && make install
cd ..

#nuestro repo
#git clone https://github.com/sisoputnfrba/tp-2016-2c-Team-Rocket



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


#scp -rpC /home/utnso/tp-2016-2c-Team-Rocket/ utnso@[192.168.0.102]:/home/utnso/tp-2016-2c-Team-Rocket


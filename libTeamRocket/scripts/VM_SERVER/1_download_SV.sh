#!/bin/bash

# declare  variable
BUILD_PATH="~/tp-2016-2c-Team-Rocket/Build/"


#Vamos a trabajar en este directorio x default.
#mkdir ~/tp-2016-2c-Team-Rocket
cd ~/tp-2016-2c-Team-Rocket

#empezamos con las commons
git clone https://github.com/sisoputnfrba/so-commons-library.git
#cd ./so-commons-library
#make
#sudo make install
#cd ..

#instalamos la batalla pokemon
git clone https://github.com/sisoputnfrba/so-pkmn-utils
#cd so-pkmn-utils
#cd src
#make all
#sudo make install
#cd ..

#instalamos osada
git clone https://github.com/sisoputnfrba/osada-utils

#dependencias de la gui
#sudo apt-get source libncurses5-dev
#sudo apt-get install libncurses5-dev

#instalamos la gui
git clone https://github.com/sisoputnfrba/so-nivel-gui-library
#cd so-nivel-gui-library
#make && make install
#cd ..

#nuestro repo
#git clone https://github.com/sisoputnfrba/tp-2016-2c-Team-Rocket




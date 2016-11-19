#!/bin/bash

# declare  variable
BUILD_PATH="~/tp-2016-2c-Team-Rocket/Build/"

#comenzamos la instalacion.............
eval "mkdir -p" $BUILD_PATH


cd ~/tp-2016-2c-Team-Rocket

#empezamos con las commons
#git clone https://github.com/sisoputnfrba/so-commons-library.git
cd ~/tp-2016-2c-Team-Rocket/so-commons-library
make
sudo make install
cd 

#instalamos la batalla pokemon
#git clone https://github.com/sisoputnfrba/so-pkmn-utils
cd ~/tp-2016-2c-Team-Rocket/so-pkmn-utils
cd src
make all
sudo make install
cd

#instalamos osada
#git clone https://github.com/sisoputnfrba/osada-utils

#dependencias de la gui
#sudo apt-get source libncurses5-dev
sudo apt-get install libncurses5-dev

#instalamos la gui
#git clone https://github.com/sisoputnfrba/so-nivel-gui-library
cd ~/tp-2016-2c-Team-Rocket/so-nivel-gui-library
make && make install
cd 

#nuestro repo
#git clone https://github.com/sisoputnfrba/tp-2016-2c-Team-Rocket


sudo apt-get update
sudo apt-get install openssl
sudo apt-get install libssl-dev

mkdir ~/tp-2016-2c-Team-Rocket/Build/Pruebas/
eval "cp -rf ~/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/Pruebas/" $BUILD_PATH

cd /home/utnso/tp-2016-2c-Team-Rocket/Build/Pruebas/massive-file-creator-master/
make
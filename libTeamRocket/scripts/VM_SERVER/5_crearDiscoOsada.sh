#!/bin/bash

# declare  variable
BUILD_PATH="~/tp-2016-2c-Team-Rocket/Build/"

cd ~/tp-2016-2c-Team-Rocket/Build/
truncate --size 10M disco.bin
cd ~/tp-2016-2c-Team-Rocket/Build/Pruebas/osada-utils-master
./osada-format ~/tp-2016-2c-Team-Rocket/Build/disco.bin
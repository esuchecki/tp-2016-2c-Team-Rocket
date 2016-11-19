#!/bin/bash

echo "Ingrese la IP de la PC"
read -r IP
scp -rpC /home/utnso/tp-2016-2c-Team-Rocket/ utnso@[$IP]:/home/utnso/tp-2016-2c-Team-Rocket/
#!/bin/bash

echo "Ingrese la IP de la PC"
read -r IP
echo "Ingrese el puerto de la PC"
read -r PUERTO
printf "IP=$IP\nPUERTO=$PUERTO" > /home/utnso/tp-2016-2c-Team-Rocket/Build/ip.txt


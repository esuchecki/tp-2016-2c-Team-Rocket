#!/bin/bash

su - -c "tee /etc/ld.so.conf.d/tR.conf <<< /home/utnso/tp-2016-2c-Team-Rocket/"
#eval "sudo tee /etc/ld.so.conf.d/tR.conf <<< /home/utnso/tp-2016-2c-Team-Rocket/"
su - -c "echo 0 >  ldconfig"
#sudo ldconfig
ldconfig -v | grep /home/utnso/tp-2016-2c-Team-Rocket

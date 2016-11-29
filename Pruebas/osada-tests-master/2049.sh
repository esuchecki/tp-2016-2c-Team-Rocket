#!/bin/bash

if [ "$#" -ne 1 ] || [ ! -d $1 ] ; then
	echo "Uso: $0 [Punto de Montaje de OSADA]"
	echo 
	exit 33
fi

OSADADIR=$1

echo "Usando $OSADADIR..."

for i in {1..2050}
do
	date +"%F %T" > $OSADADIR/$i
	
	if [ `expr $i % 24` -eq 0 ]
	then
	     # do something
		echo "Ya van: $i"
	fi

done

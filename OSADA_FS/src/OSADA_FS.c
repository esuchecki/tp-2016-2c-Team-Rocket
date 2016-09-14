/*
 ============================================================================
 Name        : OSADA_FS.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "OSADA_Constants.h"
#include "osada-utils/osada.h"


int calcularBloques(int estructura, int bytes){
	int cantidad_bloques = bytes /__tamanioBloque;
	int bloques_bitmap = cantidad_bloques/8/__tamanioBloque;
	int bloques_asignaciones = (cantidad_bloques - 1 - bloques_bitmap - 1024) * 4 /  __tamanioBloque;
	int retorno = 0;
	switch(estructura) {
	   case __estructuraHeader:
		   retorno = 1;
		   break;
	   case __estructuraBitmap:
		   retorno = bloques_bitmap;
		   break;
	   case __estructuraTablaArchivos:
		   retorno = 1024;
		   break;
	   case __estructuraTablaAsignaciones:
		   retorno = bloques_asignaciones;
		   break;
	   case __estructuraBloqueDatos:
		   retorno =  (cantidad_bloques - 1 - bloques_bitmap - 1024 - bloques_asignaciones);
		   break;
	   default :
		   retorno = 0;
	}
	return retorno;

}



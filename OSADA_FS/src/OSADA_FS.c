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
#include "osada_functions.h"
#include <commons/bitarray.h>
#include "osada-utils/osada.h"

int main(void) {
	puts("Prueba Main Osada"); /* prints PROCESO ENTRENADOR */

	//TODO: subo este codigo para que no se pierda, hay que aprolijar... Disculpen chicos!
	establecerTamanio(65*1024);
	abrirArchivo();
	osada_header* header = obtenerHeader();
	char* prueba = "prueba";
	printf("%s\n", prueba);
	printf("%s\n", header->magic_number);
	printf("%d\n", calcularBloques(__estructuraBitmap));
	printf("%d\n", header->bitmap_blocks);
	return EXIT_SUCCESS;
}

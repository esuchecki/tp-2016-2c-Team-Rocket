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
#include <commons/string.h>

int main(void) {

	abrirArchivo();
//	osada_header* header = obtenerHeader();
//	char* magicNumber = "Magic Number";
//	char* magic_number = malloc(8);
//	magic_number = string_duplicate(header->magic_number);
//	magic_number[7]='\0';
//	char* total = "Total Blocks";
//	printf("%s\n", total);
//	printf("%d\n", header->fs_blocks);
	imprimirEstructuraArchivos();
//	osada_file* tablaArchivos = obtenerTablaArchivos();
//	int j = 1;
//	int i = 0;
//	for ( i = 0 ; j>0 ; i++ ) {
//	   j = tablaArchivos[i].state;
//	   printf("%s\n", tablaArchivos[i].fname);
//	   printf("%d\n", tablaArchivos[i].parent_directory);
//	}
//	printf("%s\n", tablaArchivos[1].fname);
//	printf("%d\n", tablaArchivos[1].first_block);
//	printf("%d\n", tablaArchivos[1].parent_directory);
//	printf("%d\n", tablaArchivos[1].state);
//	printf("%s\n", tablaArchivos[2].fname);
//	printf("%d\n", tablaArchivos[2].first_block);
//	printf("%d\n", tablaArchivos[2].parent_directory);
//	printf("%d\n", tablaArchivos[2].state);
	int* tablaAsignaciones = obtenerTablaAsignaciones();
//	printf("%d\n", tablaAsignaciones[0]);
//	char* bitmapBlocks = "Total Bitmap";
//	printf("%s\n", bitmapBlocks);
//	printf("%d\n", header->bitmap_blocks);
//	t_bitarray* bitmap = obtenerBitmap();
//	char* bitmapBlocks2 = "Bitmap";
//	printf("%s\n", bitmapBlocks2);
//	printf("%d\n", bitarray_test_bit(bitmap,0));
//	printf("%d\n", bitarray_test_bit(bitmap,1));
//	printf("%d\n", bitarray_test_bit(bitmap,2));
	return EXIT_SUCCESS;
}

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
//	imprimirEstructuraArchivos();
	osada_block* archivo = obtenerArchivoPorPath("README.txt");
//	int indice = buscarArchivoPorPath("Pokemons/001.txt");
//	osada_file* tablaArchivos = obtenerTablaArchivos();
//	printf("%d\n",tablaArchivos[indice].state);
//	printf("%d\n",tablaArchivos[indice].first_block);
//	printf("%d\n",tablaArchivos[indice].file_size);
//	osada_block* bloquesDeDatos = obtenerBloqueDatos();
	printf("%s\n",archivo);
	return EXIT_SUCCESS;
}

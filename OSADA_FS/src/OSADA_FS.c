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
//	osada_block* archivo = obtenerArchivoPorPath("/README.txt");
//	char** directorios = leerDirectorio("/Vermilion City/Pokemons/");
//	int indice = buscarArchivoPorPath("Pokemons/001.txt");
//	osada_file* tablaArchivos = obtenerTablaArchivos();
//	printf("%d\n",tablaArchivos[indice].state);
//	printf("%d\n",tablaArchivos[indice].first_block);
//	printf("%d\n",tablaArchivos[indice].file_size);
//	osada_block* bloquesDeDatos = obtenerBloqueDatos();
//	printf("%s\n",archivo);
//	int i = 0;
//	for(i = 0;i<14;i++){
//		printf("%s\n",directorios[i]);
//	}
//	char* separador = "------------------------------------------------------------";
//	printf("%s\n",separador);
//	directorios = leerDirectorio("/");
//	for(i = 0;i<200;i++){
//		printf("%s\n",directorios[i]);
//	}
//	long* atributos = obtenerAtributos("/");
//	printf("%d\n",atributos[0]);
//	atributos = obtenerAtributos("/Pokemons");
//	printf("%d\n",atributos[0]);
//	printf("%d\n",atributos[1]);
//	atributos = obtenerAtributos("/Pokemons/001.txt");
//	printf("%d\n",atributos[0]);
//	printf("%d\n",atributos[1]);
//	osada_block* archivo = obtenerArchivoPorPath("Pokemons/007.txt");
//	printf("%s\n",archivo);
//	int primeroLibre = obtenerEspacioLibreTablaArchivos();
	char* padre =  obtenerPathPadre("/Vermilion City/Pokemons");
	printf("%s\n",padre);
	return EXIT_SUCCESS;
}

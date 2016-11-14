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
#include "osada-utils/osada.h"
#include <so/libSockets.h>


int main(void) {

	abrirArchivo();
	osada_header* header = obtenerHeader();
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
	//long* atributos = obtenerAtributos("/");
	//printf("%d\n",atributos[0]);
//	long * atributos = obtenerAtributos("/Vermilion City/Pokemons");
//	printf("%d\n",atributos[0]);
//	printf("%d\n",atributos[1]);
//	atributos = obtenerAtributos("/Pokemons/001.txt");
//	printf("%d\n",atributos[0]);
//	printf("%d\n",atributos[1]);
//	osada_block* archivo = obtenerArchivoPorPath("Pokemons/007.txt");
//	printf("%s\n",archivo);
//	int primeroLibre = obtenerEspacioLibreTablaArchivos();

//	char* padre =  obtenerPathPadre("/Vermilion City/Pokemons");
//	printf("%s\n",padre);

	//ejemplo de prueba.
//	int a= crearDirectorio("/Vermilion City/Pokemons/EduGroso");
//	printf("Resultado crearDirectorio: %s\n", (a==0 ? "Se creo!!" : "No se creo :("));

//	int i=0;
//	char** directorios = leerDirectorio("/");
//	while (directorios[i] != NULL) {
//		printf("%s\n",directorios[i]);
//		i++;
//	}

//	int b= cambiarNombre("/Vermilion City/Pokemons/EduGroso", "NuevoNombre");
	//int resultado = borrarDirectorio("/Vermilion City/Pokemons/EduGroso");
//	imprimirEstructuraArchivos();
//	char* path = "/Vermilion City/Pokemons/EduGroso.txt";
//	char* pathAlReves = string_reverse(path);
//	char** array = string_split(pathAlReves, "/");
//	int largo = string_length(array[0]);
//	char* pathSinArchivoAlReves = string_substring_from(pathAlReves,largo);
//	char* pathSinArchivo = string_reverse(pathSinArchivoAlReves);
//	printf("%s\n", pathSinArchivo);
//	int* tablaAsignaciones = obtenerTablaAsignaciones();
//	int i = 0;
//	int j = 0;
//	while(i<header->data_blocks){
//		printf("%d\n", tablaAsignaciones[i]);
//		i++;
//	}
//	imprimirEstructuraArchivos();
//	int resultado = borrarArchivo("/Vermilion City/Pokemons/114.txt");
//	printf("%d\n", resultado);
//	int bloquesLibres = obtenerCantidadBloquesLibres();
//	printf("%d\n", bloquesLibres);
//	osada_file* tablaArchivos = obtenerTablaArchivos();
//	int primerBloque =tablaArchivos[170].first_block;
//	liberarBloquesBitmap(primerBloque);
//	bloquesLibres = obtenerCantidadBloquesLibres();
//	printf("%d\n", bloquesLibres);
//	char* archivo = obtenerArchivoPorPath("/Pokemons/001.txt", 5,5);
//	int indice = buscarArchivoPorPath("/Pokemons/001.txt", false);
//	osada_file* archivos = obtenerTablaArchivos();
//	char* arrayDatos = (char*)archivo[0];
//	printf("%s\n", arrayDatos);
//	char* texto = "escribo los primeros 29 bytes";
//	int resultado = escribir("/Pokemons/002.txt", texto, 29,300);
//	archivo =  obtenerArchivo(bloquesArchivo, cantidadDeBloques, archivos[indice].file_size);
//	printf("%s\n", archivo);
//	printf("%d\n", resultado);
//	int tamanio = archivos[indice].file_size;
//	printf("Tamanio inicial: %d\n", tamanio);
//	int resultado = truncar("/Pokemons/004.txt", 0);
//	int cantidadDeBloques = calcularCantidadBloques(tamanio);
//	int* bloquesArchivo = obtenerBloquesArchivo(archivos[indice].first_block, cantidadDeBloques);
//	osada_block* archivo =  obtenerArchivo(bloquesArchivo, cantidadDeBloques, archivos[indice].file_size);
//	printf("%s\n", archivo);
//	int cantidadDeBloques = calcularCantidadBloques(tamanio);
//	int* bloquesArchivo = obtenerBloquesArchivo((int)archivos[indice].first_block, cantidadDeBloques);
//	printf("%d\n", bloquesArchivo);
//	archivos = obtenerTablaArchivos();
//	tamanio = archivos[indice].file_size;
//	printf("Tamanio 1ra prueba: %d\n", tamanio);
//	printf("%d\n", resultado);
//	resultado = truncar("/Pokemons/004.txt", tamanio + 1000);
//	cantidadDeBloques = calcularCantidadBloques(tamanio);
//	obtenerBloquesArchivo((int)archivos[indice].first_block, cantidadDeBloques);
//	printf("%d\n", bloquesArchivo);
//	archivos = obtenerTablaArchivos();
//	tamanio = archivos[indice].file_size;
//	printf("Tamanio 2da prueba: %d\n", tamanio);
//	printf("%d\n", resultado);
//	cantidadDeBloques = calcularCantidadBloques(tamanio);
//	obtenerBloquesArchivo((int)archivos[indice].first_block, cantidadDeBloques);
//	printf("%d\n", bloquesArchivo);
	int indice = buscarArchivoPorPath("/Pokemons/008.txt", false);
	int resultado = truncar("/Pokemons/008.txt", 0);
	osada_file* archivos = obtenerTablaArchivos();
	printf("Tamanio Archivo: %d\n", archivos[indice].file_size);
	printf("Primer Bloque: %d\n", archivos[indice].first_block);
	resultado = truncar("/Pokemons/008.txt", 1500);
	printf("Tamanio Archivo: %d\n", archivos[indice].file_size);
	printf("Primer Bloque: %d\n", archivos[indice].first_block);
	int cantidad = calcularCantidadBloques(archivos[indice].file_size);
	int* bloques = obtenerBloquesArchivo(archivos[indice].first_block, cantidad);
	int i = 0;
	//Imprimo todos los bloques que componen el archivo
	while(i < cantidad){
		printf("%d\n", bloques[i]);
		i++;
	}
	// Escribo desde el bloque 63, 20 bytes
	resultado = escribir("/Pokemons/008.txt", "mira Emi, funciono!!", 20,63);
	osada_block* archivo =  obtenerArchivo(bloques, cantidad, archivos[indice].file_size);
	int j = 0;
	char* array = (char*)archivo[0];
	while(j < 1500){
		printf("%c", array[j]);
		j++;
	}
	return EXIT_SUCCESS;
}

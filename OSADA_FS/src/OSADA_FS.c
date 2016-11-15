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

//int	resultado = truncar("/prueba4.txt", 0);

	int indice = buscarArchivoPorPath("/special.mp4", false);
	//int resultado = truncar("/Pokemons/005.txt", 0);
	osada_file* archivos = obtenerTablaArchivos();
	printf("Tamanio Archivo: %d\n", archivos[indice].file_size);
	printf("Primer Bloque: %d\n", archivos[indice].first_block);
	//resultado = truncar("/Pokemons/005.txt", 1600);
	printf("Tamanio Archivo: %d\n", archivos[indice].file_size);
	printf("Primer Bloque: %d\n", archivos[indice].first_block);
	int cantidad = calcularCantidadBloques(archivos[indice].file_size);
	int* tablaAsignaciones = obtenerTablaAsignaciones();
	int revisar =1;
	int i = archivos[indice].first_block;
	while (tablaAsignaciones[i]!= -1)
	{
		i = tablaAsignaciones[i];
		revisar++;
	}
	int* bloques = obtenerBloquesArchivo(archivos[indice].first_block, cantidad);
	i = 0;
	//Imprimo todos los bloques que componen el archivo
	while(i < cantidad){
		printf("%d\n", bloques[i]);
		i++;
	}
	// Escribo desde el byte 63, 20 bytes
	char * textoEjemplo = "Hay algo peor que la ejecución de proyectos inadecuados: la ausencia de ellos. En su afán de diferenciarla del miedo, el filósofo alemán Kierkegaard, definía a la angustia, inicialmente, como un temor sin objeto, como un visceral miedo a la nada. De ahí la naturaleza angustiante del hombre. En realidad me pareció oportuno retomar esta ya clásica noción de angustia, porque considero que el panorama científico a nivel nacional es, esencialmente, angustiante. Y lo es porque afrontamos la alborada de un nuevo milenio sin un proyecto rector -serio e idóneo- en materia de ciencia y técnica a nivel nacional. Aunque resulta paradójico o evidencie un acto de necedad supina, lo cierto del caso es que en medio de una crisis en la que se declaman o ponderan proyectos de crecimiento y desarrollo, en nuestra Argentina de fin de siglo la ciencia se cobija a la intemperie. No goza de estímulo real en orden a su promoción y consolidación. No figura en la agenda de prioridades, como de hecho ocurre en los paises que cotidianamente se consignan como modelos. En este contexto crucial, signado por la impronta de espacios vacíos, la universidad se referencia como un foro en donde la conquista del conocimiento es consecuencia de su convicción institucional. Así entonces, el desarrollo de la actividad científica en el sistema universitario debería ser visto como un enorme esfuerzo en respuesta a su rol fundacional, y a la vez, como una estrategia de proyección y servicio que potencia su validez y utilidad sin incentivos, sola, sin respaldo ni eje convocante.";
	//int resultado = escribir("/README.txt", textoEjemplo, 540,0);
	uint32_t aux;
	osada_block* archivo = obtenerArchivoPorPath("/special.mp4",131072,131072, &aux );
	//osada_block* archivo =  obtenerArchivo(bloques, cantidad, archivos[indice].file_size);
	int j = 0;
	char* array = (char*)archivo[0];
	while(j < 540){
		printf("%c", array[0 + j]);
		j++;
	}
	printf("\n");
	// Escribo desde el byte 1000, 35 bytes
//	resultado = escribir("/Pokemons/008.txt", "otra prueba para verificar que anda", 35,1000);
//	osada_block* archivo =  obtenerArchivo(bloques, cantidad, archivos[indice].file_size);
//	j = 0;
//	char* otroarray = (char*)archivo[0];
//	// Leo del byte 1000 en adelante 35 bytes
//	while(j < 35){
//		printf("%c", otroarray[1000 + j]);
//		j++;
//	}
	return EXIT_SUCCESS;
}

/*
 * osada_functions.c
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */
#include "osada-utils/osada.h"
#include "OSADA_Constants.h"
#include <stdio.h>
#include <sys/mman.h>
#include <fuse.h>
#include <commons/bitarray.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int TAMANIO_BYTES;
struct stat osadaStat;
osada_block* bloques_archivo;
osada_header* header;

//void establecerTamanio(int tamanio) { //Bytes
//	TAMANIO_BYTES = tamanio;
//}

int totalBloques() {
	return header->fs_blocks;
}

int calcularBloques(int estructura) {
	int cantidad_bloques = TAMANIO_BYTES / __tamanioBloque;
	int bloques_bitmap = cantidad_bloques / 8 / __tamanioBloque;
	int bloques_asignaciones = (cantidad_bloques - 1 - bloques_bitmap - 1024)
			* 4 / __tamanioBloque;
	int retorno = 0;
	switch (estructura) {
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
		retorno = (cantidad_bloques - 1 - bloques_bitmap - 1024
				- bloques_asignaciones);
		break;
	default:
		retorno = 0;
	}
	return retorno;
}

int obtenerBytesBloques(int cantidadBloques) {
	return (cantidadBloques * __tamanioBloque);
}

//Retorna la cantidad de bloques que son usadas por las estructuras base
int calcularBloquesEstructurasBase(){
	int header = calcularBloques(__estructuraHeader);
	int bitmap = calcularBloques(__estructuraBitmap);
	int tArchivos = calcularBloques(__estructuraTablaArchivos);
	int tAsignaciones = calcularBloques(__estructuraTablaAsignaciones);
	int total = header + bitmap + tArchivos + tAsignaciones;
	return total;
}

void abrirArchivo(){
	int fd_osada;
	fd_osada= open(__pathArchivo,O_RDWR);
	fstat(fd_osada,&osadaStat);
	bloques_archivo= mmap(0, osadaStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_osada, 0);
}

osada_header* obtenerHeader(){
	header = (osada_header*)bloques_archivo;
	return header;
}

t_bitarray* obtenerBitmap(){
	osada_header* header = obtenerHeader();
	t_bitarray* bitmap = bitarray_create(bloques_archivo[1],header->bitmap_blocks);
	return bitmap;
}

osada_file* obtenerTablaArchivos(){
	obtenerHeader();
	osada_file* tablaArchivos = (osada_file*)bloques_archivo[1 + header->bitmap_blocks];
	return tablaArchivos;
}

int* obtenerTablaAsignaciones(){
	int* tablaAsignaciones = bloques_archivo[1 + header->bitmap_blocks + 1024];
	return tablaAsignaciones;
}

osada_block* obtenerBloqueDatos(){
	osada_block* bloqueDatos = bloques_archivo[1 + header->bitmap_blocks + header->fs_blocks + header->allocations_table_offset];
	return bloqueDatos;
}

void imprimirEstructuraArchivos(){
	osada_file* tablaArchivos = obtenerTablaArchivos();
	int j = 1;
	int i,k;
	char* guion = "-";
	for ( i = 0 ; j>0 ; i++ ) {
	   j = tablaArchivos[i].state;
	   k = tablaArchivos[i].parent_directory;
	   while(k<2048){
		   printf("%s", guion);
		   k = tablaArchivos[k].parent_directory;
	   }
	   printf("%s\n", tablaArchivos[i].fname);
	}
}




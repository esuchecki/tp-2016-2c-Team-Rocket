/*
 * osada_functions.c
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */
#include "osada.h"
#include "OSADA_Constants.h"
#include <stdio.h>
#include <sys/mman.h>
#include <fuse.h>
#include <commons/bitarray.h>

int TAMANIO_BYTES;
struct stat osadaStat;
osada_block* primer_bloque;
osada_header* header;

void establecerTamanio(int tamanio) { //Bytes
	TAMANIO_BYTES = tamanio;
}

int totalBloques() {
	return TAMANIO_BYTES / __tamanioBloque;
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
	primer_bloque= mmap(0, osadaStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_osada, 0);
}

osada_header* obtenerHeader(){
	header = (osada_header*)primer_bloque;
	return header;
}

t_bitarray* obtenerBitmap(){
	t_bitarray* bitmap = (t_bitarray*)primer_bloque[1];
	return bitmap;
}

osada_file* obtenerTablaArchivos(){
	osada_file* tablaArchivos = (osada_file*)primer_bloque[1 + header->bitmap_blocks];
	return tablaArchivos;
}

osada_block* obtenerTablaAsignaciones(){
	osada_block* tablaAsignaciones = primer_bloque[1 + header->bitmap_blocks + header->fs_blocks];
	return tablaAsignaciones;
}

osada_block* obtenerBloqueDatos(){
	osada_block* bloqueDatos = primer_bloque[1 + header->bitmap_blocks + header->fs_blocks + header->allocations_table_offset];
	return bloqueDatos;
}




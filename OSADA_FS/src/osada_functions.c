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
#include <commons/string.h>

#define ROOT_INDEX     65535

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

int calcularCantidadBloques(int tamanioEnBytes){
	int esDivisor = tamanioEnBytes%OSADA_BLOCK_SIZE;
	int resultado = tamanioEnBytes / OSADA_BLOCK_SIZE;
	if(esDivisor > 0){
		resultado = resultado + 1;
	}
	return resultado;
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
	inicializarHeader();
	osada_file* tablaArchivos = (osada_file*)bloques_archivo[1 + header->bitmap_blocks];
	return tablaArchivos;
}

int* obtenerTablaAsignaciones(){
	inicializarHeader();
	int* tablaAsignaciones = bloques_archivo[header->allocations_table_offset];
	return tablaAsignaciones;
}

osada_block* obtenerBloqueDatos(){
	inicializarHeader();
	int bloquesTablaAsignaciones = ((header->fs_blocks - header->allocations_table_offset) * 4 / OSADA_BLOCK_SIZE)-1;
	osada_block* bloqueDatos = bloques_archivo[header->allocations_table_offset + bloquesTablaAsignaciones];
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
	   if(j>0){
		   while(k!=ROOT_INDEX){
			   printf("%s", guion);
			   k = tablaArchivos[k].parent_directory;
		   }
		   printf("%s\n", tablaArchivos[i].fname);
	   }
	}
}

void inicializarHeader(){
	if(header == NULL){
		obtenerHeader();
	}
}

int buscarArchivoPorPath(char* path){ //retorna el indice del archivo en la tabla de archivos
	char** array = string_split(path, "/");
	int length =  string_length(array);
	int i = 0;
	int retorno;
	int padre = ROOT_INDEX;
	while(array[i]!= NULL){
		retorno = buscarArchivoEnFS(array[i], padre);
		padre = retorno;
		i++;
	}
	return retorno;
}

char* obtenerString(char* string){
	int i;
	char* resultado = malloc(OSADA_FILENAME_LENGTH);
	for ( i = 0 ; i<OSADA_FILENAME_LENGTH ; i++ ) {
		if(string[i] != '\0'){
			resultado[i] = string[i];
		} else {
			break;
		}
	}
	return resultado;
}

/*
 * Retorna -1 si el archivo no existe o
 * el indice de su posicion en la tabla
 * de archivos
 */
int buscarArchivoEnFS(char* nombre, int padre){
	int retorno = -1;
	osada_file* tablaArchivos = obtenerTablaArchivos();
	int i,j = 1;
	for ( i = 0 ; (j>0 && retorno == -1); i++ ) {
	   j = tablaArchivos[i].state;
	   if(j>0){
		   char* directorio = obtenerString(tablaArchivos[i].fname);
		   if(strcmp(directorio,nombre)==0 && tablaArchivos[i].parent_directory == padre){
			   retorno = i;
		   }
	   }
	}
	return retorno;
}


/*
 * Retorna un puntero a un array con los
 * bloques que componen al archivo
 */
int* obtenerBloquesArchivo(int numeroBloqueInicial, int cantidadDeBloques){
	int* bloques = malloc(sizeof(int) * cantidadDeBloques);
	int* tablaAsignaciones = obtenerTablaAsignaciones();
	int i = numeroBloqueInicial;
	int j= 0;
	bloques[j] = i;
	while(tablaAsignaciones[i]!= -1){
		j = j + 1;
		bloques[j] = tablaAsignaciones[i];
		i = tablaAsignaciones[i];
	}
	return bloques;
}

osada_block* obtenerArchivo(int* bloquesQueLoConforman, int cantidadDeBloques, int tamanioArchivo){
	osada_block* archivoConOffset[cantidadDeBloques];
	osada_block* bloquesDeDatos = obtenerBloqueDatos();
	int k = 0;
	int i;
	while( k < cantidadDeBloques ){
		i = bloquesQueLoConforman[k];
		archivoConOffset[k] = bloquesDeDatos[i];
		k = k + 1;
	}
	printf("%s\n",archivoConOffset[10]);
	return archivoConOffset;
}


osada_block* obtenerArchivoPorPath(char* path){
	osada_file* tablaArchivos = obtenerTablaArchivos();
	int index = buscarArchivoPorPath(path);
	int cantidadDeBloques = calcularCantidadBloques(tablaArchivos[index].file_size);
	int* bloquesArchivo = obtenerBloquesArchivo((int)tablaArchivos[index].first_block, cantidadDeBloques);
	return obtenerArchivo(bloquesArchivo,cantidadDeBloques,tablaArchivos[index].file_size);
}



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
#include "osada_functions.h"

#define ROOT_INDEX     65535

int TAMANIO_BYTES;
struct stat osadaStat;
osada_block* bloques_archivo;
osada_header* header;

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

void inicializarHeader(){
	if(header == NULL){
		obtenerHeader();
	}
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
	t_bitarray* bitmap = bitarray_create( (char *) bloques_archivo[1],header->bitmap_blocks);
	return bitmap;
}

osada_file* obtenerTablaArchivos(){
	inicializarHeader();
	osada_file* tablaArchivos = (osada_file*)bloques_archivo[1 + header->bitmap_blocks];
	return tablaArchivos;
}

int* obtenerTablaAsignaciones(){
	inicializarHeader();
	int* tablaAsignaciones = (int*) &bloques_archivo[header->allocations_table_offset];
	return tablaAsignaciones;
}

osada_block* obtenerBloqueDatos(){
	inicializarHeader();
	int bloquesTablaAsignaciones = ((header->fs_blocks - header->allocations_table_offset) * 4 / OSADA_BLOCK_SIZE)+1;
	osada_block * bloqueDatos = &bloques_archivo[header->allocations_table_offset + bloquesTablaAsignaciones];
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
		   if((strcmp((char*)tablaArchivos[i].fname, (char*)nombre)==0) && (tablaArchivos[i].parent_directory == padre)){
			   retorno = i;
		   }
	   }
	}
	return retorno;
}

int buscarArchivoPorPath(char* path){ //retorna el indice del archivo en la tabla de archivos
	int resultado;
	if(strcmp("/",path)!=0){
		char** array = string_split(path, "/");
		int i = 0;
		int padre = ROOT_INDEX;
		while(array[i]!= NULL){
			resultado = buscarArchivoEnFS(array[i], padre);
			padre = resultado;
			i++;
		}
	} else {
		resultado = -1;
	}
	return resultado;
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
		j++;
		bloques[j] = tablaAsignaciones[i];
		i = tablaAsignaciones[i];
	}
	return bloques;
}

int calcularBytesExedentes(int cantidadDeBloques, int tamanioArchivo){
	int bytesEnBloques = cantidadDeBloques * OSADA_BLOCK_SIZE;
	int resultado;
	if(bytesEnBloques == tamanioArchivo){
		resultado = 0;
	} else {
		int unBloqueDeMenos = (cantidadDeBloques -1) * OSADA_BLOCK_SIZE;
		int exedente = tamanioArchivo - unBloqueDeMenos;
		resultado = exedente;
	}
	return resultado;
}

osada_block* obtenerArchivo(int* bloquesQueLoConforman, int cantidadDeBloques, int tamanioArchivo){
	osada_block* archivoConOffset = malloc(cantidadDeBloques * OSADA_BLOCK_SIZE * sizeof(unsigned char));
	osada_block* bloquesDeDatos = obtenerBloqueDatos();
	int k = 0;
	int i;
	int cantidadACopiar = OSADA_BLOCK_SIZE;
	int exedente = calcularBytesExedentes(cantidadDeBloques,tamanioArchivo);
	while( k < cantidadDeBloques ){
		i = bloquesQueLoConforman[k];
		if (k == (cantidadDeBloques -1) && exedente>0){
			int cantidadACopiar = exedente;
		}
		memcpy(archivoConOffset[k], bloquesDeDatos[i], cantidadACopiar * sizeof(unsigned char));
		//archivoConOffset[k] = bloquesDeDatos[i];
		k++;
	}
	//printf("%s\n",archivoConOffset[0]);
	//como vamos a printearlo por pantalla le tiro un realloc
	return archivoConOffset;
}


osada_block* obtenerArchivoPorPath(char* path){
	osada_file* tablaArchivos = obtenerTablaArchivos();
	int index = buscarArchivoPorPath(path);
	osada_block* resultado;
	if(index>0){
		int cantidadDeBloques = calcularCantidadBloques(tablaArchivos[index].file_size);
		int* bloquesArchivo = obtenerBloquesArchivo((int)tablaArchivos[index].first_block, cantidadDeBloques);
		resultado = obtenerArchivo(bloquesArchivo,cantidadDeBloques,tablaArchivos[index].file_size);
	}
	return resultado;
}

char** leerDirectorio(char* path){
	int subdirectoriosMax[2048]; //Cantidad máxima de subdirectorios
	int padre;
	if(strcmp("/",path)==0){
		padre = ROOT_INDEX;
	} else {
		padre = buscarArchivoPorPath(path);
	}
	osada_file* tablaArchivos = obtenerTablaArchivos();
	int j = 1;
	int i,k;
	k = 0;
	for ( i = 0 ; j>0 ; i++ ) {
	   j = tablaArchivos[i].state;
	   if(tablaArchivos[i].parent_directory==padre){
		   subdirectoriosMax[k]=i;
		   k++;
	   }
	}
	char** subdirectoriosNombres;
	subdirectoriosNombres = malloc(sizeof(char)*k*OSADA_FILENAME_LENGTH);
	for(i=0;i<k;i++){
		subdirectoriosNombres[i] = tablaArchivos[subdirectoriosMax[i]].fname;
	}
	return subdirectoriosNombres;
}

long* obtenerAtributos(char* path){
	long* atributos = malloc(sizeof(long) * 2);
	int indiceDirectorio = buscarArchivoPorPath(path);
	if(indiceDirectorio>=0){
		osada_file* tablaArchivos = obtenerTablaArchivos();
		atributos[0] = tablaArchivos[indiceDirectorio].state;
		atributos[1] = tablaArchivos[indiceDirectorio].file_size;
	} else {
		atributos[0]=-1;
	}

	return atributos;
}



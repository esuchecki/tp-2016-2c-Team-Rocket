/*
 * osada_functions.h
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */



#ifndef OSADA_FUNCTIONS_H_
#define OSADA_FUNCTIONS_H_


#include "osada-utils/osada.h"
#include <commons/bitarray.h>
#include <string.h>
#include <stdlib.h>



void establecerTamanio(int tamanio);

int totalBloques();

int calcularBloques(int estructura);

int obtenerBytesBloques(int cantidadBloques);

int calcularBloquesEstructurasBase();

int primerBloqueDatos();

void abrirArchivo();

osada_header* obtenerHeader();

t_bitarray* obtenerBitmap();

osada_file* obtenerTablaArchivos();

int* obtenerTablaAsignaciones();

osada_block* obtenerBloqueDatos();

void imprimirEstructuraArchivos();

int buscarArchivoPorPath(char* path, bool quieroElAnteUltimo);

int* obtenerBloquesArchivo(int numeroBloqueInicial, int cantidadDeBloques);

int calcularCantidadBloques(long tamanioEnBytes);

osada_block* obtenerArchivo(int* bloquesQueLoConforman, int cantidadDeBloques, int tamanioArchivo);

osada_block* obtenerArchivoPorPath(char* path, size_t bytes, off_t offset, uint32_t * tamanioCopiarSockets);

char** leerDirectorio(char* path);

long* obtenerAtributos(char* path);

int obtenerEspacioLibreTablaArchivos();

char* obtenerPathPadre(char* path);

int crearDirectorio(char* path);

int borrarDirectorio(char* path);

int cambiarNombre(char* path, char* nombreNuevo);

int crearArchivo(char* path, long bytes);

int borrarArchivo(char* path);

int obtenerCantidadBloquesLibres();

void liberarBloquesBitmap(int primerBloque);

#endif /* OSADA_FUNCTIONS_H_ */

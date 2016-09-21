/*
 * osada_functions.h
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */
#include "osada.h"
#include <commons/bitarray.h>

#ifndef OSADA_FUNCTIONS_H_
#define OSADA_FUNCTIONS_H_

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

osada_block* obtenerTablaAsignaciones();

osada_block* obtenerBloqueDatos();

#endif /* OSADA_FUNCTIONS_H_ */

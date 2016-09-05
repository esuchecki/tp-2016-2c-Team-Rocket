/*
 * OSADA_Constants.h
 *
 *  Created on: 3/9/2016
 *      Author: utnso
 */

#ifndef OSADA_CONSTANTS_H_
#define OSADA_CONSTANTS_H_

#include <stdint.h>
#define __maxTamanioDisco (4294967296) // 4GB = 1024 * 1024 * 1024 * 4 Bytes
#define __maxTamanioArchivo (4294967296)
#define __maxNombreArchivo (17) //Caracteres
#define __maxCantidadArchivos (2048)
#define __tamanioBloque (64) //Bytes
#define __tamanioTablaArchivos (1024) //Bloques
#define __tamanioPtroBloque (4) //Bytes

typedef struct {
	char estado;
	char  nombreArchivo[17];
	uint16_t bloquePadre;
	uint32_t tamanioArchivo;
	uint32_t fechaUltimaModificacion;
	uint32_t bloqueInicial;
} osadaFile;

typedef struct {
	char identificador[7];
	char version;
	uint32_t tamanioFS;
	uint32_t tamanioBM;
	uint32_t inicioTabla;
	uint32_t tamanioDatos;
	char relleno[40];
} t_header;


#endif /* OSADA_CONSTANTS_H_ */

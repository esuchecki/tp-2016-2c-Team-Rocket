/*
 * conexiones.h
 *
 *  Created on: 26/9/2016
 *      Author: utnso
 */

#ifndef LIB_CONEXIONES_H_
#define LIB_CONEXIONES_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
	int socketEscucha;
	int socketMasGrande;
	pthread_t hilo;
	fd_set sockets_activos;
} nodo_hilo;

pthread_mutex_t mutex_mayor;

int atenderConexiones();

void handshake(int socket_nueva_conexion, fd_set sockets_activos);

void atenderConexion(int socket_conexion);

void establecerConexion(void * data);


#endif /* LIB_CONEXIONES_H_ */

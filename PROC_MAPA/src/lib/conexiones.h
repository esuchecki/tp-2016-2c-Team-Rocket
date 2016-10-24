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
#include <so/libSockets.h>
#include <pthread.h>

#include "estructurasMapa.h"

//temporalmente
#include "libGrafica.h"

int socketMasGrande;
fd_set sockets_activos;
pthread_mutex_t mutex_sock;

int atenderConexiones(void * data);

void handshake(int socket_nueva_conexion, fd_set sockets_activos, t_mapa * unMapa);

int atenderConexion(int i, t_mapa * unMapa,fd_set sockets_activos);

void detectarDesconexion(t_data * paquete,int socket_recepcion,fd_set sockets_activos,t_mapa * mapa);


#endif /* LIB_CONEXIONES_H_ */

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

#include "estructurasMapa.h"

//temporalmente
#include "libGrafica.h"

int socketMasGrande;


int atenderConexiones(void * data);

void handshake(int socket_nueva_conexion, fd_set sockets_activos, t_mapa * unMapa);

int atenderConexion(int i, t_mapa * unMapa);

void detectarDesconexion(t_data * paquete,int socket_recepcion,fd_set sockets_activos,t_mapa * mapa);


#endif /* LIB_CONEXIONES_H_ */

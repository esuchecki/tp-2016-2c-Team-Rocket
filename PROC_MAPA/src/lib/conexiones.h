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

int atenderConexiones(void * data);

void handshake(int socket_nueva_conexion, fd_set sockets_activos);

void atenderConexion(int i, fd_set sockets_activos);


#endif /* LIB_CONEXIONES_H_ */

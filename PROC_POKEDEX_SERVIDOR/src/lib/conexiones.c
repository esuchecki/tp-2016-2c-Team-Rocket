/*
 * conexiones.c
 *
 *  Created on: 26/9/2016
 *      Author: utnso
 */
#include "conexiones.h"

#include <so/libSockets.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>



void atenderConexion(int i, fd_set sockets_activos) {
	//Me llega un mensaje del pokedex cliente
	t_data * paquete;
	paquete = leer_paquete(i);

	switch (paquete->header) {
	default:
			break;
	}
}

void handshake(int socket_nueva_conexion, fd_set sockets_activos) {

}


int atenderConexiones() {

	int socketEscucha, socketMasGrande;

	fd_set sockets_para_revisar,sockets_activos;

	socketEscucha = setup_listen("localhost", "6800");

	printf("%d\n",socketEscucha);

	listen(socketEscucha, 1024);

	socketMasGrande = socketEscucha;
	FD_ZERO(&sockets_activos);
	FD_SET(socketEscucha, &sockets_activos);

	while (1) {
		labelSelect: sockets_para_revisar = sockets_activos;

		int retornoSelect = select(socketMasGrande + 1, &sockets_para_revisar,
		NULL, NULL, NULL);

		if (retornoSelect == -1) {
			goto labelSelect;
		}
		int i;

		for (i = 0; i <= socketMasGrande; i++) {
			if (FD_ISSET(i, &sockets_para_revisar)) {
				//hay actividad
				if (i == socketEscucha) {
					//es una nueva conexion sobre el puerto de escucha
					struct sockaddr_storage remoteaddr;
					socklen_t addrlen;
					addrlen = sizeof(remoteaddr);

					int socket_nueva_conexion = accept(socketEscucha,
							(struct sockaddr *) &remoteaddr, &addrlen);
					if (socket_nueva_conexion == -1) {
						//se desconecto o no conecto alguno
						printf("error al asignar socket a la nueva conexion");

					} else {
						//Ponemos al socket nuevo en el set de sockets activos
						FD_SET(socket_nueva_conexion, &sockets_activos);

						if (socket_nueva_conexion > socketMasGrande) {
							socketMasGrande = socket_nueva_conexion;
						}
						handshake(socket_nueva_conexion, sockets_activos);
					}
				} else {
					//la actividad es un puerto ya enlazado, hay que atenderlo
					atenderConexion(i, sockets_activos);
				}
			}
		}
	}
}




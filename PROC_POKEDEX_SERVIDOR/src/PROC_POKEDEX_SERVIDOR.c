/*
 * PROC_POKEDEX_SERVIDOR.c
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <libSockets.h>
#include <libPlanificador.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char*argv[]) {

	//char * Port = malloc(10);
	//TODO: Conseguir el puerto de escucha por archivo de config

	int socketEscucha = setup_listen("localhost", "6100");

	listen(socketEscucha, 1024);

	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;
	addrlen = sizeof(remoteaddr);

	int socket_nueva_conexion = accept(socketEscucha,
			(struct sockaddr *) &remoteaddr, &addrlen);

	if (socket_nueva_conexion < 0) {
		printf("No se pudo conectar el pokedex cliente\n");
		exit(EXIT_FAILURE);
	} else {
		printf("Se conecto con exito el pokedex cliente\n");
		while (1) {
			t_data *paquete = leer_paquete(socket_nueva_conexion);
			printf("Paquete recibido\n");
			//TODO: Hacer lo que tenga que hacer el proceso
		}
	}

	return 0;
}

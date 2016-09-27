/*
 * conexiones.c

 *
 *  Created on: 26/9/2016
 *      Author: utnso
 */

#include "conexiones.h"
#include "libPlanificador.h"

#include <so/libSockets.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void detectarDesconexion(t_data * paquete, int socket_recepcion,
		fd_set sockets_activos) {

	if (paquete->header == 0) {
		//desconexion
		printf("se desconecto alguien\n");
		printf("Elimine el numerode socket: %d\n", socket_recepcion);
		desconectarEntrenador(socket_recepcion);
		close(socket_recepcion);
		FD_CLR(socket_recepcion, &sockets_activos);
		if (socket_recepcion == socketMasGrande) {
			socketMasGrande = 0;
			int fd2 = 0;
			for (fd2 = socket_recepcion - 1; fd2 >= 0; fd2--) {
				if (FD_ISSET(fd2, &sockets_activos)) {
					socketMasGrande = fd2;
					printf("el socket mas grande ahora es:%d\n",fd2);
					break;

				}
			}
		}
	}
}

void atenderConexion(int i, fd_set sockets_activos) {
	//hago lo que tenga que hacer cuando se conecte un entrenador
	t_data * paquete;
	paquete = leer_paquete(i);

	detectarDesconexion(paquete, i, sockets_activos);

	switch (paquete->header) {
	case peticionPokenest:
		;
		char * identificadorPokenest = paquete->data;
		printf("El identificador de la pokenest es: %c\n",
				*identificadorPokenest);
		//int  coordenadas = obtenerCoordenadasPokenest(*identificadorPokenest);
		//hardocodeo las coordenadas
		int coordenadaEnX = 4;
		int coordenadaEnY = 5;
		void * buffer = malloc(sizeof(int) * 2);
		memcpy(buffer, &coordenadaEnX, sizeof(int));
		memcpy(buffer + sizeof(int), &coordenadaEnY, sizeof(int));

		t_data * nuevoPaquete = pedirPaquete(ubicacionPokenest, sizeof(int) * 2,
				buffer);

		common_send(i, nuevoPaquete);

		consumirQuantum(i);

		free(nuevoPaquete);

		sem_post(&entrenador_listo);

		break;
	case movimientoEntrenador:
		//TODO: registrar movimiento del entrenador, descartar quantum
		;

		int coordenadasEnX = 0;

		int coordenadasEnY = 0;

		memcpy(&coordenadasEnX, paquete->data, sizeof(int));

		memcpy(&coordenadasEnY, paquete->data + sizeof(int), sizeof(int));

		printf("Las coordenadas de la pokenest son: %d , %d\n", coordenadasEnX,
				coordenadasEnY);

		consumirQuantum(i);

		sem_post(&entrenador_listo);

		break;
	case capturarPokemon:
		;

		t_entrenador * entrenador = list_remove(colaListos, 0);

		agregarAColaDeBloqueados(entrenador);

		consumirQuantum(i);

		sem_post(&entrenador_bloqueado);

		sem_post(&entrenador_listo);

		break;
	case objetivosCumplidos:
		//TODO: liberar recursos y desconectar entrenador
		break;
	case mejorPokemon:
		//TODO: recibe al mejor pokemon para... batalla pokemon?
		break;
	case 0:
		break;
	}
}

void handshake(int socket_nueva_conexion, fd_set sockets_activos) {
	t_data * paquete = leer_paquete(socket_nueva_conexion);

	if (paquete->header == 99) {

		t_entrenador *unEntrenador = generarEntrenador(socket_nueva_conexion,
				paquete->data);

		printf("el entrenador tiene socket: %d", unEntrenador->nroDesocket);

		agregarAColaDeListos(unEntrenador);

		sem_post(&entrenador_listo);
	} else {

		printf("No se pudo conectar, fallo el handshake\n");
		exit(EXIT_FAILURE);
	}
}

int atenderConexiones(void * data) {
//	t_mapa * mapa = data;

	int socketEscucha;

	fd_set sockets_para_revisar, sockets_activos;

	socketEscucha = setup_listen("localhost", "8001");

	printf("escucha: %d\n", socketEscucha);

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


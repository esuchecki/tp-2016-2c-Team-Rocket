/*
 * conexiones.c
 *
 *  Created on: 26/9/2016
 *      Author: utnso
 */
#include "conexiones.h"

#include <so/libSockets.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>



void atenderConexion(int socket_conexion) {
	//Me llega un mensaje del pokedex cliente

	t_data *paquete = leer_paquete(socket_conexion);

	//TODO: ver el tema de la sincronizacion.
	//pense que podriamos crear una lista con los archivos que se abrieron y el modo
	//en el que lo abrieron si para escritura o lectura cosa de que cada vez
	//que alguien haga alguna solicitud se fije ahi si puede realizarla o no.

	switch (paquete->header) {
	case solicitudReadAddr:
		;
		/*char * path = paquete->data;
		 *
		 * TODO:encontrar todos los nombres que contiene ese path detras de el
		 * intercalando un centinela %
		 *
		 */
		break;
	case solicitudGetAttr:
		;
//		char * path = paquete->data;
		//TODO: ver si el path es directorio o archivo
/*		long* atributos =  obtenerAtributos(path);
		switch(atributos[0]){
		case REGULAR:
			size_t size;
			paquete = pedirPaquete(respuestaPorArchivo,sizeof(size_t),&size);
			common_send(socket_conexion,paquete);
			break;
		case DIRECTORY:
			int null_data = 0;
			paquete = pedirPaquete(respuestaPorDirectorio,sizeof(int),&null_data);
			common_send(socket_conexion,paquete);
			break;
		case DELETED:
			int null_data = 0;
			paquete = pedirPaquete(errorGetAttr,sizeof(int),&null_data);
			common_send(socket_conexion,paquete);
			break;
		case -1:
			int null_data = 0;
			paquete = pedirPaquete(errorGetAttr,sizeof(int),&null_data);
			common_send(socket_conexion,paquete);
			break;
		}
*/
		break;
	case crearDirectorio:
		//este no se si va a ser un mensaje que le pida el cliente
		break;
	case borrarDirectorio:
		//este no se si va a ser un mensaje que le pida el cliente
		break;
	case abrirArchivo:
		//este no se si va a ser un mensaje que le pida el cliente
		break;
	case leerArchivo:
		;
		//char * path = paquete->data;
		t_data * posiciones = leer_paquete(socket_conexion);
		size_t size;
		off_t offset;
		memcpy(&size, &posiciones->data, sizeof(size_t));
		memcpy(&offset, &posiciones->data + sizeof(size_t), sizeof(off_t));

		//TODO: leer el archivo con path "path", tamanio size,y offset "offset"

		//TODO: enviarle el archivo al cliente

		break;
	case escribirArchivo:
		break;
	}

	free(paquete);
}

void handshake(int socket_nueva_conexion, fd_set sockets_activos) {

	t_data * paquete = leer_paquete(socket_nueva_conexion);

	if (paquete->header == 70) {

		int null_data = 0;

		paquete = pedirPaquete(71, sizeof(int), &null_data);

		common_send(socket_nueva_conexion, paquete);

	} else {

		printf(
				"Error en la conexion con el proceso pokedex cliente, cierro programa");

		exit(EXIT_FAILURE);

	}
}

int atenderConexiones(char* ip, char* puerto) {

	pthread_mutex_init(&mutex_mayor,NULL);

	int socketEscucha, socketMasGrande;

	fd_set sockets_para_revisar, sockets_activos;

	socketEscucha = setup_listen(ip, puerto);

	printf("%d\n", socketEscucha);

	listen(socketEscucha, 1024);

	socketMasGrande = socketEscucha;
	FD_ZERO(&sockets_activos);
	FD_SET(socketEscucha, &sockets_activos);

	while (1) {
		sockets_para_revisar = sockets_activos;

		int retornoSelect = select(socketMasGrande + 1, &sockets_para_revisar,
		NULL, NULL, NULL);

		if (retornoSelect == -1) {

		}
		int i;

		for (i = 0; i <= socketMasGrande; i++) {
			if (FD_ISSET(i, &sockets_para_revisar)) {
				//hay actividad
				if (i == socketEscucha) {
					//es una nueva conexion sobre el puerto de escucha

						pthread_t manejoCliente;

						pthread_attr_t attrHilo;
						pthread_attr_init(&attrHilo);
						pthread_attr_setdetachstate(&attrHilo,
						PTHREAD_CREATE_DETACHED);

						nodo_hilo * nodo = malloc(sizeof(nodo_hilo));
						nodo->hilo = manejoCliente;
						nodo->socketMasGrande = socketMasGrande;
						nodo->socketEscucha = socketEscucha;
						nodo->sockets_activos = sockets_activos;

						pthread_create(&manejoCliente, &attrHilo,
								(void *) establecerConexion, (void *) nodo);


				} else {
					//la actividad es un puerto ya enlazado, hay que atenderlo
					//atenderConexion(i, sockets_activos);

				}
			}
		}
	}
}

void establecerConexion(void * data) {

	nodo_hilo *datos = data;

	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;
	addrlen = sizeof(remoteaddr);

	int socket_nueva_conexion = accept(datos->socketEscucha,
			(struct sockaddr *) &remoteaddr, &addrlen);

	if (socket_nueva_conexion == -1) {
		//se desconecto o no conecto alguno
		printf("error al asignar socket a la nueva conexion");

	} else {
		//Ponemos al socket nuevo en el set de sockets activos
		FD_SET(socket_nueva_conexion, &datos->sockets_activos);

		pthread_mutex_lock(&mutex_mayor);
		if (socket_nueva_conexion > datos->socketMasGrande) {
			datos->socketMasGrande = socket_nueva_conexion;
		}
		pthread_mutex_unlock(&mutex_mayor);

		handshake(socket_nueva_conexion, datos->sockets_activos);

	}

	while (1) {
		atenderConexion(socket_nueva_conexion);
	}
}

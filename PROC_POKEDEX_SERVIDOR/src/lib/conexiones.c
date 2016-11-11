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
//TODO: cambiar estos include de lugar!.
#include "../OSADA_FS/src/OSADA_Constants.h"
#include "../OSADA_FS/src/osada-utils/osada.h"
#include "../OSADA_FS/src/osada_functions.h"
#include "../libTeamRocket/so/libSockets.h"

void atender(int socket);

void atenderConexion(int socket_conexion) {
	//Me llega un mensaje del pokedex cliente

	printf("entro a atender conexion\n");
	t_data *paquete = leer_paquete(socket_conexion);
	printf("leyo paquete\n");
	char * path = paquete->data;
	size_t size = 0;
	off_t offset = 0;
	int null_data;
	//TODO: ver el tema de la sincronizacion.
	//pense que podriamos crear una lista con los archivos que se abrieron y el modo
	//en el que lo abrieron si para escritura o lectura cosa de que cada vez
	//que alguien haga alguna solicitud se fije ahi si puede realizarla o no.

	switch (paquete->header) {
	case poke_solicitudReadDir:
		;
		printf("Me pidieron el readDir de: %s\n", path);
		char ** directorios = leerDirectorio(path);

		int temp = 0;
		int aux = sizeof(char) * (OSADA_FILENAME_LENGTH + 1);
		char * buffer = malloc(sizeof(char));
		//char * buffer = malloc(sizeof(char) * (OSADA_FILENAME_LENGTH+1) );
		while (directorios[temp] != NULL) {
			buffer = realloc(buffer, aux * (temp + 1));
			memcpy(buffer + (aux * temp), directorios[temp],
			OSADA_FILENAME_LENGTH);
			//Me aseguro que en la ultima posicion haya un '\0'
			char a = '\0';
			memcpy(buffer + (aux * temp) + OSADA_FILENAME_LENGTH, &a,
					sizeof(char));
			temp++;
		}

		if (temp > 0)	//Tenia al menos un archivo el directorio.
				{
			paquete = pedirPaquete(poke_respuestaReadDir, aux * (temp), buffer);
			common_send(socket_conexion, paquete);
		} else {	//No tiene archivos.
			paquete = pedirPaquete(poke_errorReadDir, sizeof(int), &null_data);
			common_send(socket_conexion, paquete);
		}

		//le pongo un NULL al final.
		//temp++;
		//buffer = realloc(buffer, sizeof(char) * (temp) * (OSADA_FILENAME_LENGTH+1) );
		//buffer[temp-1] = NULL;

//		char hola[OSADA_FILENAME_LENGTH+1];
//		strcpy(hola, directorios[0]);
		//= directorios[0];
		//sendeamos
		//size_t size;

		free(buffer);

		break;
	case poke_solicitudGetAttr:
		;

		//TODO: ver si el path es directorio o archivo
		printf("Me pidieron el getAttr de: %s\n", path);
		long* atributos = obtenerAtributos(path);

		switch (atributos[0]) {
		case REGULAR:
			;
			//size= (size_t) atributos[1];
			paquete = pedirPaquete(poke_respuestaPorArchivo, sizeof(long),
					&atributos[1]);
			common_send(socket_conexion, paquete);
			//size = 0;
			break;
		case DIRECTORY:
			;
			null_data = 0;
			paquete = pedirPaquete(poke_respuestaPorDirectorio, sizeof(int),
					&null_data);
			common_send(socket_conexion, paquete);
			break;
		case DELETED:
			;
			null_data = 0;
			paquete = pedirPaquete(poke_errorGetAttr, sizeof(int), &null_data);
			common_send(socket_conexion, paquete);
			break;
		default:
			;
			null_data = 0;
			paquete = pedirPaquete(poke_errorGetAttr, sizeof(int), &null_data);
			common_send(socket_conexion, paquete);
			break;
		}

		break;
	case poke_crearDirectorio:
		printf("Me pidieron crear el dir: %s\n", path);
		int directorio = -1;
		directorio = crearDirectorio(path);
		//printf("Rta= %d\n", directorio);

		paquete = pedirPaquete(poke_respuestaCreacion, sizeof(int),
				&directorio);
		common_send(socket_conexion, paquete);
		break;

	case poke_borrarDirectorio:
		printf("Me pidieron borrar el dir: %s\n", path);

		int directorioABorrar = borrarDirectorio(path);
		paquete = pedirPaquete(poke_respuestaBorrado, sizeof(int),
				&directorioABorrar);
		common_send(socket_conexion, paquete);
		break;
	case poke_borrarArchivo:
		printf("Me pidieron borrar el file: %s\n", path);

		int archivoABorrar = borrarArchivo(path);
		paquete = pedirPaquete(poke_respuestaBorradoArchivo, sizeof(int),
				&archivoABorrar);
		common_send(socket_conexion, paquete);
		break;
	case poke_abrirArchivo:
		//este no se si va a ser un mensaje que le pida el cliente
		break;
	case poke_leerArchivo:
		;
		//char * path = paquete->data;
		t_data * posiciones = leer_paquete(socket_conexion);
		memcpy(&size, posiciones->data, sizeof(size_t));
		memcpy(&offset, posiciones->data + sizeof(size_t), sizeof(off_t));

		printf("Me pidieron leer el Archivo: %s\n", path);
		printf("size: %zu, offset: %jd\n", size, (intmax_t) offset);

		//TODO: leer el archivo con path "path", tamanio size,y offset "offset"
		osada_block* archivo = obtenerArchivoPorPath(path, size, offset);
		//printf("%s\n",archivo);
		//Hardcodeo el 556 para que lea README.txt
		paquete = pedirPaquete(poke_respuestaLectura, 556, archivo);
		common_send(socket_conexion, paquete);

		break;
	case poke_escribirArchivo:
		break;
		//case -1:
	case poke_renombrar:
		;
		/*
		 *
		 * ESTO NO IRIA... VERSION CON BUFFER
		 int largoPath;

		 memcpy(&largoPath, paquete->data,sizeof(int));

		 char* path = malloc(largoPath);

		 memcpy(&path , paquete->data+sizeof(int), largoPath);

		 int largoName;

		 memcpy(&largoName, paquete->data + sizeof(int) + largoPath,sizeof(int));

		 char* nombre = malloc(largoName);

		 memcpy(&nombre , paquete->data+sizeof(int)+largoPath+largoName, largoName);
		 */
		t_data *paquete2 = leer_paquete(socket_conexion);
		printf("leyo paquete2 \n");

		char* nombre = paquete2->data;
		printf("Me pidieron renombrar: %s, %s \n", path, nombre);
		int renombro = cambiarNombre(path, nombre);
		//int renombro = cambiarNombre( lectura2->data, path);
		paquete = pedirPaquete(poke_respuestaRenombrado, sizeof(int),
				&renombro);
		common_send(socket_conexion, paquete);

		break;
	default:
		;
		pthread_exit(0);
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

	pthread_mutex_init(&mutex_mayor, NULL);

	int socketEscucha, socketMasGrande;

	fd_set sockets_para_revisar, sockets_activos;

	socketEscucha = setup_listen(ip, puerto);

	printf("Estoy escuchando clientes en el socket %d\n", socketEscucha);

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

		printf("Soy una nueva conexion.\n");
	}

	while (1) {
		atenderConexion(socket_nueva_conexion);
	}
}

/*
 char * guardarConCentinela(char **paquete, int * sizeOfBuffer) {
 int i;
 char caracter;
 char centinela = '\0';

 for (i = 0; caracter != centinela; i++) {

 caracter = paquete[i];

 if (caracter == centinela) {
 i++;
 break;
 }
 }

 char * buffer = malloc(sizeof(char) * i);

 caracter = paquete[0];

 for (i = 0; caracter != centinela; i++) {
 caracter = paquete[i];
 if (caracter == centinela) {
 break;
 }
 memcpy(buffer + i, paquete + i, sizeof(char));
 }

 buffer[i] = '\0';

 return buffer;
 }
 */

//Codigo de Maxi Santos
void escucharNuevasConexiones(char* ip, char *puerto) {

	int socketEscucha = setup_listen(ip, puerto);
	int i = 0;

	printf("Estoy escuchando clientes en el socket %d\n", socketEscucha);
	listen(socketEscucha, 1024);

	pthread_t hilos[20];
	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;
	addrlen = sizeof(remoteaddr);

	//Aca abrim

	while (1) {
		int socket_nueva_conexion = accept(socketEscucha,
				(struct sockaddr *) &remoteaddr, &addrlen);
		//pthread_create(&hilos[i], NULL,(void *) atender,(void*) socket_nueva_conexion);;
		atender(socket_nueva_conexion);
		printf("Este es el cliente numero %d", i);
		i++;
	}
}

//Codigo de Maxi Santos
void atender(int socket) {
	nodo_hilo * nodo = malloc(sizeof(nodo_hilo));
	handshake(socket, nodo->sockets_activos);
	printf("Nuevo Socket: %d\n", socket);
	while (1) {
		atenderConexion(socket);
	}
}

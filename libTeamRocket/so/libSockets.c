/*
 * libSockets.c
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#include "../so/libSockets.h"
#include <commons/log.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "constantes.h"
//#include "../so/libPlanificador.h"

int setup_listen(char * IP, char * Port) {
	struct addrinfo * serverInfo = common_setup(IP, Port);
	if (serverInfo == NULL) {
		exit(-1);
	}
	int socketEscucha;

	socketEscucha = socket(serverInfo->ai_family, serverInfo->ai_socktype,
			serverInfo->ai_protocol);
	int yes = 1;
	if (setsockopt(socketEscucha, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))
			== -1) {
		perror("setsockopt");
		exit(1);
	}
	if (bind(socketEscucha, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0) {
		printf("Puerto en Time-wait, error del bind");
		exit(-1);
	}
	freeaddrinfo(serverInfo);
	return socketEscucha;

}
struct addrinfo* common_setup(char * IP, char * Port) {
	struct addrinfo hints;
	struct addrinfo * serverInfo;
	int error;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;

	if (!strcmp(IP, "localhost")) {
		hints.ai_flags = AI_PASSIVE;
		error = getaddrinfo(NULL, Port, &hints, &serverInfo);
	} else {
		error = getaddrinfo(IP, Port, &hints, &serverInfo);
	}
	if (error != 0) {
		//printf("Problema con el getaddrinfo(): %d",(gai_stdrerror(error)));
		return NULL;
	}
	return serverInfo;
}

int connect_to(char* IP, char * port) {
	struct addrinfo * serverInfo = common_setup(IP, port);
	if (serverInfo == NULL) {
		return -1;
	}
	int serverSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype,
			serverInfo->ai_protocol);

	if (serverSocket == -1) {
		return -1;
	}
	if (connect(serverSocket, serverInfo->ai_addr, serverInfo->ai_addrlen)
			== -1) {
		close(serverSocket);
		return -1;
	}
	freeaddrinfo(serverInfo);
	return serverSocket;
}

t_data * leer_paquete(int socket) {
	t_data * paquete_entrante = malloc(sizeof(t_data));
	int resultado;
	label1: resultado = recv(socket, &paquete_entrante->header, sizeof(int),MSG_WAITALL);
	if(resultado != 4){
		goto label1;
	}
	label2: resultado = recv(socket, &paquete_entrante->tamanio, sizeof(int), MSG_WAITALL);
	if(resultado != 4){
		goto label2;
	}
	paquete_entrante->data = malloc(paquete_entrante->tamanio);

	label3: resultado = recv(socket, paquete_entrante->data, paquete_entrante->tamanio,	MSG_WAITALL);
	if(resultado != paquete_entrante->tamanio){
		goto label3;
	}
	return paquete_entrante;

}

t_data * pedirPaquete(int header, int tamanio, void * data) {
	t_data * paquete = malloc(sizeof(t_data));

	paquete->header = header;
	paquete->tamanio = tamanio;
	paquete->data = data;

	return paquete;
}

char * serializar(t_data * unPaquete) {
	void * buffer = malloc(sizeof(int) + sizeof(int) + unPaquete->tamanio);

	memcpy(buffer, &unPaquete->header, sizeof(int));
	memcpy(buffer + sizeof(int), &unPaquete->tamanio, sizeof(int));
	memcpy(buffer + sizeof(int) + sizeof(int), unPaquete->data,
			unPaquete->tamanio);

	return buffer;

}

int common_send(int socket, t_data * paquete) {
	void * buffer;
	int tamanioTotal;
	tamanioTotal = paquete->tamanio + sizeof(int) + sizeof(int);
	buffer = serializar(paquete);

	int resultado;
	label: resultado = send(socket, buffer, tamanioTotal, MSG_WAITALL);
	if(resultado != tamanioTotal){
		goto label;
	}
	free(buffer);
	return resultado;
}


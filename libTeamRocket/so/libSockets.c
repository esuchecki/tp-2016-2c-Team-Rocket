/*
 * libSockets.c
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#include "../so/libSockets.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "../so/libPlanificador.h"

void handshake(int i,fd_set sockets_activos){
	t_data * paquete = leer_paquete(i);
	if (paquete->header == 99){

		t_entrenador *unEntrenador = generarEntrenador(i);

		agregarAColaDeListos(unEntrenador);

	}else{

		printf("No se pudo conectar\n");
		exit(EXIT_FAILURE);
	}
}

struct addrinfo* common_setup(char * IP, char * Port){
	struct addrinfo hints;
	struct addrinfo * serverInfo;
	int error;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (!strcmp(IP,"localhost")){
		hints.ai_flags = AI_PASSIVE;
		error = getaddrinfo(NULL,Port,&hints,&serverInfo);
	}else{
		error = getaddrinfo(IP,Port,&hints,&serverInfo);
	}
	if (error){
		//printf("Problema con el getaddrinfo(): %d",(gai_stdrerror(error)));
		return NULL;
	}
	return serverInfo;
}

int setup_listen(char * IP, char * Port){
	struct addrinfo * serverInfo = common_setup(IP, Port);
	if (serverInfo == NULL){
		exit(-1);
	}
	int socketEscucha;

	socketEscucha = socket(serverInfo->ai_family,serverInfo->ai_socktype,serverInfo->ai_protocol);

	if(bind(socketEscucha,serverInfo->ai_addr,serverInfo->ai_addrlen )< 0){
		printf("Puerto en Time-wait, error del bind");
		int yes =1;
		if (setsockopt(socketEscucha, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		    perror("setsockopt");
		    exit(1);
		}
		exit(-1);
	}
	freeaddrinfo(serverInfo);
	return socketEscucha;

}

int connect_to(char* IP, char * port){
	struct addrinfo * serverInfo = common_setup(IP,port);
	if(serverInfo == NULL){ return -1;}
	int serverSocket = socket(serverInfo->ai_family,serverInfo->ai_socktype,serverInfo->ai_protocol);
	if (serverSocket == -1){
		return -1;
	}
	if (connect(serverSocket,serverInfo->ai_addr,serverInfo->ai_addrlen)==-1){
		close(serverSocket);
		return -1;
	}
	freeaddrinfo(serverInfo);
	return serverSocket;
}

t_data * leer_paquete(int socket){
	t_data * paquete_entrante = malloc(sizeof(t_data));

	recv(socket,&paquete_entrante->header,sizeof(int),MSG_WAITALL);
	recv(socket,&paquete_entrante->tamanio,sizeof(int),MSG_WAITALL);

	paquete_entrante->data = malloc(paquete_entrante->tamanio);

	recv(socket,paquete_entrante->data,paquete_entrante->tamanio,MSG_WAITALL);

	return paquete_entrante;
}

t_data * pedirPaquete(int header,int tamanio,void * data){
	t_data * paquete = malloc(sizeof(t_data));

	paquete->header = header;
	paquete->tamanio = tamanio;
	paquete->data = data;

	return paquete;
}

char * serializar(t_data * unPaquete){
	void * buffer = malloc(sizeof(int) + sizeof(int) + unPaquete->tamanio);

	memcpy(buffer,&unPaquete->header,sizeof(int));
	memcpy(buffer + sizeof(int),&unPaquete->tamanio,sizeof(int));
	memcpy(buffer + sizeof(int) + sizeof(int),unPaquete->data,unPaquete->tamanio);

	return buffer;

}

void common_send(int socket, t_data * paquete){
	void * buffer;
	int tamanioTotal;
	tamanioTotal = paquete->tamanio  + sizeof(int) + sizeof(int);
	buffer = serializar(paquete);

	send(socket,buffer,tamanioTotal,MSG_WAITALL);

	free(buffer);
}

void atenderConexion(int i,fd_set sockets_activos){

  //hago lo que tenga que hacer cuando se conecte un entrenador
	t_data *prueba;
	prueba = leer_paquete(i);
	if (prueba->header == 1){
		char  msj[20];
		strcpy(msj,"Su turno entrenador");
		prueba = pedirPaquete(1,20,msj);
		common_send(i,prueba);
	}

}

int atenderConexiones(void *Puerto){
	char * PuertoEscucha = Puerto;
	printf("%s\n", PuertoEscucha);

	int socketEscucha;
	fd_set sockets_activos,sockets_para_revisar;

	socketEscucha = setup_listen("localhost",PuertoEscucha);

	listen(socketEscucha,1024);

	int socketMasGrande = socketEscucha;
	FD_ZERO(&sockets_activos);
	FD_SET(socketEscucha,&sockets_activos);

	while(1){
		labelSelect: sockets_para_revisar = sockets_activos;

		int retornoSelect = select(socketMasGrande + 1,&sockets_para_revisar,NULL,NULL,NULL);

		if(retornoSelect == -1){
			goto labelSelect;
		}
		int i;

		for (i = 0; i <= socketMasGrande;i++){
			if (FD_ISSET(i,&sockets_para_revisar)){
				//hay actividad
				if (i == socketEscucha){
					//es una nueva conexion sobre el puerto de escucha
					struct sockaddr_storage remoteaddr;
					socklen_t addrlen;
					addrlen = sizeof(remoteaddr);

					int socket_nueva_conexion = accept(socketEscucha,(struct sockaddr * )&remoteaddr,&addrlen);
					if (socket_nueva_conexion == -1){
						//se desconecto o no conecto alguno
						printf("error al asignar socket a la nueva conexion");

					}else{
						//Ponemos al socket nuevo en el set de sockets activos
						FD_SET(socket_nueva_conexion,&sockets_activos);
						printf("el socket de conexion es: %d\n",socket_nueva_conexion);

						if(socket_nueva_conexion > socketMasGrande){
							socketMasGrande = socket_nueva_conexion;
						}
						handshake(socket_nueva_conexion,sockets_activos);
					}
				}else{
					//la actividad es un puerto ya enlazado, hay que atenderlo
					atenderConexion(i,sockets_activos);
				}
			}
		}
	}
}


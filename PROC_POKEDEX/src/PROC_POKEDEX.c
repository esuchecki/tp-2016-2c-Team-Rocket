/*
 ============================================================================
 Name        : PROC_POKEDEX.c
 Author      : TEAM ROCKET
 Version     :
 Copyright   : TEAM ROCKET
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

//prueba incluir commons
#include <commons/log.h>

int main(void) {
	puts("PROCESO POKEDEX"); /* prints PROCESO POKEDEX */
	return EXIT_SUCCESS;
}

//EJEMPLO DEL MAIN PARA USO DEL POKEDEX SERVIDOR
/*
int main(int argc,char*argv[]){

	char * Port;
	//TODO: Conseguir el puerto de escucha por archivo de config

	int socketEscucha = setup_listen("localhost",Port);

	listen(socketEscucha,1024);

	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;
	addrlen = sizeof(remoteaddr);

	int socket_nueva_conexion = accept(socketEscucha,(struct sockaddr * )&remoteaddr,&addrlen);

	if(socket_nueva_conexion < 0){
		printf("No se pudo conectar el pokedex cliente\n");
		exit(EXIT_FAILURE);
	}else{
		printf("Se conecto con exito el pokedex cliente\n");


		//TODO: Hacer lo que tenga que hacer el proceso
	}
	return 0;
}
*/

//EJEMPLO DEL MAIN PARA EL USO DEL POKEDEX CLIENTE
/*
int main(int argc,char*argv[]){

	char * IP;
	char * Puerto;
	//TODO: obtener de archivo de configs el ip y puerto

	int socket = connect_to(IP,Puerto);

	//TODO: Hacer lo que tenga que hacer el proceso

	return 0;
}
*/

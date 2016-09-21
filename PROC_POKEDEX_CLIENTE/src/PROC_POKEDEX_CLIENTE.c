/*
 * PROC_POKEDEX_CLIENTE.c
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <so/libSockets.h>
#include <string.h>

int main(int argc,char*argv[]){

	char * IP;
	char * Puerto;
	//TODO: obtener de archivo de configs el ip y puerto

	int socket = connect_to("localhost","6100");

	//TODO: Hacer lo que tenga que hacer el proceso
	char *mensaje = malloc(15);
	strcpy(mensaje,"Hola servidor");
	int tamanio = strlen(mensaje);

	t_data * paqueteHandshake = pedirPaquete(1,tamanio,mensaje);

	common_send(socket,paqueteHandshake);

	return EXIT_SUCCESS;
}


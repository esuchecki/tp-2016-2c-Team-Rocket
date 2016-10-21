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

#include "lib/teamRocketFuse.h"


int main(int argc,char*argv[]){

	/*
	char * IP;
	char * Puerto;
	//TODO: obtener de archivo de configs el ip y puerto

	int socketConexion = connect_to("localhost","6100");

	//TODO: Hacer lo que tenga que hacer el proceso
	 *
	int null_data = 0;

	t_data * paqueteHandshake = pedirPaquete(70,sizeof(int),&null_data);

	common_send(socketConexion,paqueteHandshake);

	paqueteHandshake = leer_paquete(socketConexion);
	if(paqueteHandshake->header == 71){
		printf("La conexion con el servidor es un exito");
	}else{
		printf("La conexion con el servidor fallo");
		exit(EXIT_FAILURE);
	}
	 */


	//aca ejecuta la sentencia de fuse.
	return iniciarFuse(argc, argv);

}

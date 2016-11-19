/*
 * PROC_POKEDEX_CLIENTE.c
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <so/libSockets.h>
#include <so/libConfig.h>
#include <commons/log.h>
#include <string.h>
#include "lib/teamRocketFuse.h"
#include "PROC_POKEDEX_CLIENTE.h"

int main(int argc,char*argv[]){


	char * IP;
	char * Puerto;


	//Para leer la config como parametro de ejecucion
	//char * pathConfg = argv[1];

	//inicializarLogCliente(argv,true);

	t_config * config = newConfigType(pathConfigCli);
	//Consigo la ip
	IP = configLeerString(config,"IP");
	if(IP == NULL){
		exit(EXIT_FAILURE);
	}
	//Consigo el puerto
	Puerto = configLeerString(config,"PUERTO");
	if(Puerto == NULL){
		exit(EXIT_FAILURE);
	}

	inicializarLogCliente(argv,true);


	//socketConexion = connect_to("localhost","6100");
	int socketConexion = connect_to(IP,Puerto);


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

	log_info(logCliente, "My socket connection is: %s", string_itoa(socketConexion));
	//TODO: FUSE
	return iniciarFuse(argc, argv);

}

void inicializarLogCliente( char *argv[],bool consolaOn )
{

	logCliente = log_create("/home/utnso/Escritorio/log.txt" , argv[0], consolaOn, LOG_LEVEL_DEBUG);


	if (logCliente != NULL)
	{
		puts("Se creo OK el arch de log");
	}
	else
	{
		puts("NO se pudo crear el archivo de log");
		exit(EXIT_FAILURE);
	}
	log_info(logCliente, "-------------------------------------------------------");

}

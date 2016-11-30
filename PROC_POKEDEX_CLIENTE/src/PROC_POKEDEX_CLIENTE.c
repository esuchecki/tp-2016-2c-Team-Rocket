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
		printf("Error al leer la config de la IP..\n\n");
		exit(EXIT_FAILURE);
	}
	//Consigo el puerto
	Puerto = configLeerString(config,"PUERTO");
	if(Puerto == NULL){
		printf("Error al leer la config del PUERTO.\n\n");
		exit(EXIT_FAILURE);
	}

	inicializarLogCliente(argv,false);


	printf("Me voy a conectar a: IP=%s, Puerto=%s.\n", IP, Puerto);
	//socketConexion = connect_to("localhost","6100");
	socketConexion = connect_to(IP,Puerto);


	int null_data = 0;

	t_data * paqueteHandshake = pedirPaquete(70,sizeof(int),&null_data);

	common_send(socketConexion,paqueteHandshake);

	paqueteHandshake = leer_paquete(socketConexion);
	if(paqueteHandshake->header == 71){
		printf("La conexion con el servidor es un exito.\nVoy a iniciar Fuse.\n\n");
	}else{
		printf("La conexion con el servidor fallo.\n\n");
		exit(EXIT_FAILURE);
	}

	log_info(logCliente, "My socket connection is: %s", string_itoa(socketConexion));
	//TODO: FUSE
	return iniciarFuse(argc, argv);

}





void inicializarLogCliente( char *argv[],bool consolaOn )
{

	logCliente = log_create("/home/utnso/tp-2016-2c-Team-Rocket/Build/logCliente.txt" , argv[0], consolaOn, LOG_LEVEL_INFO);


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

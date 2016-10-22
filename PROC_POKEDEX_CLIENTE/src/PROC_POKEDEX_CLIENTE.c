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

t_log * logCliente;

#define pathConfig "/home/utnso/git/tp-2016-2c-Team-Rocket/PROC_POKEDEX_CLIENTE/config_cliente.txt"

void inicializarLogCliente ( char *argv[] );

int main(int argc,char*argv[]){


	char * IP;
	char * Puerto;
	//Para leer la config como parametro de ejecucion
	//char * pathConfg = argv[1];

	t_config * config = newConfigType(pathConfig);
	//Configo la ip
	IP = configLeerString(config,"IP");
	if(IP == NULL){
		exit(EXIT_FAILURE);
	}
	//Consigo el puerto
	Puerto = configLeerString(config,"PUERTO");
	if(Puerto == NULL){
		exit(EXIT_FAILURE);
	}

	inicializarLogCliente(argv);

	//int socketConexion = connect_to("localhost","6100");
	int socketConexion = connect_to(IP,Puerto);

	//TODO: Hacer lo que tenga que hacer el proceso

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



	//aca ejecuta la sentencia de fuse.
	return iniciarFuse(argc, argv);

}

void inicializarLogCliente ( char *argv[] )
{

	logCliente = log_create("/home/utnso/git/tp-2016-2c-Team-Rocket/PROC_POKEDEX_CLIENTE/log" , argv[0], false, LOG_LEVEL_DEBUG);


	if (logCliente != NULL)
	{
		puts("se creo OK el arch de log");
	}
	else
	{
		puts("NO se pudo crear el archivo de log");
		exit(EXIT_FAILURE);
	}
	log_info(logCliente, "-------------------------------------------------------");

}

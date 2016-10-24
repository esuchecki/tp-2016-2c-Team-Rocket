/*
 * PROC_POKEDEX_SERVIDOR.c
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <so/libConfig.h>
#include "lib/conexiones.h"
#include <commons/log.h>

t_log * logServidor;

#define pathConfig "/home/utnso/git/tp-2016-2c-Team-Rocket/PROC_POKEDEX_SERVIDOR/config_servidor.txt"

int main(int argc, char*argv[]) {

	//char * Port = malloc(10);
	//TODO: Conseguir el puerto de escucha por archivo de config

	char* IP;
	char* PUERTO;
	//Para leer la config como parametro de ejecucion
		//char * pathConfg = argv[1];

		t_config * config = newConfigType(pathConfig);
		//Configo la ip
		IP = configLeerString(config,"IP");
		if(IP == NULL){
			exit(EXIT_FAILURE);
		}
		//Consigo el puerto
		PUERTO = configLeerString(config,"PUERTO");
		if(PUERTO == NULL){
			exit(EXIT_FAILURE);
		}

		inicializarLogServidor(argv);

	atenderConexiones(IP,PUERTO);

	return 0;
}

void inicializarLogServidor ( char *argv[] )
{

	logServidor = log_create("/home/utnso/git/tp-2016-2c-Team-Rocket/PROC_POKEDEX_SERVIDOR/log" , argv[0], false, LOG_LEVEL_DEBUG);


	if (logServidor != NULL)
	{
		puts("se creo OK el arch de log");
	}
	else
	{
		puts("NO se pudo crear el archivo de log");
		exit(EXIT_FAILURE);
	}
	log_info(logServidor, "-------------------------------------------------------");

}

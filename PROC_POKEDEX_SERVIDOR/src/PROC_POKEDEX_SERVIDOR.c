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
#include "PROC_POKEDEX_SERVIDOR.h"
//TODO: cambiar este
#include "../OSADA_FS/src/osada_functions.h"


int main(int argc, char*argv[]) {


	abrirArchivo();
	//char * Port = malloc(10);

	char* IP = malloc(15);
	char* PUERTO = malloc(15);
	strcpy(IP, "127.0.0.1");
	strcpy(PUERTO, "6100");
	/*
	//Para leer la config como parametro de ejecucion
		//char * pathConfg = argv[1];

		inicializarLogServidor(argv,true);

		t_config * config = newConfigType(pathConfig);
		//Configo la ip
		IP = configLeerString(config,"IP");
		if(IP == NULL){
			log_error(logServidor,"Fallo cargar la ip");
			exit(EXIT_FAILURE);
		}
		//Consigo el puerto
		PUERTO = configLeerString(config,"PUERTO");
		if(PUERTO == NULL){
			log_error(logServidor,"Fallo cargar el puerto");
			exit(EXIT_FAILURE);
		}


		log_debug(logServidor,"Voy a atender conexiones en la ip %s",IP);
		*/
	//atenderConexiones(IP,PUERTO);
	pthread_rwlock_init(&lecturaEscritura,NULL);
	escucharNuevasConexiones(IP,PUERTO);

	return 0;
}

void inicializarLogServidor ( char *argv[],bool consolaOn )
{

	logServidor = log_create("/home/utnso/git/tp-2016-2c-Team-Rocket/PROC_POKEDEX_SERVIDOR/log.txt" , argv[0], consolaOn, LOG_LEVEL_DEBUG);


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

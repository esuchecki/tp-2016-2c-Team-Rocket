/*
 ============================================================================
 Name        : PROC_ENTRENADOR.c
 Author      : TEAM ROCKET
 Version     :
 Copyright   : TEAM ROCKET
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//agreagado de librerias utn.so
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include <so/libSockets.h>
#include <so/libConfig.h>
#include <unistd.h>

#include "lib/estadisticas.h"
//librerias propias

#include "lib/estructurasEntrenador.h"
#include "lib/libConfigStruct.h"
#include "lib/seniales.h"
#include "so/constantes.h"
#include "lib/movimiento.h"


//------------------------------------------//
/* ********************************************	*/
//----------- Sector Funciones -------------//
void validarArgumentos(int argc, char *argv[]);
void inicializarLogEntrenador(char *argv[]);



//------------------------------------------//

int main(int argc, char *argv[]) {


	validarArgumentos(argc, argv);
	inicializarLogEntrenador(argv);

	t_entrenadorFisico * miEntrenador;
	miEntrenador = inicializarEstructurasDelEntrenador(argv[1], argv[2]);
	borrarDirectorioDeBill(miEntrenador);
	inicializarTiemposDelEntrenador(&miEntrenador->misEstadisticas);
	inicializarSenialesEntrenador(miEntrenador, (void *) &finalizarEntrenador);


	iniciarAventura(miEntrenador);

	finalizarEntrenador(miEntrenador);
	return EXIT_SUCCESS;
}

void validarArgumentos(int argc, char *argv[]) {
	puts("PROCESO ENTRENADOR"); /* prints PROCESO ENTRENADOR */

	//arg[0] -> nombre del proceso ejecutable
	//arg[1] -> nombre del Entrenador
	//arg[2] -> donde esta ubicada la pokedex
	if (argc != 3) /* debe recibir 3 argumentos */
	{
		printf("\tUso incorrecto..\n");
		printf("\tuso: %s nombreEntrenador dirPokeDex\n", argv[0]);
		printf("\tejemplo: %s %s %s\n", argv[0], "Ash",
				"/home/utnso/git/tp-2016-2c-Team-Rocket/PROC_ENTRENADOR/test_files");
		exit(EXIT_FAILURE);
	}

	if (argv[1] == NULL || (strlen(argv[1]) < 2)) {
		printf("\tRevise los parametros.\n");
		exit(EXIT_FAILURE);
	}
	if (argv[2] == NULL || (strlen(argv[2]) < 2)) {
		printf("\tRevise los parametros.\n");
		exit(EXIT_FAILURE);
	}

}

void inicializarLogEntrenador(char *argv[]) /*levanto el archivo para loggear*/
{
	//argv[0];	Program_NAME
	//argv[1];	Nombre del entrenador
	//string_from_format(__ubicacionArchivoDeLogEntrenador, argv[1])	//Con esto le paso el nombre del archivo de log.

	myArchivoDeLog = log_create(
			string_from_format(__ubicacionArchivoDeLogEntrenador, argv[1]),
			argv[0], false, LOG_LEVEL_INFO);
	if (myArchivoDeLog != NULL) {
		puts("se creo OK el arch de log");
	} else {
		puts("NO se pudo crear el archivo de log");
		exit(EXIT_FAILURE);
	}
	log_info(myArchivoDeLog,
			"-------------------------------------------------------");

}

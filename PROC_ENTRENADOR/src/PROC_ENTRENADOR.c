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
#include <so/libPlanificador.h>
#include <so/libConfig.h>
#include <unistd.h>

#include "lib/estadisticas.h"
//librerias propias

#include "lib/estructurasEntrenador.h"
#include "lib/libConfigStruct.h"

//------------------------------------------//
/* ********************************************	*/
//----------- Sector Funciones -------------//

void validarArgumentos ( int argc, char *argv[] );
void inicializarLogEntrenador (char *argv[]);
void inicializarSocketEntrenador ();
//------------------------------------------//






int main( int argc, char *argv[] )
{

	validarArgumentos (argc, argv );
	inicializarLogEntrenador(argv);

	t_entrenadorFisico * miEntrenador;
	miEntrenador = inicializarEstructurasDelEntrenador (argv[1], argv[2]);



	//*******************************************************
	//TODO: en la fc que ejecuta la logica del entrenador, agergar este llamado:
	inicializarTiemposDelEntrenador(&miEntrenador->misEstadisticas);
	//TODO: por ahora contaria solo el tiempo que estuvo boludiando con los sockets y otras cosas...

	sleep(2);

	//TODO: cuando se convirtio en maestro pokemon hay que llamar a este metodo
	mostrarTiempoTotalAventura (&miEntrenador->misEstadisticas);
	//TODO: y otros a implementar...
	//*******************************************************





	inicializarSocketEntrenador ();



	finalizarEntrenador(miEntrenador);
	return EXIT_SUCCESS;
}

void validarArgumentos ( int argc, char *argv[] )
{
	puts("PROCESO ENTRENADOR"); /* prints PROCESO ENTRENADOR */

	//arg[0] -> nombre del proceso ejecutable
	//arg[1] -> nombre del Entrenador
	//arg[2] -> donde esta ubicada la pokedex
	if ( argc !=3  ) /* debe recibir 3 argumentos */
	{
		printf("\tUso incorrecto..\n");
		printf("\tuso: %s nombreEntrenador dirPokeDex\n", argv[0]);
	  	printf("\tejemplo: %s %s %s\n", argv[0], "Ash", "/home/utnso/git/tp-2016-2c-Team-Rocket/PROC_ENTRENADOR/test_files");
		exit(EXIT_FAILURE);
	}




	if ( argv[1] == NULL || (strlen (argv[1]) < 2 ) )
	{
		printf("\tRevise los parametros.\n");
		exit(EXIT_FAILURE);
	}
	if ( argv[2] == NULL || (strlen (argv[2]) < 2 ) )
	{
		printf("\tRevise los parametros.\n");
		exit(EXIT_FAILURE);
	}


}

void inicializarLogEntrenador ( char *argv[] )		/*levanto el archivo para loggear*/
{


	//TODO: revisar que pasa si no existe el archivo de log y/o el directorio
	char* file =__ubicacionArchivoDeLog;
	char* pg_name = argv[0];

	//TODO: revisar que pasa si no esta creado el archivo :S
	myArchivoDeLog = log_create(file, pg_name, false, LOG_LEVEL_INFO);
	if (myArchivoDeLog != NULL)
	{
		puts("se creo OK el arch de log");
	}
	else
	{
		puts("NO se pudo crear el archivo de log");
		exit(EXIT_FAILURE);
	}
	log_info(myArchivoDeLog, "-------------------------------------------------------");

}


void inicializarSocketEntrenador (t_entrenadorFisico * nuevoEntrenador)
{
	//TODO: proximamente lo levantamos dinamico..
	log_info(myArchivoDeLog, "Inicializando la conexion por socket");
	int socket = connect_to("127.0.0.1", "6400");
	if (socket == -1) {
		puts("No se pudo conectar");
		//TODO: agregar las variables nombreMapa IP/Puerto al Log.
		log_info(myArchivoDeLog, "No se pudo inicializar la conexion por socket");
		finalizarEntrenador(nuevoEntrenador);
	} else {
		printf("socket: %d\n", socket);
		//TODO: agregar las variables nombreMapa IP/Puerto al Log.
		log_info(myArchivoDeLog, "Me conecte por socket, mi socket es: %d", socket);
		int simbolo = 0;
		t_data * info = pedirPaquete(99, sizeof(int), &simbolo);

		common_send(socket, info);

		while (1) {
			info = leer_paquete(socket);

			switch (info->header) {
			case ubicacionPokenest:
				//TODO: en info->data estara la posicion de la pokenest
				break;
			case capturastePokemon:
				//TODO: hacer lo que tengo que hacer
				break;
			case dameMejorPokemon:
				//TODO: enviar pokemon mas fuerte. realizar esta funcion
				break;
			case ganasteBatalla:
				//TODO: hacer lo que tenga que hacer
				break;
			case perdisteBatalla:
				//TODO: hacer lo que tenga que hacer
				break;
			case ubicacionMedallaMapa:
				//TODO: hacer lo que tenga que hacer
				break;
			}
		}
	}
	close(socket);
}


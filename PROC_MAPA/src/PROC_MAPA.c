/*
 ============================================================================
 Name        : PROC_MAPA.c
 Author      : TEAM ROCKET
 Version     :
 Copyright   : TEAM ROCKET
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//----------- Sector Include ---------------//

#include "lib/libGrafica.h"
#include "lib/libSockets.h"
#include "lib/libPlanificador.h"
#include "lib/libConfig.h"
#include <pthread.h>
//------------------------------------------//

//------------------------------------------//
/* ********************************************	*/
//----------- Sector Funciones -------------//

void validarArgumentos ( int argc, char *argv[] );
void inicializarLogMapa ();

//------------------------------------------//


//------------------------------------------//
/* ********************************************	*/
//----------- Sector Constantes -------------//

#define __ubicacionArchivoDeLog "/home/utnso/Escritorio/logMapa_teamRocket"
#define __nombreDePrograma "Mapa"


//------------------------------------------//





int main( int argc, char *argv[] )
{
	validarArgumentos (argc, argv );
	inicializarLogMapa();

	//Creo el hilo planificador
	pthread_t hiloPlanificador;
	pthread_create(&hiloPlanificador, NULL, ejecutarPlanificador, NULL);

	//TODO: Config - obtener directorio
	char *directorioConfig = malloc(100);
	strcat(directorioConfig,argv[2]);
	strcat(directorioConfig,"/Mapa");
	strcat(directorioConfig,argv[1]);
	strcat(directorioConfig,"/metadata");

	t_config * configMapa = newConfigType (directorioConfig);

	#warning("Consultar el nombre del proceso")
	//TODO: consultar el nombre de proceso, creo que es "mapa"
	#warning("Consultar si el nombre del mapa puede contener espacios")
	//TODO: consultar si el nombre de un mapa puede contener espacios....? En enunciado dice "Ciudad Paleta"
	#warning ("cuando hay un error abortivo hay que cerrar correctamente el gui")


	//TODO: no inicializar 2 procesos mapa con el mismo nombre en el sistema...

	t_mapa * mapa;

	nivel_gui_inicializar();
	mapa = inicializarEstructurasDelMapa (argv[1], argv[2]);

	dibujarMapa (mapa);

	//atiendo las conexiones de los entrenadores que se me conecten
	char * Puerto = configLeerString(configMapa,directorioConfig);

	//Atiendo las conexiones
	atenderConexiones(Puerto);

	finalizarGui(mapa);
	return EXIT_SUCCESS;	//enrealidad nunca ejecuta esta instruccion
}












void validarArgumentos ( int argc, char *argv[] )
{
	puts("PROCESO MAPA"); /* prints PROCESO MAPA */

	//arg[0] -> nombre del proceso ejecutable
	//arg[1] -> nombre del mapa
	//arg[2] -> donde esta ubicada la pokedex
	if ( argc !=3  ) /* debe recibir 3 argumentos */
	{
		printf("\tUso incorrecto..\n");
		printf("\tuso: %s nombreMapa dirPokeDex\n", argv[0]);
	  	printf("\tejemplo: %s %s %s\n", argv[0], "prueba1", "/home/utnso/git/tp-2016-2c-Team-Rocket/PROC_MAPA/test_files");
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

void inicializarLogMapa ()		/*levanto el archivo para loggear*/
{


	//TODO: revisar que pasa si no existe el archivo de log y/o el directorio
	char* file =__ubicacionArchivoDeLog;
	char* pg_name = __nombreDePrograma;

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




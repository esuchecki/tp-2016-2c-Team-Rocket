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
#include <so/libConfig.h>
#include <pthread.h>
#include "lib/libPlanificador.h"
#include "lib/conexiones.h"
#include "lib/deadlock.h"
#include <so/libSockets.h>
//------------------------------------------//

//------------------------------------------//
/* ********************************************	*/
//----------- Sector Funciones -------------//

void validarArgumentos ( int argc, char *argv[] );
void inicializarLogMapa ( char *argv[] );

//------------------------------------------//



int main( int argc, char *argv[] )
{
	validarArgumentos (argc, argv );
	inicializarLogMapa(argv);

	t_mapa * mapa;

	nivel_gui_inicializar();
	mapa = inicializarEstructurasDelMapa (argv[1], argv[2]);

	inicializarSenialesMapa (mapa, (void *) finalizarGui );

	pthread_t hiloPlanificador,hiloConexiones,hiloBloqueados,hiloDeadlock;
	inicializar_estructuras_planificador();
	pthread_create(&hiloPlanificador, NULL, ejecutarPlanificador, (void *)mapa);
	pthread_create(&hiloBloqueados,NULL,manejarEntrenadoresBloqueados,(void *)mapa);
	pthread_create(&hiloConexiones,NULL, (void *)atenderConexiones, (void *)mapa);
	pthread_create(&hiloDeadlock,NULL,deteccionDeadlock,(void *)mapa);

	//TODO: Lucas soy emi, creo que despues va a haber que darle otro tratamiento a los sockets para que no impriman msjs en pantalla (medio hacen lio con la gui).
	//dps mirate en libTeamRocket/so/metodos_a_implementar_sockets que me parece que estan todos los metodos que vamos a tener que llamar entre entrenador y mapa!.

	#warning ("cuando hay un error abortivo hay que cerrar correctamente el gui")


	//TODO: no inicializar 2 procesos mapa con el mismo nombre en el sistema...
	//Nota: esto se hace revisando que al inicializar las conexiones el puerto no este ocupado!!

	dibujarMapa (mapa);

	pthread_join(hiloPlanificador,NULL);
	pthread_join(hiloConexiones,NULL);
	pthread_join(hiloBloqueados,NULL);

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

void inicializarLogMapa ( char *argv[] )		/*levanto el archivo para loggear*/
{
	//TODO: revisar que pasa si no existe el archivo de log y/o el directorio

	//argv[0];	Program_NAME
	//argv[1];	Nombre del mapa
	//string_from_format(__ubicacionArchivoDeLogMapa, argv[1])	//Con esto le paso el nombre del archivo de log.

	//TODO: revisar que pasa si no esta creado el archivo :S
	myArchivoDeLog = log_create( string_from_format(__ubicacionArchivoDeLogMapa, argv[1]) , argv[0], false, LOG_LEVEL_DEBUG);


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




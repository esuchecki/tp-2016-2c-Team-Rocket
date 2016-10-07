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

/*
 *
 */
#include "lib/batallaPkmn.h"

//------------------------------------------//
/* ********************************************	*/
//----------- Sector Funciones -------------//
void validarArgumentos(int argc, char *argv[]);
void inicializarLogEntrenador(char *argv[]);
void inicializarSocketEntrenador(t_entrenadorFisico * nuevoEntrenador);
void accionDelMapaAnteSIGUSR1(t_entrenadorFisico * unEntrenador);
void accionDelMapaAnteSIGTERM(t_entrenadorFisico * unEntrenador);

void enviarMsjFantasmaParaMoverse(int socketConection, t_data * info);

//------------------------------------------//

int main(int argc, char *argv[]) {


	validarArgumentos(argc, argv);
	inicializarLogEntrenador(argv);

	t_entrenadorFisico * miEntrenador;
	miEntrenador = inicializarEstructurasDelEntrenador(argv[1], argv[2]);



	// *******************************************************
	//TODO: en la fc que ejecuta la logica del entrenador, agergar este llamado:
	inicializarTiemposDelEntrenador(&miEntrenador->misEstadisticas);
	//TODO: por ahora contaria solo el tiempo que estuvo boludiando con los sockets y otras cosas...

	sleep(2);

	//TODO: cuando se convirtio en maestro pokemon hay que llamar a este metodo
	mostrarTiempoTotalAventura(&miEntrenador->misEstadisticas);
	//TODO: y otros a implementar...
	// *******************************************************

	// *******************************************************
	//TODO: esto deberia ir en el while de movimientos de un entrenador..
	inicializarSenialesMapa(miEntrenador, (void *) finalizarEntrenador);

	sleep(2);	//para probar tirarle una senial.

	funcionesQueQuieroEjecutarSegunLaSenial(miEntrenador,
			(void *) finalizarEntrenador, (void*) accionDelMapaAnteSIGUSR1,
			(void*) accionDelMapaAnteSIGTERM);
	// *******************************************************

	inicializarSocketEntrenador(miEntrenador);









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
	//TODO: revisar que pasa si no existe el archivo de log y/o el directorio

	//argv[0];	Program_NAME
	//argv[1];	Nombre del entrenador
	//string_from_format(__ubicacionArchivoDeLogEntrenador, argv[1])	//Con esto le paso el nombre del archivo de log.

	//TODO: revisar que pasa si no esta creado el archivo :S
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

void inicializarSocketEntrenador(t_entrenadorFisico * nuevoEntrenador) {
	//TODO: proximamente lo levantamos dinamico..
	log_info(myArchivoDeLog, "Inicializando la conexion por socket");
	int socketConexion = connect_to("127.0.0.1", "8001");
	if (socketConexion == -1) {
		//TODO: agregar las variables nombreMapa IP/Puerto al Log.
		log_error(myArchivoDeLog,
				"No se pudo inicializar la conexion por socket");
		finalizarEntrenador(nuevoEntrenador);
	} else {
		printf("socket: %d\n", socketConexion);
		//TODO: agregar las variables nombreMapa IP/Puerto al Log.

		log_info(myArchivoDeLog, "Me conecte por socket, mi socket es: %d",
				socketConexion);

		t_data * info = pedirPaquete(99, sizeof(char),
				&nuevoEntrenador->metadata->simbolo);
		//TODO: ojo lucas, creo que aca cuando le haces free a info, perdemos el simbolo del entrenador.

		common_send(socketConexion, info);

		printf("envie paquete handshake\n");

		while (1) {
			info = leer_paquete(socketConexion);

			switch (info->header) {
			case otorgarTurno:
				;
				//TODO:Obtener de la lista de objetivos el identificador de
				//la pokenest, si la tiene la pokenest no la pide
				if (nuevoEntrenador->directorioPokeDex == NULL) {
					char identificadorPokenest = 'P';		//lo hardcodeo ahora
					t_data * paquete = pedirPaquete(peticionPokenest,
							sizeof(char), &identificadorPokenest);

					common_send(socketConexion, paquete);

					free(paquete);
				}else {

					//TODO: tengo la pokenest.. tengo que moverme

				}

				break;

			case ubicacionPokenest:
				//TODO: en info->data estara la posicion de la pokenest
				;
				int coordenadasEnX = 0;
				int coordenadasEnY = 0;
				memcpy(&coordenadasEnX, info->data, sizeof(int));
				memcpy(&coordenadasEnY, info->data + sizeof(int), sizeof(int));
				printf("Las coordenadas de la pokenest son: %d , %d\n",
						coordenadasEnX, coordenadasEnY);

				enviarMsjFantasmaParaMoverse(socketConexion, info);
				sleep(5);

				break;
			case capturastePokemon:
				//TODO: hacer lo que tengo que hacer
				break;
			case dameMejorPokemon:
				//TODO: enviar pokemon mas fuerte. realizar esta funcion

				//En caso de deadlock llamar a esta fc
				//elegirMejorPokemon(miEntrenador,(void*) finalizarEntrenador);

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
	close(socketConexion);
}

void accionDelMapaAnteSIGUSR1(t_entrenadorFisico * unEntrenador) {
	//TODO: ¿Tengo que validar que la metadata este inicializada?
	unEntrenador->metadata->vidas++;	//sumo una vida
}

void accionDelMapaAnteSIGTERM(t_entrenadorFisico * unEntrenador) {
	//TODO: ¿Tengo que validar que la metadata este inicializada?
	if (unEntrenador->metadata->vidas > 0)		//Minima cantidad de vidas es 0.
		unEntrenador->metadata->vidas--;	//resto una vida

	//TODO: Validar si me quede sin vidas me muero =(
}

void enviarMsjFantasmaParaMoverse(int socketConection, t_data * info) {
	puts("entro");
	//char * mensaje = info->data;
	int * mensaje = malloc(sizeof(int));
	//strcpy(mensaje, "6");
	//printf("%s\n", mensaje);
	*mensaje = 6;	//solo movete 1 pos.
	t_data * paquete = pedirPaquete(movimientoEntrenador, sizeof(int), mensaje);
	common_send(socketConection, paquete);
	free(mensaje);
	free(paquete);
}

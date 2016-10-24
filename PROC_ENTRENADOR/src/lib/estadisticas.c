/*
 * tiempo.c
 *
 *  Created on: 20/9/2016
 *      Author: utnso
 */


#include "estadisticas.h"


void inicializarTiemposDelEntrenador (t_stats * estadisticas)
{
	estadisticas->cant_muertes = 0;
	estadisticas->cant_deadlocks =0;
	estadisticas->cant_reintentos =0;
	estadisticas->bloqEnPokeNest_time = 0;
	estadisticas->start_time = time (NULL);
	//TODO: se puede validar que time devolvio -1 y prevenir problemas..
}



void mostrarEstadisticas(t_stats * estadisticas) {
	//1- Muestro el tiempo de juego:
	//Hago tiempo final - inicial
	double tiempoAventura = difftime(time(NULL), estadisticas->start_time);

	//imprimo solo la parte entera (0 decimales)-> Unidad en segundos..
	printf("Tiempo Total Aventura=  %.0f segundos\n", tiempoAventura);
	log_info(myArchivoDeLog, "Tiempo Total Aventura=  %.0f segundos",
			tiempoAventura);

	//2- Muestro tiempo Bloqueo en PokeNest:
	printf("Tiempo Total Bloqueo en PokeNest=  %i segundos\n",
			estadisticas->bloqEnPokeNest_time);
	log_info(myArchivoDeLog, "Tiempo Total Bloqueo en PokeNest=  %.0f segundos",
			estadisticas->bloqEnPokeNest_time);

	//3- Cuantas veces estuvo en deadLock
	printf("Cantidad de DeadLocks=  %i \n", estadisticas->cant_deadlocks);
	log_info(myArchivoDeLog, "Cantidad de DeadLocks=  %i ",
			estadisticas->cant_deadlocks);

	//4- Cuantas veces murio en el juego
	printf("Cantidad de Muertes=  %i \n", estadisticas->cant_muertes);
	log_info(myArchivoDeLog, "Cantidad de Muertes=  %i ",
			estadisticas->cant_muertes);
}



//TODO: hacer un metodo que muestre el "maestro Pokemon estadisticas" (creo que deberia ir en otro modulo..)
//TODO: aca si puede ir las funciones para calcular el tiempo de deadlock.

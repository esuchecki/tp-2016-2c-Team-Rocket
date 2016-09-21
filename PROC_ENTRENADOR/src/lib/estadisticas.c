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
	estadisticas->bloqEnPokeNest_time = 0;
	estadisticas->start_time = time (NULL);
	//TODO: se puede validar que time devolvio -1 y prevenir problemas..
}

void mostrarTiempoTotalAventura (t_stats * estadisticas)
{
	//Hago tiempo final - inicial
	double tiempoAventura = difftime( time (NULL), estadisticas->start_time);

	//imprimo solo la parte entera (0 decimales)-> Unidad en segundos..
	printf ("Tiempo Total Aventura=  %.0f segundos\n", tiempoAventura);
	log_info(myArchivoDeLog,"Tiempo Total Aventura=  %.0f segundos",  tiempoAventura);

}


//TODO: hacer un metodo que muestre el "maestro Pokemon estadisticas" (creo que deberia ir en otro modulo..)
//TODO: aca si puede ir las funciones para calcular el tiempo de deadlock.

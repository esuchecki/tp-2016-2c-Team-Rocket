/*
 * libPlanificador.c
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#include "libPlanificador.h"
#include <pthread.h>

void * ejecutarPlanificador(){

	return NULL;
}

void agregarAColaDeListos(t_entrenador *unEntrenador){
	list_add(colaListos,&unEntrenador);
}

t_entrenador * generarEntrenador(int i){
	t_entrenador *unEntrenador = malloc(sizeof(t_entrenador));
	unEntrenador->socket = i;
	return unEntrenador;
}

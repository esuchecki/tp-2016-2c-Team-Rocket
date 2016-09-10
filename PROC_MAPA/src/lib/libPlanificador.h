/*
 * libPlanificador.h
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#ifndef LIB_LIBPLANIFICADOR_H_
#define LIB_LIBPLANIFICADOR_H_

#include <commons/collections/list.h>

t_list *colaListos;
t_list *colaBloqueados;

typedef struct{
	int socket;
	//TODO: ver que otros datos poner aca
}t_entrenador;

void * ejecutarPlanificador();
void agregarAColaDeListos(t_entrenador *unEntrenador);
t_entrenador * generarEntrenador(int i);

#endif /* LIB_LIBPLANIFICADOR_H_ */

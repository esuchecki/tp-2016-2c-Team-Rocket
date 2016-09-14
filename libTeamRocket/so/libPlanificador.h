/*
 * libPlanificador.h
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#ifndef LIBPLANIFICADOR_H_
#define LIBPLANIFICADOR_H_

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



#endif /* LIBPLANIFICADOR_H_ */

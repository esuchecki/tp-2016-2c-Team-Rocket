/*
 * libPlanificador.c
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#include "../so/libPlanificador.h"

#include <pthread.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <string.h>
#include "../so/libSockets.h"

void desconectarEntrenador(int nroDesocket){
	bool seDesconecto(void * data){
		t_entrenador * alguno = data;
		return alguno->nroDesocket == nroDesocket ;
	}

	t_entrenador * entrenadorAEliminar = list_remove_by_condition(colaListos,seDesconecto);
	//manejar el tema de los recursos del entrenador
	free(entrenadorAEliminar);
}

t_entrenador * ejecutar_algoritmo(char * algoritmo){
	if(strcmp(algoritmo,"RR")==0){
		printf("la cola de listos tiene %d elementos\n",list_size(colaListos));
		t_entrenador * elProximo = list_remove(colaListos, 0);
		return elProximo;
		/*t_entrenador * elProximo = list_get(colaListos, 0);
		if(elProximo->instruccionesEjecutadas < elProximo->quantum){
			return elProximo;
		}else{
			list_add(colaListos,elProximo);
			elProximo = list_get(colaListos,1);
			return elProximo;
		}*/


	}else{
		//algoritmo entrenador mas cercano a pokedex
		t_entrenador *uno = malloc(sizeof(t_entrenador));
		uno->nroDesocket  = 0;
		return uno;
	}
}
void * ejecutarPlanificador(void * datos) {
	//t_mapa2 *mapa = datos;
	printf("iniciado hilo planificador\n");
	while (1) {
		sem_wait(&entrenador_listo);
		printf("Entre al planificador");
		pthread_mutex_lock(&mutex_algoritmo);

		t_entrenador *proximoEntrenador = ejecutar_algoritmo("RR");

		int null = 0;

		t_data *turno = pedirPaquete(1,sizeof(int),&null);

		common_send(proximoEntrenador->nroDesocket ,turno);
		printf("envie paquete al entrenador: %d\n",proximoEntrenador->nroDesocket );

		free(turno);

		pthread_mutex_unlock(&mutex_algoritmo);

	}

	return NULL;
}

void agregarAColaDeListos(t_entrenador *unEntrenador) {
	pthread_mutex_lock(&mutex_listos);
	list_add(colaListos, unEntrenador);
	pthread_mutex_unlock(&mutex_listos);
}

t_entrenador * generarEntrenador(int i,void * data) {
	t_entrenador *unEntrenador = malloc(sizeof(t_entrenador));

	unEntrenador->nroDesocket  = i;

	return unEntrenador;
}

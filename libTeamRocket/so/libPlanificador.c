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

void inicializar_estructuras_planificador(){
	sem_init(&entrenador_listo,1,0);
	sem_init(&entrenador_bloqueado,1,0);
	pthread_mutex_init(&mutex_listos,NULL);
	pthread_mutex_init(&mutex_algoritmo,NULL);
	pthread_mutex_init(&mutex_bloqueados,NULL);
	pthread_mutex_init(&mutex_ejecucion,NULL);
	colaListos = list_create();
	colaBloqueados = list_create();
	colaEjecucion = list_create();
}

void desconectarEntrenador(int nroDesocket) {
	bool seDesconecto(void * data) {
		t_entrenador * alguno = data;
		return alguno->nroDesocket == nroDesocket;
	}

	t_entrenador * entrenadorAEliminar = list_remove_by_condition(colaListos,
			seDesconecto);

	//TODO: manejar el tema de los recursos del entrenador

	free(entrenadorAEliminar);
}

t_entrenador * ejecutar_algoritmo(char * algoritmo) {
	if (strcmp(algoritmo, "RR") == 0) {

		t_entrenador * elProximo = list_remove(colaListos, 0);
		if (elProximo->instruccionesEjecutadas < elProximo->quantum) {
			return elProximo;
		} else {
			list_add(colaListos, elProximo);
			elProximo = list_remove(colaListos, 1);
			return elProximo;
		}

	} else {
		//algoritmo entrenador mas cercano a pokedex
		t_entrenador *uno = malloc(sizeof(t_entrenador));
		uno->nroDesocket = 0;
		return uno;
	}
}


void agregarAListaDeEjecucion(t_entrenador * entrenador_a_ejecutar) {
	pthread_mutex_lock(&mutex_ejecucion);
	list_add(colaEjecucion,entrenador_a_ejecutar);
	pthread_mutex_unlock(&mutex_ejecucion);

}

t_entrenador * removerDeListaDeEjecucion(int socket_entrenador) {

	bool encontrar_socket_entrenador(void * nodo) {
		return ((((t_entrenador*) nodo)->nroDesocket) == socket_entrenador);
	}

	pthread_mutex_lock(&mutex_ejecucion);

	t_entrenador * algunEntrenador = list_remove_by_condition(colaEjecucion,
			encontrar_socket_entrenador);

	pthread_mutex_unlock(&mutex_ejecucion);

	return algunEntrenador;

}

void * ejecutarPlanificador(void * datos) {
	//t_mapa *mapa = datos;

	while (1) {
		sem_wait(&entrenador_listo);

		pthread_mutex_lock(&mutex_algoritmo);

		t_entrenador *proximoEntrenador = ejecutar_algoritmo("RR");

		pthread_mutex_unlock(&mutex_algoritmo);

		agregarAListaDeEjecucion(proximoEntrenador);

		int null = 0;

		t_data *turno = pedirPaquete(1, sizeof(int), &null);

		common_send(proximoEntrenador->nroDesocket, turno);

		free(turno);


	}

	return NULL;
}

void agregarAColaDeBloqueados(t_entrenador * unEntrenador) {
	pthread_mutex_lock(&mutex_bloqueados);

	agregarAColaDeBloqueados(unEntrenador);

	pthread_mutex_unlock(&mutex_bloqueados);

}

t_entrenador * desbloquearEntrenador(){
	t_entrenador * entrenador = malloc(sizeof(t_entrenador));

	//TODO: ver como es el desbloqueo de un entrenador

	return entrenador;
}

void * manejarEntrenadoresBloqueados() {
	while (1) {
		sem_wait(&entrenador_bloqueado);

		pthread_mutex_lock(&mutex_bloqueados);

		t_entrenador * unEntrenador = desbloquearEntrenador();

		agregarAColaDeListos(unEntrenador);

		pthread_mutex_unlock(&mutex_bloqueados);

	}

	return NULL;
}

void agregarAColaDeListos(t_entrenador *unEntrenador) {
	pthread_mutex_lock(&mutex_listos);

	list_add(colaListos, unEntrenador);

	pthread_mutex_unlock(&mutex_listos);
}

t_entrenador * generarEntrenador(int i, void * data) {
	t_entrenador *unEntrenador = malloc(sizeof(t_entrenador));
	char * simboloEntrenador = data;

	unEntrenador->nroDesocket = i;
	unEntrenador->simbolo = *simboloEntrenador;

	return unEntrenador;
}

int obtenerCoordenadasPokenest(char identificadorPokenest){
	int coordenadas = 0;
	//TODO: buscar las coordenadas de la pokenest con identificador = identificadorPokenest

	return coordenadas;
}


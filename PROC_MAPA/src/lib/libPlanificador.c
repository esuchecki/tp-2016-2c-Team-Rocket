/*
 * libPlanificador.c
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#include "libPlanificador.h"
#include "estructurasMapa.h"

#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <commons/collections/list.h>
#include <string.h>
#include <so/libSockets.h>


void inicializar_estructuras_planificador() {
	sem_init(&entrenador_listo, 1, 0);
	sem_init(&entrenador_bloqueado, 1, 0);
	pthread_mutex_init(&mutex_listos, NULL);
	pthread_mutex_init(&mutex_algoritmo, NULL);
	pthread_mutex_init(&mutex_bloqueados, NULL);
	pthread_mutex_init(&mutex_ejecucion, NULL);
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

t_entrenador * ejecutar_algoritmo(char * algoritmo,int quantum) {
	if (strcmp(algoritmo, "RR") == 0) {

		t_entrenador * elProximo = list_get(colaListos, 0);

		if (elProximo->instruccionesEjecutadas < quantum) {

			return elProximo;

		} else {
			elProximo = list_remove(colaListos, 0);

			elProximo->instruccionesEjecutadas = 0;

			list_add(colaListos, elProximo);

			elProximo = list_get(colaListos, 0);

			return elProximo;
		}

	} else {
		//algoritmo entrenador mas cercano a pokedex
		t_entrenador *unEntrenador = malloc(sizeof(t_entrenador));

		unEntrenador = buscarDesconocedorPokenest();

		if(unEntrenador == NULL){

			t_entrenador  * unEntrenador = buscarCercaniaAPokenest();

			return unEntrenador;

		}else{
			return unEntrenador;
		}
	}
}

t_entrenador * buscarDesconocedorPokenest(){

	bool desconocePokenest(void *datos){
		t_entrenador * alguno = datos;
		return  alguno->distanciaAProximaPokenest == -1;
	}

	t_entrenador *entrenador  = list_find(colaListos,desconocePokenest);

	return entrenador;

}

t_entrenador * buscarCercaniaAPokenest(){

	bool cercaDePokenest(void *datos){
		t_entrenador *elMasCercano = datos;
		int i,menorDistancia;
		menorDistancia = 99;
		for(i=0;list_size(colaListos)> i;i++){
			t_entrenador * alguno = list_get(colaListos,i);
			if(alguno->distanciaAProximaPokenest < menorDistancia){
				menorDistancia = alguno->distanciaAProximaPokenest;
			}
		}
		return  elMasCercano->distanciaAProximaPokenest == menorDistancia;
	}

	t_entrenador *entrenador  = list_find(colaListos,cercaDePokenest);

	return entrenador;

}


void * ejecutarPlanificador(void * datos) {
	t_mapa *mapa = datos;

	while (1) {
		sem_wait(&entrenador_listo);

		pthread_mutex_lock(&mutex_algoritmo);

		t_entrenador *proximoEntrenador = ejecutar_algoritmo(mapa->metadata->algoritmo,mapa->metadata->quantum);

		pthread_mutex_unlock(&mutex_algoritmo);

		int null_data = 0;

		t_data *turno = pedirPaquete(pedirPokenest, sizeof(int), &null_data);

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

t_entrenador * desbloquearEntrenador() {
	t_entrenador * entrenador = malloc(sizeof(t_entrenador));

	//TODO: ver como es el desbloqueo de un entrenador

	entrenador->distanciaAProximaPokenest = -1;
	return entrenador;
}

void * manejarEntrenadoresBloqueados() {
	while (1) {
		sem_wait(&entrenador_bloqueado);

		pthread_mutex_lock(&mutex_bloqueados);

		t_entrenador * unEntrenador = desbloquearEntrenador();

		agregarAColaDeListos(unEntrenador);

		pthread_mutex_unlock(&mutex_bloqueados);

		sem_post(&entrenador_listo);

	}

	return NULL;
}

void agregarAColaDeListos(t_entrenador *unEntrenador) {
	pthread_mutex_lock(&mutex_listos);

	list_add(colaListos, unEntrenador);

	pthread_mutex_unlock(&mutex_listos);
}

void quitarDeColaDeListos(t_entrenador * entrenador){

	pthread_mutex_lock(&mutex_listos);

	bool mismoSocket(void * datos){

		t_entrenador * elegido = datos;

		return elegido->nroDesocket == entrenador->nroDesocket;

	}

	list_remove_by_condition(colaListos,mismoSocket);

	pthread_mutex_unlock(&mutex_listos);

}


t_entrenador * generarEntrenador(int i, void * data) {
	t_entrenador *unEntrenador = malloc(sizeof(t_entrenador));
	char * simboloEntrenador = data;

	unEntrenador->nroDesocket = i;
	unEntrenador->simbolo = *simboloEntrenador;
	unEntrenador->instruccionesEjecutadas = 0;
	unEntrenador->distanciaAProximaPokenest = -1;

	return unEntrenador;
}

int obtenerCoordenadasPokenest(char identificadorPokenest) {
	int coordenadas = 0;
//TODO: buscar las coordenadas de la pokenest con identificador = identificadorPokenest

	return coordenadas;
}

t_entrenador * reconocerEntrenadorSegunSocket(int nroDeSocket) {
	bool encontrar_socket_entrenador(void * nodo) {
		return ((((t_entrenador*) nodo)->nroDesocket) == nroDeSocket);
	}

	t_entrenador * entrenador = list_find(colaListos,
			encontrar_socket_entrenador);

	return entrenador;
}
void consumirQuantum( int i) {

	t_entrenador * entrenador  = reconocerEntrenadorSegunSocket(i);

	entrenador->instruccionesEjecutadas++;

	log_debug(myArchivoDeLog,"cantidad de instrucciones ejecutadas: %d\n",
			entrenador->instruccionesEjecutadas);

}


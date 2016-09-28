/*
 * libPlanificador.h
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#ifndef LIBPLANIFICADOR_H_
#define LIBPLANIFICADOR_H_

#include <commons/collections/list.h>
#include <semaphore.h>
#include <so/libSockets.h>


t_list *colaListos;
t_list *colaBloqueados;
t_list *colaEjecucion;
sem_t entrenador_listo,entrenador_bloqueado;
pthread_mutex_t mutex_listos,mutex_algoritmo,mutex_bloqueados,mutex_ejecucion;

typedef struct{
	int nroDesocket;
	char simbolo;
	int instruccionesEjecutadas;
	int distanciaAProximaPokenest;
	//TODO: ver que otros datos poner aca
}t_entrenador;

void inicializar_estructuras_planificador();

void * ejecutarPlanificador(void * data);

void agregarAColaDeListos(t_entrenador *unEntrenador);

void * manejarEntrenadoresBloqueados();

t_entrenador * desbloquearEntrenador();

void agregarAColaDeBloqueados(t_entrenador *unEntrenador);

t_entrenador * generarEntrenador(int i,void * data);

t_entrenador * ejecutar_algoritmo(char * algoritmo,int quantum);

void desconectarEntrenador(int nroDesocket );

int obtenerCoordenadasPokenest(char identificadorPokenest);

void consumirQuantum(int numeroDeSocket);

t_entrenador * reconocerEntrenadorSegunSocket(int nroDeSocket);

t_entrenador * buscarDesconocedorPokenest();

t_entrenador * buscarCercaniaAPokenest();

void quitarDeColaDeListos(t_entrenador * entrenador);

void setearDistanciaPokenest(int nroDeSocket);

#endif /* LIBPLANIFICADOR_H_ */

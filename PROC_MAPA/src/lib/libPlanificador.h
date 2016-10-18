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
#include <pkmn/factory.h>
#include "estructurasMapa.h"


t_list *colaListos;
t_list *colaBloqueados;
t_list *colaFinalizados;
sem_t entrenador_listo,entrenador_bloqueado,mapa_libre;
pthread_mutex_t mutex_listos,mutex_algoritmo,mutex_bloqueados,mutex_ejecucion;



void inicializar_estructuras_planificador();

t_entrenador * generarEntrenador(int i,void * data);

void * ejecutarPlanificador(void * data);

t_entrenador * ejecutar_algoritmo(char * algoritmo,int quantum);

void agregarAColaDeListos(t_entrenador *unEntrenador);

void quitarDeColaDeListos(t_entrenador * entrenador);

void * manejarEntrenadoresBloqueados(void * datos);

void asignarPokemonAEntrenador(t_mapa *mapa, t_entrenador * entrenador) ;

void desbloquearEntrenador(t_mapa * mapa);

void agregarAColaDeBloqueados(t_entrenador *unEntrenador);

void quitarDeColaDeBloqueados(t_entrenador *entrenador);

void desconectarEntrenador(int nroDesocket ,t_mapa * mapa,fd_set sockets_activos,int socketMasGrande);

void liberarRecursos(t_entrenador *entrenador);

void agregarAColaDeFinalizados(t_entrenador *entrenadorAEliminar);

int obtenerCoordenadasPokenest(char identificadorPokenest);

int consumirQuantum(int numeroDeSocket,int quantum);

t_entrenador * reconocerEntrenadorSegunSocket(int nroDeSocket);

t_entrenador * buscarDesconocedorPokenest();

t_entrenador * buscarCercaniaAPokenest();

void setearDistanciaPokenest(int nroDeSocket, t_mapa * self,char  pokenest);

void loguearColasDePlanificacion(t_list *lista, char *nombreLista);

#endif /* LIBPLANIFICADOR_H_ */

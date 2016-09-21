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


t_list *colaListos;
t_list *colaBloqueados;
sem_t entrenador_listo;
pthread_mutex_t mutex_listos,mutex_algoritmo;

typedef struct{
	int nroDesocket;
	char simbolo;
	int quantum;
	int instruccionesEjecutadas;
	//TODO: ver que otros datos poner aca
}t_entrenador;

typedef struct
{
	int tiempoChequeadoDeadlock;
	char * batalla;
	char * algoritmo;
	int quantum;
	int retardo;
	char * ip;
	char * puerto;
} t_metadataMapa2 ;

typedef struct
{
     char * nombre;
     char * directorioPokeDex;
     t_metadataMapa2 * metadata;
     t_list* items;
     //t_pokeNest pokeNest;		//a futuro esto deberia ser un array o una lista enrealidad...
} t_mapa2 ;

void * ejecutarPlanificador();
void agregarAColaDeListos(t_entrenador *unEntrenador);
t_entrenador * generarEntrenador(int i,void * data);
t_entrenador * ejecutar_algoritmo(char * algoritmo);
void desconectarEntrenador(int nroDesocket );


#endif /* LIBPLANIFICADOR_H_ */

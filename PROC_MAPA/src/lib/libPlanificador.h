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
	int quantum;
	int instruccionesEjecutadas;
	//TODO: ver que otros datos poner aca
}t_entrenador;

void inicializar_estructuras_planificador();

void * ejecutarPlanificador(void * data);

void agregarAColaDeListos(t_entrenador *unEntrenador);

void * manejarEntrenadoresBloqueados();

t_entrenador * desbloquearEntrenador();

void agregarAColaDeBloqueados(t_entrenador *unEntrenador);

void agregarAListaDeEjecucion(t_entrenador * entrenador_a_ejecutar) ;

t_entrenador * removerDeListaDeEjecucion(int socket_entrenador);

t_entrenador * generarEntrenador(int i,void * data);

t_entrenador * ejecutar_algoritmo(char * algoritmo);

void desconectarEntrenador(int nroDesocket );

int obtenerCoordenadasPokenest(char identificadorPokenest);

void detectarDesconexion(t_data * paquete,int socket_recepcion,fd_set sockets_activos);

void consumirQuantum(int numeroDeSocket);








int atenderConexiones(void * data);

void handshake(int socket_nueva_conexion, fd_set sockets_activos);

void atenderConexion(int i, fd_set sockets_activos);

#endif /* LIBPLANIFICADOR_H_ */

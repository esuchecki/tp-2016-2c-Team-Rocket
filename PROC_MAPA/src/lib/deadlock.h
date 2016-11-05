/*
 * deadlock.h
 *
 *  Created on: 5/10/2016
 *      Author: utnso
 */

#ifndef LIB_DEADLOCK_H_
#define LIB_DEADLOCK_H_

#include "libPlanificador.h"
#include "estructurasMapa.h"
#include "batallaPkmn.h"
#include "conexiones.h"

t_list * parcial;

void cargarEstructurasDeadlock(int recursosTotales[], int recursosDisponibles[],
		int **asignados, int **requeridos, int cantidadPokenest,
		int numeroDeProcesos, t_mapa * mapa);

int calcularRecursosAsignados(t_entrenador *entrenadorBloqueado,
		t_pokeNest *pokenest);

t_list * detectarDeadlock(t_mapa * datosMapa);

void * deteccionDeadlock(void * datos);

void peticionesDePokemones(t_list * listaDeadlock) ;

void loguearListaDeadlock(t_list *listaDeadlock);

int encontrarEntrenadorQueRetieneRecurso(t_entrenador *entrenadorAux,
		t_list *listaDeadlock,t_mapa * mapa,t_entrenador *entrenador);

void rearmarListaDeadlock(t_list * listaDeadlock,t_mapa * mapa);

void borrarDeListaPrincipal(t_list *listaDeadlock);

void removerDeListaSecundaria();

#endif /* LIB_DEADLOCK_H_ */

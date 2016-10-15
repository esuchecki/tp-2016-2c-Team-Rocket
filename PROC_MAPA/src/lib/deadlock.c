/*
 * deadlock.c
 *
 *  Created on: 5/10/2016
 *      Author: utnso
 */
#include "deadlock.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int calcularRecursosAsignados(t_entrenador *entrenadorBloqueado,
		t_pokeNest *pokenest) {
	bool entrenadorAtrapoPokemon(void * nodo) {
		return ((t_pokemonEnPokeNest *) nodo)->capturadoPorEntrenador
				== entrenadorBloqueado->simbolo;
	}

	return list_count_satisfying(pokenest->pokemones, entrenadorAtrapoPokemon);

}

void cargarEstructurasDeadlock(int recursosTotales[], int recursosDisponibles[],
		int **asignados, int **requeridos, int cantidadPokenest,
		int numeroDeProcesos, t_mapa * mapa) {

	int i, k, j;

	for (i = 0; i < list_size(mapa->pokeNest); i++) {
		t_pokeNest * pokeNest = list_get(mapa->pokeNest, i);
		recursosTotales[i] = list_size(pokeNest->pokemones);
	}

	for (k = 0; cantidadPokenest > k; k++) {
		int t;
		int recursosAsignados = 0;
		for (t = 0; t < numeroDeProcesos; t++) {
			t_pokeNest * pokenest = list_get(mapa->pokeNest, k);

			t_entrenador * entrenadorBloqueado = list_get(colaBloqueados, t);

			int cantidadPokemonesEntrenador = calcularRecursosAsignados(
					entrenadorBloqueado, pokenest);

			recursosAsignados = recursosAsignados + cantidadPokemonesEntrenador;
		}
		recursosDisponibles[k] = recursosTotales[k] - recursosAsignados;
	}

	for (i = 0; i < cantidadPokenest; i++) {
		for (j = 0; j < numeroDeProcesos; j++) {

			t_pokeNest * pokenest = list_get(mapa->pokeNest, i);

			t_entrenador * entrenadorBloqueado = list_get(colaBloqueados, j);

			int cantidadPokemonesEntrenador = calcularRecursosAsignados(
					entrenadorBloqueado, pokenest);

			asignados[j][i] = cantidadPokemonesEntrenador;
		}
	}

	for (i = 0; numeroDeProcesos > i; i++) {
		for (j = 0; j < cantidadPokenest; j++) {
			t_entrenador *entrenador = list_get(colaBloqueados, i);

			t_pokeNest * pokenest = list_get(mapa->pokeNest, j);
			if (entrenador->pokemonSolicitado == pokenest->identificador) {
				requeridos[i][j] = 1;
			} else {
				requeridos[i][j] = 0;
			}

		}
	}

}

t_list * detectarDeadlock(t_mapa * datosMapa) {

	t_mapa * mapa = datosMapa;

	t_list *listaDeadlock = list_create();

	int numeroDeProcesos = list_size(colaBloqueados);
	int cantidadPokenest = list_size(mapa->pokeNest);

	int recursosTotales[cantidadPokenest];
	int recursosDisponibles[cantidadPokenest];

	int **asignados = malloc(numeroDeProcesos * sizeof(int *));
	int i;
	for (i = 0; i < numeroDeProcesos; i++) {
		asignados[i] = malloc(sizeof(int) * cantidadPokenest);
	}

	int **requeridos = malloc(sizeof(int*) * numeroDeProcesos);
	for (i = 0; i < numeroDeProcesos; i++) {
		requeridos[i] = (int*) malloc(sizeof(int) * cantidadPokenest);
	}

	cargarEstructurasDeadlock(recursosTotales, recursosDisponibles, asignados,
			requeridos, cantidadPokenest, numeroDeProcesos, mapa);

	int work[cantidadPokenest];

	bool Finish[numeroDeProcesos];

	int j;

	for (i = 0; i < cantidadPokenest; i++) {
		work[i] = recursosDisponibles[i];
	}

	for (i = 0; i < numeroDeProcesos; i++) {
		for (j = 0; j < cantidadPokenest; j++) {
			if (asignados[i][j] != 0) {
				Finish[i] = false;
				goto vuelta;
			} else
				Finish[i] = true;
		}
		vuelta: ;
	}

	for (i = 0; i < numeroDeProcesos; i++) {
		if (Finish[i] == false) {
			t_list * vectorBooleano = list_create();
			for (j = 0; j < cantidadPokenest; j++) {
				if (requeridos[i][j] <= work[i]) {

					list_add(vectorBooleano, (void *) true);

				}
			}
			bool sonVerdaderos(void *data) {
				bool prueba = data;
				return prueba == true;
			}
			if (list_size(vectorBooleano) == cantidadPokenest
					&& list_all_satisfy(vectorBooleano, sonVerdaderos)) {
				work[i] = work[i] + asignados[i][j];
				Finish[i] = true;
			}

		}
	}

	for (i = 0; i < cantidadPokenest; i++) {
		if (Finish[i] == false) {
			t_entrenador *entrenador = list_get(colaBloqueados, i);
			list_add(listaDeadlock, entrenador);
		}
	}

	/*	for (i = 0; i < cantidadPokenest; i++) {
	 free(asignados[i]);
	 }
	 free(asignados);

	 for (i = 0; i < cantidadPokenest; i++) {
	 free(requeridos[i]);
	 }
	 free(requeridos);
	 */
	if (list_size(listaDeadlock) < 2) {
		return NULL;
	} else {
		return listaDeadlock;
	}

}

void * deteccionDeadlock(void * datos) {

	t_mapa * mapa = datos;

	if (mapa->metadata->tiempoChequeadoDeadlock == 0) {

		pthread_exit(NULL);

	} else {
		while (1) {
			usleep(mapa->metadata->tiempoChequeadoDeadlock);

			t_list * listaDeadlock = detectarDeadlock(mapa);

			if (listaDeadlock == NULL) {

				//No Hay Deadlock, no hago nada

			} else {

				peticionesDePokemones(listaDeadlock);

				if (list_size(listaDeadlock) == 2) {
					t_entrenador * entrenador1 = list_get(listaDeadlock, 0);
					t_entrenador * entrenador2 = list_get(listaDeadlock, 1);

					//t_entrenador * loser = malloc(sizeof(t_entrenador));
					t_entrenador * loser;
					loser = batallarDosEntrenadores(
							entrenador1,
							entrenador2);
					//TODO: buscar el dueño del pokemon loser y eliminarlo
					//Listo el dueño, falta eliminarlo

				} else {
					t_entrenador * loser = batallarListaDePkmn(listaDeadlock);
					//TODO: buscar el dueño del pokemon loser y eliminarlo
					//Listo el dueño, falta eliminarlo
				}
			}
		}
	}

	return NULL;
}

void peticionesDePokemones(t_list * listaDeadlock) {
	int i;
	for (i = 0; i < list_size(listaDeadlock); i++) {
		t_entrenador * entrenador = list_get(listaDeadlock, i);

		t_data * paquete = pedirPaquete(dameMejorPokemon, 4, &i);

		common_send(entrenador->nroDesocket, paquete);

		paquete = leer_paquete(entrenador->nroDesocket);
		if (paquete->header == mejorPokemon) {
			entrenador->mejorPokemon = malloc(paquete->tamanio);
			entrenador->mejorPokemon = paquete->data;
		} else {
			log_info(myArchivoDeLog, "Error en la recepcion del mejor pokemon");
		}
	}

}

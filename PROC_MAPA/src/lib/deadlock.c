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

/*
 * @NAME: esUnicoEsteEntrenador
 * @DESC: Se utiliza para validar que el algoritmo de deteccion de deadlock, no me agrege 2 veces al mismo entrenador en la lista de deadlock.
 */
bool esUnicoEsteEntrenador(t_list* listaDeadlock, char simboloEntrenador);

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
	log_info(myArchivoDeLog,"--ESTRUCTURAS HILO DEADLOCK--");
	log_info(myArchivoDeLog,"MATRIZ RECURSOS TOTALES");
	for(i=0;i<list_size(mapa->pokeNest); i++){
		t_pokeNest * pokeNest = list_get(mapa->pokeNest, i);
		recursosTotales[i] = list_size(pokeNest->pokemones);
		log_info(myArchivoDeLog,"%c -> %d",pokeNest->identificador,recursosTotales[i]);
	}
	log_info(myArchivoDeLog,"MATRIZ RECURSOS DISPONIBLES");
	for(i=0;i<cantidadPokenest;i++){
		t_pokeNest * pokeNest = list_get(mapa->pokeNest, i);
		log_info(myArchivoDeLog,"%c -> %d",pokeNest->identificador,recursosDisponibles[i]);
	}
	log_info(myArchivoDeLog,"MATRIZ DE RECURSOS ASIGNADOS");
	for (i = 0; i < cantidadPokenest; i++) {
			for (j = 0; j < numeroDeProcesos; j++) {
			t_pokeNest * pokenest = list_get(mapa->pokeNest, i);
			t_entrenador * entrenadorBloqueado = list_get(colaBloqueados, j);
			log_info(myArchivoDeLog,"%c -> %c -> %d",entrenadorBloqueado->simbolo,pokenest->identificador,asignados[j][i]);
		}
	}
	log_info(myArchivoDeLog,"MATRIZ DE RECURSOS REQUERIDOS");
	for (i = 0; i < cantidadPokenest; i++) {
		for (j = 0; j < numeroDeProcesos; j++) {
			t_pokeNest * pokenest = list_get(mapa->pokeNest, i);
			t_entrenador * entrenadorBloqueado = list_get(colaBloqueados, j);
			log_info(myArchivoDeLog,"%c -> %c -> %d",entrenadorBloqueado->simbolo,pokenest->identificador,requeridos[j][i]);
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

			if (entrenador != NULL) {
				//le pido al algoritmo de deadlock que si el entrenador estaba en la cola, no lo vuelva a agregar.
				if (esUnicoEsteEntrenador(listaDeadlock, entrenador->simbolo))
					list_add(listaDeadlock, entrenador);
			}
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
		list_destroy(listaDeadlock);
		return NULL;
	} else {
		return listaDeadlock;
	}

}

bool esUnicoEsteEntrenador(t_list* listaDeadlock, char simboloEntrenador) {
	//defino la condicion (que no exista el item)
	bool _yaExistiaEsteId(void * nodo) {
		if (((t_entrenador *) nodo)->simbolo == simboloEntrenador) //sabemos que idDelItem es un char.
				{
			return 1;	//Ese id ya existe
		}
		return 0;
	}

	//devuelvo 0-false si NO es unico.
	if (list_count_satisfying(listaDeadlock, (void *) _yaExistiaEsteId) > 0)
		return 0;

	//devuelvo 1-true xq es unico.
	return 1;
}

void * deteccionDeadlock(void * datos) {

	t_mapa * mapa = datos;

	while (1) {
		sleepInMiliSegundos(mapa->metadata->tiempoChequeadoDeadlock);

		t_list * listaDeadlock = detectarDeadlock(mapa);
		/*
		 t_list * listaDeadlock = list_create();
		 sleep(40);
		 list_add(listaDeadlock, (t_entrenador *) list_get(colaBloqueados,0));
		 list_add(listaDeadlock, (t_entrenador *) list_get(colaBloqueados,1));
		 log_info(myArchivoDeLog, "---------%s", string_itoa(list_size(listaDeadlock)));*/
		if ((mapa->metadata->batalla[0] == '1') && (listaDeadlock != NULL)) {
			loguearListaDeadlock(listaDeadlock);
			parcial = list_create();
			while (list_size(listaDeadlock) != 0) {
				log_info(myArchivoDeLog, "Resolucion de deadlock por batalla");

				rearmarListaDeadlock(listaDeadlock, mapa);

				peticionesDePokemones(parcial);

				t_entrenador * loser = batallarListaDePkmn(parcial);

				if (loser == NULL) {
					//TODO: si entro aca se mandaron algun moco...que hago!??
				} else {
					int null_data = 0;
					t_data *paquetePerdisteBatalla = pedirPaquete(
							perdisteBatalla, sizeof(int), &null_data);
					common_send(loser->nroDesocket, paquetePerdisteBatalla);
					free(paquetePerdisteBatalla);


					desconectarEntrenador(loser->nroDesocket, mapa,
							sockets_activos, socketMasGrande);

					borrarDeListaPrincipal(listaDeadlock);

					removerDeListaSecundaria();

					loguearListaDeadlock(listaDeadlock);
					//		rearmarListaDeadlock(listaDeadlock, mapa);
					//		list_destroy(listaDeadlock);
					//		loguearListaDeadlock(parcial);

				}
			}
			list_destroy(listaDeadlock);
		}
	}
	return NULL;
}

void peticionesDePokemones(t_list * listaDeadlock) {
	int i;
	for (i = 0; i < list_size(listaDeadlock); i++) {
		t_entrenador * entrenador = list_get(listaDeadlock, i);

		t_data * paquete = pedirPaquete(dameMejorPokemon, sizeof(int), &i);

		common_send(entrenador->nroDesocket, paquete);

		paquete = leer_paquete(entrenador->nroDesocket);
		if (paquete->header == mejorPokemon) {

			entrenador->mejorPokemon = malloc(sizeof(t_pokemon));

			char * auxSpecies = malloc(50 * sizeof(char));
			void * buffer = malloc(sizeof(t_pokemon) + 50 * sizeof(char));
			buffer = paquete->data;

			memcpy(entrenador->mejorPokemon, buffer, sizeof(t_pokemon));
			memcpy(auxSpecies, buffer + sizeof(t_pokemon), 50 * sizeof(char));

			entrenador->mejorPokemon->species = auxSpecies;
		} else {
			log_info(myArchivoDeLog, "Error en la recepcion del mejor pokemon");
		}
	}

}

void loguearListaDeadlock(t_list *listaDeadlock) {
	log_info(myArchivoDeLog, "Cantidad lista deadlock = %d",
			list_size(listaDeadlock));
	if (list_size(listaDeadlock) > 2) {
		int i;
		log_info(myArchivoDeLog, "LISTA - DEADLOCK:");
		for (i = 0; i < list_size(listaDeadlock); i++) {
			t_entrenador * entrenador = list_get(listaDeadlock, i);
			log_info(myArchivoDeLog, "Entrenador - %c", entrenador->simbolo);
		}
	}
}

void rearmarListaDeadlock(t_list * listaDeadlock, t_mapa *mapa) {
	t_entrenador * entrenador = list_get(listaDeadlock, 0);
	t_entrenador * entrenadorAux = malloc(sizeof(t_entrenador));

	entrenadorAux = entrenador;

	list_add(parcial, entrenadorAux);

	int resultado = encontrarEntrenadorQueRetieneRecurso(entrenadorAux,
			listaDeadlock, mapa, entrenador);
	if (resultado == 0) {
		log_error(myArchivoDeLog, "No encontre la espera circular");
	} else {
		log_info(myArchivoDeLog, "ENCONTRE LA ESPERA CIRCULAR");
	}
}

void borrarDeListaPrincipal(t_list *listaDeadlock) {
	int i;
	for (i = 0; i < list_size(parcial); i++) {
		t_entrenador * entrenador = list_get(parcial, i);
		bool mismoEntrenador(void *nodo) {
			return ((t_entrenador *) nodo)->simbolo == entrenador->simbolo;
		}
		list_remove_by_condition(listaDeadlock, mismoEntrenador);

	}
}

int encontrarEntrenadorQueRetieneRecurso(t_entrenador *entrenadorAux,
		t_list *listaDeadlock, t_mapa * mapa, t_entrenador *entrenador) {

	bool segunPokemonEntrenador(void * nodo) {
		return entrenadorAux->pokemonSolicitado
				== ((t_pokeNest *) nodo)->identificador;
	}

	t_pokeNest *pokenest;
	seguir: pokenest = list_find(mapa->pokeNest, segunPokemonEntrenador);

	int i;
	for (i = 0; i < list_size(pokenest->pokemones); i++) {
		t_pokemonEnPokeNest * pokemon = list_get(pokenest->pokemones, i);
		//t_entrenador * alguno = list_get(listaDeadlock, i);
		bool mismoSimbolo(void *nodo) {
			return ((t_entrenador*) nodo)->simbolo
					== pokemon->capturadoPorEntrenador;
		}
		t_entrenador * alguno = list_find(listaDeadlock, mismoSimbolo);
		entrenadorAux = alguno;
		if (entrenador->simbolo == entrenadorAux->simbolo) {
			return 1;
			//return alguno;
		} else {
			list_add(parcial, entrenadorAux);
			//encontrarEntrenadorQueRetieneRecurso(entrenadorAux,listaDeadlock,mapa,entrenador);
			entrenadorAux = alguno;
			goto seguir;

		}
	}
	return 0;
}

void removerDeListaSecundaria(){
	seguir:
	;
	int i;
	for(i = 0; i < list_size(parcial) ; i++){
		list_remove(parcial,i);
		goto seguir;
	}
}

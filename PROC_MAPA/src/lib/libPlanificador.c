/*
 * libPlanificador.c
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#include "libPlanificador.h"
#include "estructurasMapa.h"
#include "libGrafica.h"
#include "conexiones.h"
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
#include <time.h>

void inicializar_estructuras_planificador() {
	sem_init(&entrenador_listo, 1, 0);
	sem_init(&entrenador_bloqueado, 1, 0);
	sem_init(&mapa_libre, 1, 0);
	sem_init(&semaforoGraficar, 1, 0);

	sem_post(&mapa_libre);
	sem_post(&semaforoGraficar);
	pthread_mutex_init(&mutex_listos, NULL);
	pthread_mutex_init(&mutex_algoritmo, NULL);
	pthread_mutex_init(&mutex_bloqueados, NULL);

	colaListos = list_create();
	colaBloqueados = list_create();
	colaFinalizados = list_create();

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

void * ejecutarPlanificador(void * datos) {
	t_mapa *mapa = datos;

	while (1) {

		//Si no hay semaforo entonces cheuqeo la actualizaciones de seniales.
		volver01:
		if (sem_trywait(&mapa_libre) != 0)	//No fue exitoso
		{
			funcionesQueQuieroEjecutarSegunLaSenial(mapa, (void* ) &accionDelMapaAnteSIGUSR2 );
			sleepInMiliSegundos(mapa->metadata->retardo);
			goto volver01;
		}
		//sem_wait(&mapa_libre);
		//sem_wait(&entrenador_listo);
		volver02:
		if (sem_trywait(&entrenador_listo) != 0)	//No fue exitoso
		{
			funcionesQueQuieroEjecutarSegunLaSenial(mapa, (void* ) &accionDelMapaAnteSIGUSR2 );
			sleepInMiliSegundos(mapa->metadata->retardo);
			goto volver02;
		}

		pthread_mutex_lock(&mutex_algoritmo);

		//chequeo el evento de una senial.
		funcionesQueQuieroEjecutarSegunLaSenial(mapa, (void* ) &accionDelMapaAnteSIGUSR2 );

		t_entrenador *entrenadorElegido = ejecutar_algoritmo(mapa->metadata->algoritmo, mapa->metadata->quantum);
		pthread_mutex_unlock(&mutex_algoritmo);

		log_info(myArchivoDeLog,"ESCUCHO AL ENTRENADOR-SOCKET %d",entrenadorElegido->nroDesocket);
		atenderConexion(entrenadorElegido->nroDesocket, mapa,sockets_activos);
	}

	return NULL;
}

t_entrenador * ejecutar_algoritmo(char * algoritmo, int quantum) {
	if (strcmp(algoritmo, "RR") == 0) {

		t_entrenador * elProximo = list_get(colaListos, 0);
		log_debug(myArchivoDeLog, "Algoritmo elige a: %c", elProximo->simbolo);
		return elProximo;

	} else {
		//algoritmo entrenador mas cercano a pokedex
		t_entrenador *unEntrenador = malloc(sizeof(t_entrenador));

		unEntrenador = buscarDesconocedorPokenest();

		if (unEntrenador == NULL) {

			t_entrenador * unEntrenador = buscarCercaniaAPokenest();

			return unEntrenador;

		} else {
			return unEntrenador;
		}
	}
}

void agregarAColaDeListos(t_entrenador *unEntrenador) {
	pthread_mutex_lock(&mutex_listos);

	list_add(colaListos, unEntrenador);

	log_info(myArchivoDeLog, "Se Mueve a cola de listos al entrenador: %c",
			unEntrenador->simbolo);

	loguearColasDePlanificacion(colaListos, "Listos");
	loguearColasDePlanificacion(colaBloqueados, "Bloqueados");
	loguearColasDePlanificacion(colaFinalizados, "Finalizados");

	pthread_mutex_unlock(&mutex_listos);
}

void quitarDeColaDeListos(t_entrenador * entrenador) {

	pthread_mutex_lock(&mutex_listos);

	bool mismoSocket(void * datos) {

		t_entrenador * elegido = datos;

		return elegido->nroDesocket == entrenador->nroDesocket;

	}

	list_remove_by_condition(colaListos, &mismoSocket);

	log_info(myArchivoDeLog, "Se quita de la cola de Listos al entrenador: %c",
			entrenador->simbolo);

	loguearColasDePlanificacion(colaListos, "Listos");
	loguearColasDePlanificacion(colaBloqueados, "Bloqueados");
	loguearColasDePlanificacion(colaFinalizados, "Finalizados");

	pthread_mutex_unlock(&mutex_listos);

}

void * manejarEntrenadoresBloqueados(void * datos) {
	t_mapa * mapa = datos;
	while (1) {
		sem_wait(&entrenador_bloqueado);

		desbloquearEntrenador(mapa);

	}

	return NULL;
}

void asignarPokemonAEntrenador(t_mapa *mapa, t_entrenador * entrenador) {
	bool igualIdentificador(void * datos) {
		t_pokeNest * poke = datos;
		return poke->identificador == entrenador->pokemonSolicitado;
	}

	bool noEstaAtrapado(void * datos) {
		t_pokemonEnPokeNest * pokemon = datos;
		return pokemon->capturadoPorEntrenador == '\0';
	}

	t_pokeNest * pokeNest = list_find(mapa->pokeNest, igualIdentificador);
	t_pokemonEnPokeNest * pokemon = list_find(pokeNest->pokemones,
			noEstaAtrapado);
	if (pokemon != NULL) {

		pokemon->capturadoPorEntrenador = entrenador->simbolo;
		//restar un recurso a ese pokemon.
		ITEM_NIVEL * resultado = encontrameEsteIdEnLaLista(mapa,
				entrenador->pokemonSolicitado);
		resultado->quantity--;
		entrenador->distanciaAProximaPokenest = -1;
		entrenador->instruccionesEjecutadas = 0;

		log_debug(myArchivoDeLog, "le asigne al entrenador %c, el pokemon: %c",
				entrenador->simbolo, pokeNest->identificador);
		log_debug(myArchivoDeLog, "nombre del archivo: %s",
				pokemon->pokemonNNNdat);
		//char * directorioPkmn;
		//directorioPkmn = malloc ( (sizeof (char)) * PATH_MAX +1);
		//sprintf(directorioPkmn, "/%s/%s/%s/%s/%s", mapa->directorioPokeDex, __ubicacionMapas, mapa->nombre , __ubicacionDirPokenest, pokemon->pokemonNNNdat);
//TODO: me falto pasarle el nombre de la pokenest antes del pokemonNNN.dat

		//TODO: pasarle path con la ubicacion del pokemon!
		//int null_data = 0;
		//t_data *capturaPkmn = pedirPaquete(capturastePokemon, (sizeof (char)) * PATH_MAX +1 , directorioPkmn);
		t_data *capturaPkmn = pedirPaquete(capturastePokemon,
				(sizeof(char)) * PATH_MAX + 1, pokemon->pokemonNNNdat);
		common_send(entrenador->nroDesocket, capturaPkmn);
		log_debug(myArchivoDeLog, "%s", pokemon->pokemonNNNdat);
		//free(directorioPkmn);
		free(capturaPkmn);

		quitarDeColaDeBloqueados(entrenador);

		agregarAColaDeListos(entrenador);

		sem_post(&entrenador_listo);

	}else{
		log_error(myArchivoDeLog,"No se puede entregar recurso por falta de recurso");
	}

}

void desbloquearEntrenador(t_mapa *mapa) {

	int i;
	for (i = 0; i < list_size(colaBloqueados); i++) {

		pthread_mutex_lock(&mutex_bloqueados);

		t_entrenador * entrenador = list_get(colaBloqueados, i);

		pthread_mutex_unlock(&mutex_bloqueados);

		asignarPokemonAEntrenador(mapa, entrenador);

	}

}

void agregarAColaDeBloqueados(t_entrenador * unEntrenador) {
	pthread_mutex_lock(&mutex_bloqueados);

	list_add(colaBloqueados, unEntrenador);

	log_info(myArchivoDeLog,
			"Se agrega a la cola de bloqueados al entrenador: %c",
			unEntrenador->simbolo);

	loguearColasDePlanificacion(colaListos, "Listos");
	loguearColasDePlanificacion(colaBloqueados, "Bloqueados");
	loguearColasDePlanificacion(colaFinalizados, "Finalizados");

	pthread_mutex_unlock(&mutex_bloqueados);

}

void quitarDeColaDeBloqueados(t_entrenador *entrenador) {
	log_debug(myArchivoDeLog, "entroo...");
	pthread_mutex_lock(&mutex_bloqueados);

	bool mismoSocket(void * datos) {

		t_entrenador * elegido = datos;

		return elegido->nroDesocket == entrenador->nroDesocket;

	}

	list_remove_by_condition(colaBloqueados, &mismoSocket);

	log_info(myArchivoDeLog,
			"Se quita de la cola de Bloqueados al entrenador: %c",
			entrenador->simbolo);

	loguearColasDePlanificacion(colaListos, "Listos");
	loguearColasDePlanificacion(colaBloqueados, "Bloqueados");
	loguearColasDePlanificacion(colaFinalizados, "Finalizados");

	pthread_mutex_unlock(&mutex_bloqueados);

}

void desconectarEntrenador(int nroDesocket, t_mapa * mapa,
		fd_set sockets_activos, int socketMasGrande) {

	bool seDesconecto(void * data) {
		t_entrenador * alguno = data;
		return alguno->nroDesocket == nroDesocket;
	}

	t_entrenador * entrenadorAEliminar = list_remove_by_condition(colaListos,
			seDesconecto);

	if (entrenadorAEliminar == NULL) {
		entrenadorAEliminar = list_remove_by_condition(colaBloqueados,
				seDesconecto);
	}
	pthread_mutex_lock(&mutex_sock);
	FD_CLR(nroDesocket, &sockets_activos);
	pthread_mutex_unlock(&mutex_sock);

	close(nroDesocket);

	if (nroDesocket == socketMasGrande) {
		socketMasGrande = 0;
		int fd2 = 0;
		for (fd2 = nroDesocket - 1; fd2 >= 0; fd2--) {
			if (FD_ISSET(fd2, &sockets_activos)) {
				socketMasGrande = fd2;
				log_debug(myArchivoDeLog, "el socket mas grande cambia a: %d",
						socketMasGrande);
				break;

			}
		}
	}

	//Con esto evitamos que un entrenador NUEVO (que no se agrego a las colas de planificacion), haga lio..
	if (entrenadorAEliminar != NULL)
	{
		agregarAColaDeFinalizados(entrenadorAEliminar);

		borrarEntrenadorDelMapa(mapa, entrenadorAEliminar->simbolo);
		sem_post(&semaforoGraficar);

		//liberarRecursos(entrenadorAEliminar);

		sem_post(&mapa_libre);
		sem_post(&entrenador_bloqueado);
	}

	free(entrenadorAEliminar);
}



void agregarAColaDeFinalizados(t_entrenador *entrenadorAEliminar) {

	list_add(colaFinalizados, entrenadorAEliminar);

	log_info(myArchivoDeLog, "Se finaliza el entrenador: %c",
			entrenadorAEliminar->simbolo);

	loguearColasDePlanificacion(colaListos, "Listos");
	loguearColasDePlanificacion(colaBloqueados, "Bloqueados");
	loguearColasDePlanificacion(colaFinalizados, "Finalizados");

}



int consumirQuantum(int i, int quantum) {

	t_entrenador * entrenador = reconocerEntrenadorSegunSocket(i);

	entrenador->instruccionesEjecutadas++;

	log_debug(myArchivoDeLog, "cantidad de instrucciones ejecutadas: %d",
			entrenador->instruccionesEjecutadas);
	if (quantum <= entrenador->instruccionesEjecutadas) {
			log_debug(myArchivoDeLog,
					"El entrenador %c es encolado nuevamente por fin de quantum",
					entrenador->simbolo);
			quitarDeColaDeListos(entrenador);
			entrenador->instruccionesEjecutadas = 0;
			agregarAColaDeListos(entrenador);
			sem_post(&mapa_libre);
			sem_post(&entrenador_listo);
			return 1;
	}else{
		return 0;
	}
}

t_entrenador * reconocerEntrenadorSegunSocket(int nroDeSocket) {
	bool encontrar_socket_entrenador(void * nodo) {
		return ((((t_entrenador*) nodo)->nroDesocket) == nroDeSocket);
	}

	t_entrenador * entrenador = list_find(colaListos,
			encontrar_socket_entrenador);

	return entrenador;
}

t_entrenador * buscarDesconocedorPokenest() {

	bool desconocePokenest(void *datos) {
		t_entrenador * alguno = datos;
		return alguno->distanciaAProximaPokenest == -1;
	}

	t_entrenador *entrenador = list_find(colaListos, desconocePokenest);

	return entrenador;

}

t_entrenador * buscarCercaniaAPokenest() {

	bool cercaDePokenest(void *datos) {
		t_entrenador *elMasCercano = datos;
		int i, menorDistancia;
		menorDistancia = 99;
		for (i = 0; list_size(colaListos) > i; i++) {
			t_entrenador * alguno = list_get(colaListos, i);
			if (alguno->distanciaAProximaPokenest < menorDistancia) {
				menorDistancia = alguno->distanciaAProximaPokenest;
			}
		}
		return elMasCercano->distanciaAProximaPokenest == menorDistancia;
	}

	t_entrenador *entrenador = list_find(colaListos, cercaDePokenest);

	return entrenador;

}

void setearDistanciaPokenest(int nroDeSocket, t_mapa * self, char pokenest) {
	t_entrenador *entrenador = reconocerEntrenadorSegunSocket(nroDeSocket);
	//Lucas: Necesito conocer al mapa para saber la posicion :S
	// Estoy suponiendo que pokemonSolicitado es el char de la pokenest. Ej: 'P'.
	// La unica condicion es que el entrenador ya tiene que estar en el mapa cargado!!

	int aux = distanciaEntrenadorPokenest(entrenador->simbolo, self, pokenest);

	if (aux < 0) {
		//problemas al calcular la distancia
		log_debug(myArchivoDeLog,
				"Hubo algun error al calcular la distancia de #Socket: %s",
				string_itoa((nroDeSocket)));
		//printf("hubo algun error\n");
	} else {
		log_debug(myArchivoDeLog, "distancia a pokenest = %s",
				string_itoa((aux)));
		entrenador->distanciaAProximaPokenest = aux;
	}

	//TODO: ver si el entrenador tiene que mandarme su posicion actual para poder
	//calcular la distancia a la pokenest
	//para poder setear la siguiente variable entrenador->distanciaAProximaPokenest

}

void loguearColasDePlanificacion(t_list *lista, char *nombreLista) {
	int j;
	if (list_size(lista) == 0) {

		log_info(myArchivoDeLog, "La cola de planificacion de %s esta vacía",
				nombreLista);

	} else {

		for (j = 0; j < list_size(lista); j++) {
			t_entrenador * entrenador = list_get(lista, j);

			log_info(myArchivoDeLog, "Cola de %s: %c", nombreLista,
					entrenador->simbolo);

		}

	}
}


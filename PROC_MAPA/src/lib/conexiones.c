/*
 * conexiones.c

 *
 *  Created on: 26/9/2016
 *      Author: utnso
 */

#include "conexiones.h"
#include "libPlanificador.h"

#include <so/libSockets.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void detectarDesconexion(t_data * paquete, int socket_recepcion,
		fd_set sockets_activos, t_mapa * mapa) {

	if (paquete->header == 0) {
		//desconexion

		desconectarEntrenador(socket_recepcion, mapa, sockets_activos,
				socketMasGrande);

		log_debug(myArchivoDeLog, "Se desconecto el numero de socket: %d\n",
				socket_recepcion);

	}

}

int atenderConexion(int i, t_mapa * mapa,fd_set sockets_activos) {
	int flag = -1;
	int resultadoSend = -1;
	t_data * paquete;
	proceder: paquete = leer_paquete(i);
	log_debug(myArchivoDeLog, "Atiendo al socket %d,con el header %d ", i,
			paquete->header);

	switch (paquete->header) {
	case peticionPokenest:
		;

		int coordenadaEnX = -1;		//inicializo en fantasma
		int coordenadaEnY = -1;		//inicializo en fantasma
		if (dondeQuedaEstaPokeNest(mapa, paquete->data, &coordenadaEnX,
				&coordenadaEnY))// Aca esta la magia, si no devolvio error entonces lo ejecuto.
				{
			log_debug(myArchivoDeLog,
					"atenderConexion / peticionPokenest / dondeQuedaEstaPokeNest : %s;%s",
					string_itoa(coordenadaEnX), string_itoa(coordenadaEnY));
			if ((coordenadaEnX != -1) && (coordenadaEnY != -1)) {
				void * buffer = malloc(sizeof(int) * 2);
				memcpy(buffer, &coordenadaEnX, sizeof(int));
				memcpy(buffer + sizeof(int), &coordenadaEnY, sizeof(int));

				t_data * nuevoPaquete = pedirPaquete(ubicacionPokenest,
						sizeof(int) * 2, buffer);

				t_entrenador * entrenador = reconocerEntrenadorSegunSocket(i);

				entrenador->pokenest = *(char*) paquete->data;

				//usleep(mapa->metadata->retardo);
				//sleep(1);

				resultadoSend = common_send(i, nuevoPaquete);

				if(strcmp(mapa->metadata->algoritmo,"RR")==0){
					flag = consumirQuantum(i, mapa->metadata->quantum);
				}else{
					setearDistanciaPokenest(i, mapa, entrenador->pokenest);
					flag = 1;
					sem_post(&mapa_libre);
					sem_post(&entrenador_listo);
				}
				free(nuevoPaquete);

				break;//con esto salvamos que tire error cuando fue una ejecucion valida.
			}
		}

		//romper el entrenador
		log_error(myArchivoDeLog,"error en donde queda esta pokenest");
		desconectarEntrenador(i, mapa, sockets_activos, socketMasGrande);
		return 0;

		break;
	case movimientoEntrenador:
		//TODO: registrar movimiento del entrenador, descartar quantum
		;

		int respuesta = -1;
		memcpy(&respuesta, paquete->data, sizeof(enum actividad));

		if (respuesta != -1) {
			t_entrenador * entrenador = reconocerEntrenadorSegunSocket(i);

			//Mando mover al entrenador. Si hace algo raro, lo desconecto.
			if (moverEntrenador(mapa, entrenador->simbolo, respuesta)) {
				log_info(myArchivoDeLog, "Desconecte a %c xq se movio mal",
						entrenador->simbolo);
				//desconectarEntrenador(i, mapa, sockets_activos,
				//		socketMasGrande);
				break;
				//TODO: le tengo que avisar al entrenador que hizo algo mal??
			}

			//usleep(mapa->metadata->retardo);
			//sleep(1);

			if(strcmp(mapa->metadata->algoritmo,"RR")==0){
				flag = consumirQuantum(i, mapa->metadata->quantum);
			}else{
				setearDistanciaPokenest(i, mapa, entrenador->pokenest);
				flag = 1;
				sem_post(&mapa_libre);
				sem_post(&entrenador_listo);
			}
			int null_data = 0;
			t_data *turno = pedirPaquete(otorgarTurno, sizeof(int), &null_data);
			resultadoSend = common_send(entrenador->nroDesocket, turno);
			free(turno);


			break;
		}

		//romper el entrenador
		log_error(myArchivoDeLog,"error en donde queda esta pokenest");
		desconectarEntrenador(i, mapa, sockets_activos, socketMasGrande);
		return 0;
		break;
	case capturarPokemon:
		;
		//paquete->data tendra el identificador del pokemon a atrapar

		t_entrenador * entrenador = reconocerEntrenadorSegunSocket(i);

		char *identificador = paquete->data;

		entrenador->pokemonSolicitado = identificador[0];
		log_debug(myArchivoDeLog,"EL ENTRENADOR %c SOLICITA POKE: %c",entrenador->simbolo,entrenador->pokemonSolicitado);
		flag = 1;	// le aborto el quantum!

		quitarDeColaDeListos(entrenador);

		sem_post(&mapa_libre);

		agregarAColaDeBloqueados(entrenador);

		//usleep(mapa->metadata->retardo);
		//sleep(1);

		sem_post(&entrenador_bloqueado);

		return 0;	//le aborto el turno
		break;
	case mejorPokemon:
		//TODO: recibe al mejor pokemon para... batalla pokemon?
		break;
	default:
		/*desconectarEntrenador(i, mapa,sockets_activos,
		 socketMasGrande);

		 log_debug(myArchivoDeLog, "Se desconecto el numero de socket: %d\n",
		 i);
		 return 1;*/
		log_error(myArchivoDeLog,"estoy en un default de un switch case en atenderConexion");
		//return 0;
		break;
	}

	log_info(myArchivoDeLog,"VALOR FLAG:%d",flag);
	if (resultadoSend == 0 || paquete->header <= 0) {
		desconectarEntrenador(i, mapa, sockets_activos, socketMasGrande);
		return 0;
	}

	sleepInMiliSegundos(mapa->metadata->retardo);
	sem_post(&semaforoGraficar);

	//Valido si llego una senial en el medio.
	if (_SIGUSR2_flag ==1)
		return 0;


	if (flag == 0 || flag == -1) {
		goto proceder;
	}

	return 0;
}

void handshake(int socket_nueva_conexion, fd_set sockets_activos, t_mapa * mapa) {
	t_data * paquete = leer_paquete(socket_nueva_conexion);

	if (paquete->header == 99) {

		t_entrenador *unEntrenador = generarEntrenador(socket_nueva_conexion,
				paquete->data);



		if (unEntrenador->simbolo != '\0') {
			//Mando mover al entrenador. Si hace algo raro, lo desconecto.
			if (cargarEntrenador(mapa->items, unEntrenador->simbolo)) {
				log_info(myArchivoDeLog, "Desconecte a %c xq no lo pude cargar",
						unEntrenador->simbolo);
				log_error(myArchivoDeLog,
						"No se pudo conectar, fallo el handshake\n");
				desconectarEntrenador(socket_nueva_conexion, mapa,
						sockets_activos, socketMasGrande);
				return;
				//TODO: le tengo que avisar al entrenador que hizo algo mal??
			}
		}
		//-------

		log_debug(myArchivoDeLog,
				"Se creo un entrenador con simbolo: %c, y con numero de socket: %d",
				unEntrenador->simbolo, unEntrenador->nroDesocket);

		int null_data = 0;

		t_data * paquete = pedirPaquete(50, sizeof(int), &null_data);

		common_send(socket_nueva_conexion, paquete);

		agregarAColaDeListos(unEntrenador);

		sem_post(&semaforoGraficar);
		sem_post(&entrenador_listo);

	} else {

		log_error(myArchivoDeLog, "No se pudo conectar, fallo el handshake\n");
		finalizarGui(mapa);
	}
}

int atenderConexiones(void * data) {
	t_mapa * mapa = data;

	int socketEscucha;

	fd_set sockets_para_revisar;

	socketEscucha = setup_listen("localhost", mapa->metadata->puerto);

	log_debug(myArchivoDeLog, "Se crea el socket escucha con numero: %d\n",
			socketEscucha);

	listen(socketEscucha, 1024);

	socketMasGrande = socketEscucha;
	FD_ZERO(&sockets_activos);
	FD_SET(socketEscucha, &sockets_activos);


	while (1) {
		sleepInMiliSegundos(1);

		pthread_mutex_lock(&mutex_sock);
		sockets_para_revisar = sockets_activos;
		pthread_mutex_unlock(&mutex_sock);

		int retornoSelect;
//		select:
		retornoSelect = select(socketMasGrande + 1, &sockets_para_revisar,
		NULL, NULL, NULL);

		if (retornoSelect == -1) {

		}

		int i;

		for (i = 0; i <= socketMasGrande; i++) {
			if (FD_ISSET(i, &sockets_para_revisar)) {
				//hay actividad
				if (i == socketEscucha) {
					//es una nueva conexion sobre el puerto de escucha
					struct sockaddr_storage remoteaddr;
					socklen_t addrlen;
					addrlen = sizeof(remoteaddr);

					int socket_nueva_conexion = accept(socketEscucha,
							(struct sockaddr *) &remoteaddr, &addrlen);
					if (socket_nueva_conexion == -1) {
						//se desconecto o no conecto alguno
						puts("error al asignar socket a la nueva conexion");

					} else {
						//Ponemos al socket nuevo en el set de sockets activos
						pthread_mutex_lock(&mutex_sock);
						FD_SET(socket_nueva_conexion, &sockets_activos);
						pthread_mutex_unlock(&mutex_sock);

						if (socket_nueva_conexion > socketMasGrande) {
							socketMasGrande = socket_nueva_conexion;
						}

						handshake(socket_nueva_conexion, sockets_activos, data);

					}
				} else {
					//la actividad es un puerto ya enlazado, hay que atenderlo
					//int resultado =
					//atenderConexion(i, mapa);
					//if(resultado == 1){

				}
			}
		}

	}
}


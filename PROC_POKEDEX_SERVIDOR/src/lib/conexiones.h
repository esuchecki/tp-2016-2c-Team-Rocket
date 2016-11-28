/*
 * conexiones.h
 *
 *  Created on: 26/9/2016
 *      Author: utnso
 */

#ifndef LIB_CONEXIONES_H_
#define LIB_CONEXIONES_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <commons/collections/list.h>
#include <commons/log.h>

typedef struct {
	int socketEscucha;
	int socketMasGrande;
	pthread_t hilo;
	fd_set sockets_activos;
} nodo_hilo;

enum modosApertura{
	modo_lectura = 5,
	modo_escritura = 8,
	modo_lectura_escritura = 10,
	noEstaAbierto = 16
};


//typedef struct {
//	int ubicacionEnTablaArchivos;
//	int modoApertura;
//	int cantidadDeVecesAbierta;
//	pthread_rwlock_t * sem_rw;
//}nodo_archivo;


pthread_rwlock_t semaforoConcurrenciaArchivo[2048];

t_list * tablaArchivosAbiertos;


t_log * logServidor;

pthread_mutex_t mutex_mayor;
pthread_mutex_t mutex_archivos;

int atenderConexiones();

void handshake(int socket_nueva_conexion, fd_set sockets_activos);

void atenderConexion(int socket_conexion);

void establecerConexion(void * data);

void escucharNuevasConexiones(char* ip,char *puerto);

//nodo_archivo * verificarAperturasArchivos(char * path);

void inicializarSemaforosTablaArchivos ();


#endif /* LIB_CONEXIONES_H_ */

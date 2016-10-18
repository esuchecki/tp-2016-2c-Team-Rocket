/*
 * libSockets.h
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#ifndef LIBSOCKETS_H_
#define LIBSOCKETS_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int header;
	int tamanio;
	void * data;
}t_data;

enum enviosEntrenador{
	peticionPokenest = 10,
	movimientoEntrenador = 11,//ver si subdividirlo en 4 diferentes
	capturarPokemon = 12,
	objetivosCumplidos = 13,
	mejorPokemon = 14,
	otorgarTurno = 15
} ;

enum enviosMapa{
	ubicacionPokenest = 20,
	capturastePokemon = 21,
	dameMejorPokemon = 22,
	ganasteBatalla = 23,
	perdisteBatalla = 24,
	ubicacionMedallaMapa = 25
};

//MAXIS: son los mensajes "internos" para que el entrenador se mueva.
enum actividad {
	destino = 0,
	moverDerecha = 1,
	moverArriba = 2,
	moverIzquierda = 3,
	moverAbajo = 4,
	noActividad = 5
};

/*
 * @NAME: common_setup
 * @DESC: devuelve las configuraciones para utilizar sockets, tanto para el servidor como el cliente
 */

struct addrinfo* common_setup(char * IP, char * Port);

/*
 * @NAME: setup_listen
 * @DESC: busca el numero de socket y utiliza la funcion bind para enlazarse al puerto
 */

int setup_listen(char * IP,char * Port);

/*
 * @NAME: connect_to
 * @DESC: funcion para el cliente, para conectarse a un servidor
 */

int connect_to(char* IP, char * port);

/*
 * @NAME: leer_paquete
 * @DESC: recibe un paquete de tipo t_data del socket pasado por parametro
 */

t_data * leer_paquete(int socket);

/*
 * @NAME: pedirPaquete
 * @DESC: arma un paquete del tipo t_data segun los parametros que le pasan,
 * 		header,es el codigo de operacion, tamanio, es el tamanio de la data, data, los datos a enviar.
 */

t_data * pedirPaquete(int header,int tamanio,void * data);

/*
 * @NAME: serializar
 * @DESC: coloca en un buffer el paquete a enviar, lo serializa
 */

char * serializar(t_data * unPaquete);

/*
 * @NAME: common_send
 * @DESC: envia al socket pasado por parametro el paquete t_data
 */

int common_send(int socket, t_data * paquete);




#endif /* LIBSOCKETS_H_ */

/*
 ============================================================================
 Name        : MovEnMapa.c
 Author      : maxis
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "commons/collections/list.h"

typedef struct estadoEntrenador {
	int e_posX;
	int e_posY;
	int p_posX;
	int p_posY;
	char ultimoMov;
} t_estadoEntrenador;

typedef struct hitos {
	int p_posX;
	int p_posY;
	char p_nombre;
} t_hitos;

enum actividad {
	destino = 0,
	moverDerecha = 1,
	moverArriba = 2,
	moverIzquierda = 3,
	moverAbajo = 4,
	noActividad = 5
};

/**
 *
 * ultimoMov->'x' o 'y'
 * respuesta:
 * 			- 0 -> esta en destino
 * 			- 1 -> movete derecha
 * 			- 2 -> movete arriba
 * 			- 3 -> movete izquierda
 * 			- 4 -> movete abajo
 * 			- -1-> hay algun error... ver mas adelante
 */

int queHago(t_estadoEntrenador* estado) {

	//TODO: ver en cada movimiento que no se vaya fuera del mapa...
	// en teoria no va a pasar porque los pokemones van a estar en el mapa y se va a mover hasta alli.
	// pero quedaria joya chequearlo
	int respuesta = -1;
	int distanciaX = estado->p_posX - estado->e_posX;
	int distanciaY = estado->p_posY - estado->e_posY;
	if (estado->ultimoMov == 'X') {
		if (distanciaY < 0) {
			respuesta = 2;
		} else if (distanciaY > 0) {
			respuesta = 4;
		} else if (distanciaX < 0) {
			respuesta = 1;
		} else if (distanciaX > 0) {
			respuesta = 3;
		} else {
			respuesta = 0;
		}
	} else if (estado->ultimoMov == 'Y') {
		if (distanciaX < 0) {
			respuesta = 3;
		} else if (distanciaX > 0) {
			respuesta = 1;
		} else if (distanciaY < 0) {
			respuesta = 2;
		} else if (distanciaY > 0) {
			respuesta = 4;
		} else {
			respuesta = 0;
		}
	} else {
		//en este caso estoy en el primer movimiento... tomo este por defecto, ver cual tomar
		respuesta = 1;
	}

	return respuesta;
}

void capturarPoke(){
	//TODO.. todo jaj
}

void actualizarEstado(t_estadoEntrenador* estado, int respuesta) {

	switch (respuesta) {
	case destino:
		//TODO: falta implementar la func...
		capturarPoke();
		break;
	case moverDerecha:
		estado->e_posX++;
		estado->ultimoMov = 'X';
		puts("muevoDerecha");
		break;
	case moverArriba:
		estado->e_posY--;
		estado->ultimoMov = 'Y';
		puts("muevoArriba");
		break;
	case moverIzquierda:
		estado->e_posX--;
		estado->ultimoMov = 'X';
		puts("muevoIzquierda");
		break;
	case moverAbajo:
		estado->e_posY++;
		estado->ultimoMov = 'Y';
		puts("muevoAbajo");
		break;
	case noActividad:
		break;
	}
}

int main(void) {

	t_estadoEntrenador* estado = malloc(sizeof(t_estadoEntrenador));
	/*Seteo los valores ahora a mano, luego los traemos del config*/
	estado->e_posX = 0;
	estado->e_posY = 0;
	//TODO: CREO la lista de pokes asi voy agregando la estructura que cree
	//		y despues la recorremos asi va a buscar varios pokes
	/*
	 *
	 *
	 *
	 *
	 * la linea de aca abajo no anda
	 * la 156 tampoco!!!
	 */
	//t_list* listaPoke = list_create();
	int itera = 0;
	char nombre = 'a';
	int px = 2;
	int py = 3;
	while (itera < 4) {

		t_hitos* hitos = malloc(sizeof(t_hitos));
		hitos->p_nombre = nombre;
		hitos->p_posX = px;
		hitos->p_posY = py;
		itera++;
		nombre++;
		px += 3;
		py += 4;
		//list_add(listaPoke, hitos);
	}

	/*
	 *
	 * esto lo voy a cambiar cuando ande la lista de hitos... entonces
	 * voy a recorrer esa lista buscando las posiciones donde estan los pokes
	 */
	estado->p_posX = 7;
	estado->p_posY = 10;
	estado->ultimoMov = 'Y';




	int act = 99;
	int iteracion = 0;
	while (act > 0) {
		printf("iteracion %i \n", iteracion);
		int respuesta = queHago(estado);
		printf("devuelve %i \n", respuesta);
		actualizarEstado(estado, respuesta);
		iteracion++;
		if (respuesta == 0) {
			act = 0;
			printf("TOMA POKEMON!!!!!  \n");
		} else if (respuesta == -1) {
			act = -1;
			printf("SE FUE DEL MAPA, O ALGUN ERROR PASO! \n");
		}
	}

	free(estado);
	return EXIT_SUCCESS;
}


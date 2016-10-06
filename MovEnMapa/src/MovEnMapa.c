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
#include <string.h>
#include <stdlib.h>
#include <commons/collections/list.h>


typedef struct estadoEntrenador {
	int e_posX;
	int e_posY;
	int p_posX;
	int p_posY;
	char ultimoMov;
} t_estadoEntrenador;

typedef struct hitos {
	char p_nombre;
	char m_nombre[20];
} t_hitos;

enum actividad {
	destino = 0,
	moverDerecha = 1,
	moverArriba = 2,
	moverIzquierda = 3,
	moverAbajo = 4,
	noActividad = 5
};

int posX = 3;
int posY = 5;

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

void actualizarEstado(t_estadoEntrenador* estado, int respuesta) {

	switch (respuesta) {
	case destino:
		//TODO: falta implementar la func...

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

void actualizarPosicionPoke() {
	posX = posX + 3;
	posY = posX + 1;
	printf("posicion x: %i ", posX);
	printf("posicion y: %i\n", posY);
}

void inicializar(t_list* listaDePokeACapturar) {
	int mapa = 1;
	while (mapa <= 3) {
		int poke;
		poke = 1;
		char pokeActual = 'a';
		while (poke <= 4) {
			t_hitos* pokeHitos = malloc(sizeof(t_hitos));

			if (mapa == 1) {
				strcpy(pokeHitos->m_nombre, "primer mapa");
			} else if (mapa == 2) {
				strcpy(pokeHitos->m_nombre, "segundo mapa");
			} else {
				strcpy(pokeHitos->m_nombre, "tercer mapa");
			}

			pokeHitos->p_nombre = pokeActual;
			list_add(listaDePokeACapturar, pokeHitos);
			printf("mapa: %d\n", mapa);
			printf("agrego pokemon: %c\n", pokeActual);
			poke = poke + 1;
			//printf("pokemon nro : %i\n", *poke);
			pokeActual++;

		}
		mapa++;
	}

}

void soyMaestroPoke() {
	puts("SOY MAESTRO POKE EMI, SOY MAESTRO POKEEEEEEEEEEEEE");
}

void buscarUnPokemon(t_hitos* pokeActual, t_estadoEntrenador* estado,
		t_list* listaPokeCapturados) {
	printf("tengo que buscar a: %c \n", pokeActual->p_nombre);

	estado->p_posX = posX;
	estado->p_posY = posY;
	int respuesta = 99;
	while (respuesta > 0) {

		respuesta = queHago(estado);
		printf("devuelve %i \n", respuesta);
		actualizarEstado(estado, respuesta);

		if (respuesta == 0) {

			//agregar pokemon a la lista de capturados
			agregarPokemonACapturados(listaPokeCapturados, pokeActual);
			actualizarPosicionPoke();
			printf("TOMA POKEMON!!!!!  \n");
		} else if (respuesta == -1) {

			printf("SE FUE DEL MAPA, O ALGUN ERROR PASO! \n");
		}
	}

}
void agregarPokemonACapturados(t_list* listaPokeCapturados, t_hitos* pokeActual) {

	list_add(listaPokeCapturados, pokeActual);
}

void desarrollarJuego(t_list* listaDePokeACapturar, t_list* listaPokeCapturados,
		t_estadoEntrenador* estado) {
	//MAXIS... VOY A USAR UN ITERADOR MIO, SOLO PORQUE TENGO LOS MAPAS EN LA MISMA LISTA...
	// DESPUES DESCOMENTAMOS LA LISTA DE ABAJO Y CORREGIMOS
	//list_iterate(listaDePokeACapturar, &buscarUnPokemon);
	bool seguir = true;
	actualizarPosicionPoke();
	int posicion = 0;
	t_hitos* pokeActual = list_get(listaDePokeACapturar, posicion);
	while (pokeActual != NULL && seguir) {
		buscarUnPokemon(pokeActual, estado, listaPokeCapturados);
		posicion++;
		pokeActual = list_get(listaDePokeACapturar, posicion);
	}
soyMaestroPoke();
}

int main(void) {
	/*
	 *
	 * 29092016...revisar este comentario...
	 * -- tendria que tener dos arrays que simulen dos "mapas" cuando termina con uno, sigue con otro.
	 * lo que tengo que hacer es...
	 * 1- pido posicion de pokemon
	 * 2- la guardo  en mi estado
	 * 3- itero hasta llegar a la posicion del pokemon
	 * 4- lo almaceno en una "lista" de poke capturados
	 * 5- sigo con otro pokemon
	 *
	 */
	t_list* listaDePokeACapturar = list_create();
	inicializar(listaDePokeACapturar);

	t_list* listaPokeCapturados = list_create();
	t_estadoEntrenador* estado = malloc(sizeof(t_estadoEntrenador));

	/*Seteo los valores ahora a mano, luego los traemos del config*/
	estado->e_posX = 1;
	estado->e_posY = 1;

	desarrollarJuego(listaDePokeACapturar, listaPokeCapturados, estado);

	free(estado);
	free(listaDePokeACapturar);
	free(listaPokeCapturados);
	return EXIT_SUCCESS;
}


/*
 * estructurasMapa.h
 *
 *  Created on: 17/9/2016
 *      Author: utnso
 */

#ifndef LIB_ESTRUCTURASMAPA_H_
#define LIB_ESTRUCTURASMAPA_H_

#include <stdint.h>
#include <tad_items.h>		//contiene una lista con los items a dibujar en el mapa
#include <pkmn/factory.h>	//contiene el t_pokemon utilizado para batallar pkmn
#include <commons/log.h>

//------------------------------------------//
/* ********************************************	*/
//----------- Sector Estructuras -------------//

typedef struct
{
	uint16_t tiempoChequeadoDeadlock;
	char * batalla;
	char * algoritmo;
	uint16_t quantum;
	uint16_t retardo;	//Valor en milisegundos
	char * ip;
	char * puerto;
} t_metadataMapa ;


typedef struct
{
	char * pokemonNNNdat;	//nombre del archivo.
	char capturadoPorEntrenador;	//simbolo del entrenador para identificar quien lo tiene ('\0' para ninguno)
} t_pokemonEnPokeNest;


typedef struct
{
	char identificador;
	t_list * pokemones;	//lista de t_pokemonEnPokeNest;
} t_pokeNest;


typedef struct
{
     char * nombre;
     char * directorioPokeDex;
     t_metadataMapa * metadata;
     t_list* items;
     t_list * pokeNest;		//a futuro esto deberia ser un array o una lista enrealidad...
} t_mapa ;


//Esto es usado por el planificador! :)
typedef struct{
	int nroDesocket;
	char simbolo;
	int instruccionesEjecutadas;
	int distanciaAProximaPokenest;
	char pokemonSolicitado;
	char pokenest;
	t_pokemon * mejorPokemon;	//Por defecto setear en Null.
	//TODO: ver que otros datos poner aca
}t_entrenador;



//------------------------------------------//

#endif /* LIB_ESTRUCTURASMAPA_H_ */


//Esto es para cuando hago "estructurasMapa.h", si la variable de log ya estaba declarada, la hace declarar como extern. (xq ya estaba inicializada)
#ifndef LIB_ESTRUCTURASMAPA_log_H_
#define LIB_ESTRUCTURASMAPA_log_H_
#else
#define extern LIB_ESTRUCTURASMAPA_log_H_
#endif /* DEFINE_VARIABLES */

LIB_ESTRUCTURASMAPA_log_H_ t_log* myArchivoDeLog;

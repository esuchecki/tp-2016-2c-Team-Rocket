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
	uint16_t retardo;
	char * ip;
	char * puerto;
} t_metadataMapa ;


typedef struct
{
	//uint16_t nivel;
	char capturadoPorEntrenador;	//simbolo del entrenador para identificar quien lo tiene ('\0' para ninguno)
	//TODO: revisar si agregar un campo NNN, para saber que archivo es este pokemon.
	//entiendo que la correspondencia entre el NNN y el int (index) es NNN=index-1.
} t_pokemon;


typedef struct
{
	char identificador;
	//char tipo[20];
	//uint16_t pos_x;
	//uint16_t pos_y;
	t_list * pokemones;	//a futuro esto deberia ser un array o una lista enrealidad....
} t_pokeNest;


typedef struct
{
     char * nombre;
     char * directorioPokeDex;
     t_metadataMapa * metadata;
     t_list* items;
     t_list * pokeNest;		//a futuro esto deberia ser un array o una lista enrealidad...
} t_mapa ;






//------------------------------------------//

#endif /* LIB_ESTRUCTURASMAPA_H_ */


//Esto es para cuando hago "estructurasMapa.h", si la variable de log ya estaba declarada, la hace declarar como extern. (xq ya estaba inicializada)
#ifndef LIB_ESTRUCTURASMAPA_log_H_
#define LIB_ESTRUCTURASMAPA_log_H_
#else
#define extern LIB_ESTRUCTURASMAPA_log_H_
#endif /* DEFINE_VARIABLES */

LIB_ESTRUCTURASMAPA_log_H_ t_log* myArchivoDeLog;

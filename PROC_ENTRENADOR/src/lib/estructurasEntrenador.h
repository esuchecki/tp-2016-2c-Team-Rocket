/*
 * estructurasEntrenador.h
 *
 *  Created on: 20/9/2016
 *      Author: utnso
 */

#ifndef LIB_ESTRUCTURASENTRENADOR_H_
#define LIB_ESTRUCTURASENTRENADOR_H_

#include <stdint.h>
#include <tad_items.h>		//contiene una lista con los items a dibujar en el mapa

#include <commons/log.h>

//------------------------------------------//
/* ********************************************	*/
//----------- Sector Estructuras -------------//

typedef struct
{
	int cant_muertes;
	int cant_deadlocks;
	int cant_reintentos;
	time_t bloqEnPokeNest_time;
	time_t start_time;
}t_stats;


typedef struct
{
	char * nombreMapa;
	char ** objetivosDelMapa;	//VALORES=[1,2,3,4,5]. El array termina en NULL.
} t_mapa;

typedef struct
{
	char simbolo;
	t_list * hojaDeViaje;	//Es una lista que contiene nodos del tipo t_mapa.
	uint16_t vidas;
	uint16_t reintentos;
} t_metadataEntrenador ;


//MAXIS-> aca voy a guardar el estado activo del entrenador
typedef struct estadoEntrenador {
	int e_posX;
	int e_posY;
	int p_posX;
	int p_posY;
	char ultimoMov;
	int respuesta;
	int indexMapaActual;
	int indexObjetivoPkmn;
} t_estadoEntrenador;


typedef struct
{
     char * nombre;
     char * directorioPokeDex;
     t_metadataEntrenador* metadata;
     t_stats misEstadisticas;
     t_estadoEntrenador * moverseEnMapa;
} t_entrenadorFisico;




//Esto es para cuando hago "estructurasMapa.h", si la variable de log ya estaba declarada, la hace declarar como extern. (xq ya estaba inicializada)
#ifndef LIB_ESTRUCTURASENTRENADOR_log_H_
#define LIB_ESTRUCTURASENTRENADOR_log_H_
#else
	#ifndef LIB_ESTRUCTURASENTRENADOR_extern_log_H_
	#define LIB_ESTRUCTURASENTRENADOR_extern_log_H_
	extern LIB_ESTRUCTURASENTRENADOR_log_H_
	#endif
#endif /* DEFINE_VARIABLES */

LIB_ESTRUCTURASENTRENADOR_log_H_ t_log* myArchivoDeLog;




#endif /* LIB_ESTRUCTURASENTRENADOR_H_ */

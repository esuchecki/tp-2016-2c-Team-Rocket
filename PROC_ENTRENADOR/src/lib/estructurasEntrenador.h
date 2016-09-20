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
	char * nombreMapa;
	char * objetivosDelMapa;	//array o lista a futuro  - a definir
} t_mapa;

typedef struct
{
	char * simbolo;
	t_mapa * hojaDeViaje;	//array o lista a futuro - a definir
	uint16_t vidas;
	uint16_t reintentos;
} t_metadataEntrenador ;


typedef struct
{
     char * nombre;
     char * directorioPokeDex;
     t_metadataEntrenador* metadata;
} t_entrenadorFisico;




//Esto es para cuando hago "estructurasMapa.h", si la variable de log ya estaba declarada, la hace declarar como extern. (xq ya estaba inicializada)
#ifndef LIB_ESTRUCTURASENTRENADOR_H_
#define LIB_ESTRUCTURASENTRENADOR_H_
#else
#define extern LIB_ESTRUCTURASENTRENADOR_H_
#endif /* DEFINE_VARIABLES */

LIB_ESTRUCTURASENTRENADOR_H_ t_log* myArchivoDeLog;




#endif /* LIB_ESTRUCTURASENTRENADOR_H_ */

/*
 * tiempo.h
 *
 *  Created on: 20/9/2016
 *      Author: utnso
 */

#ifndef LIB_ESTADISTICAS_H_
#define LIB_ESTADISTICAS_H_

#include <time.h>
#include "estructurasEntrenador.h"
#include <stdlib.h>


/*
 * @NAME: inicializarTiemposDelEntrenador
 * @DESC: Carga el tiempo de inicio al entrenador
 */
void inicializarTiemposDelEntrenador (t_stats * estadisticas);


/*
 * @NAME: mostrarEstadisticas
 * @DESC: Fc a ser llamada cuando es maestro pokemon.
 */
void mostrarEstadisticas(t_stats * estadisticas);

#endif /* LIB_ESTADISTICAS_H_ */

/*
 * batallaPkmn.h
 *
 *  Created on: 6/10/2016
 *      Author: utnso
 */

#ifndef LIB_BATALLAPKMN_H_
#define LIB_BATALLAPKMN_H_

#define _GNU_SOURCE	//esto es para usar asprintf
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pkmn/factory.h>


#include "estructurasEntrenador.h"
#include <so/constantes.h>	//para los path's
#include "entrenadorConfig.h"
#include <so/libConfig.h>




t_pokemon * elegirMejorPokemon (t_entrenadorFisico * unEntrenador, void (*fc_abortar) (void *));







#endif /* LIB_BATALLAPKMN_H_ */

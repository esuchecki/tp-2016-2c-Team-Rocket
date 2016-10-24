/*
 * movimiento.h
 *
 *  Created on: 8/10/2016
 *      Author: utnso
 */

#ifndef LIB_MOVIMIENTO_H_
#define LIB_MOVIMIENTO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//agreagado de librerias utn.so
#include <commons/collections/list.h>
#include <commons/log.h>
#include <so/libSockets.h>
#include <unistd.h>
#include <pkmn/factory.h>

#include "estadisticas.h"
//librerias propias

#include "estructurasEntrenador.h"
#include "seniales.h"
#include "so/constantes.h"
#include "libConfigStruct.h"
#include "estadisticas.h"
#include "batallaPkmn.h"
#include "abortarEntrenador.h"




void iniciarAventura(t_entrenadorFisico * unEntrenador);


#endif /* LIB_MOVIMIENTO_H_ */

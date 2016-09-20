/*
 * libConfigStruct.h
 *
 *  Created on: 20/9/2016
 *      Author: utnso
 */

#ifndef LIB_LIBCONFIGSTRUCT_H_
#define LIB_LIBCONFIGSTRUCT_H_



#include "entrenadorConfig.h"
#include "estructurasEntrenador.h"


void borrarEntrenador ();
t_entrenadorFisico * inicializarEstructurasDelEntrenador (char *nombreEntrenador, char *directorioPokeDex);
void finalizarEntrenador (t_entrenadorFisico * nuevoEntrenador);
void leerMetadataDelEntrenador (t_entrenadorFisico * nuevoEntrenador);
t_config *config_create_for_metadataEntrenador(t_entrenadorFisico * nuevoEntrenador);
void loguearEstructuraDelEntrenador(t_entrenadorFisico * nuevoEntrenador);
//------------------------------------------//




#define __ubicacionArchivoDeLog "./logEntrenador_teamRocket"

#define __ubicacionEntrenadores "/Entrenadores/"
#define __ubicacionMetadataEntrenadores "/metadata"

#define __nombreEnConfig_Vidas "vidas"
#define __nombreEnConfig_Reintentos "reintentos"




#endif /* LIB_LIBCONFIGSTRUCT_H_ */

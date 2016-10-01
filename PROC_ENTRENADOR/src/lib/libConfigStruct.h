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
#include "so/constantes.h"

/*
 * @NAME: borrarEntrenador
 * @DESC: libera de memoria la estructura de un entrenador (uso interno)
 */
void borrarEntrenador ();

/*
 * @NAME: inicializarEstructurasDelEntrenador
 * @DESC: Lee sus archivos config y los carga en memoria
 */
t_entrenadorFisico * inicializarEstructurasDelEntrenador (char *nombreEntrenador, char *directorioPokeDex);

/*
 * @NAME: finalizarEntrenador
 * @DESC: Funcion de EXIT (salida) correcta de un entenador.
 */
void finalizarEntrenador (t_entrenadorFisico * nuevoEntrenador);

/*
 * @NAME: leerMetadataDelEntrenador
 * @DESC: Cargar la parte de t_metadata de un entrenador (uso interno)
 */
void leerMetadataDelEntrenador (t_entrenadorFisico * nuevoEntrenador);

/*
 * @NAME: config_create_for_metadataEntrenador
 * @DESC: Crea el archivo de config de un entrenador (uso interno)
 */
t_config *config_create_for_metadataEntrenador(t_entrenadorFisico * nuevoEntrenador);

/*
 * @NAME: loguearEstructuraDelEntrenador
 * @DESC: Loguea todas las estructuras de un t_entrenadorFisico en el archivo de log.
 */
void loguearEstructuraDelEntrenador(t_entrenadorFisico * nuevoEntrenador);

/*
 * @NAME: inicializarHojaDeViaje
 * @DESC: Se encarga de cargar la hoja de viaje en memoria (uso interno)
 */
void inicializarHojaDeViaje (t_config *metadataEntrenador, t_entrenadorFisico * miEntrenador);

/*
 * @NAME: crearNuevoNodo
 * @DESC: Funcion auxiliar para inicializar la hoja de viajes.  (uso interno)
 */
t_mapa * crearNuevoNodo (char * aQueMapasMeTengoQueConectar, t_config *metadataEntrenador, t_entrenadorFisico * miEntrenador);

/*
 * @NAME: loguearHojaDeViajeDeUnMapa
 * @DESC: Loguea exclusivamente la hoja de viajes en un log
 */
void loguearHojaDeViajeDeUnMapa (t_mapa * nodoActual);

/*
 * @NAME: hayAlgunObjetivoDuplicado
 * @DESC: Valida si en los objetivos de un mapa pasa esto:
 * 		obj[Mapa8]=[P,P,F] (error!)
 * 		obj[Mapa8]=[P,F,P,F,P,F] (ok!)
 */
void hayAlgunObjetivoDuplicado (char ** objetivoDeEsteMapa, t_entrenadorFisico * miEntrenador, t_mapa * nodoActual);
//------------------------------------------//






#endif /* LIB_LIBCONFIGSTRUCT_H_ */

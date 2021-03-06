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
#include "abortarEntrenador.h"


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
 * @NAME: loguearEstructuraDelEntrenador
 * @DESC: Loguea todas las estructuras de un t_entrenadorFisico en el archivo de log.
 */
void loguearEstructuraDelEntrenador(t_entrenadorFisico * nuevoEntrenador);

/*
 * @NAME: cualEsLaIpDeEsteMapa
 * @DESC: Hay que pasarle un puntero (sin inicializar) para la Ip y otro idem para el puerto.
 * 		-> Los devuelve inicializados, con la metadata del mapa (nombreMapa).
 * 		-> Si hubo algun lio los devuelve en NULL.
 */
void cualEsLaIpDeEsteMapa (t_entrenadorFisico * miEntrenador, char * nombreMapa, char ** returnedIp, char ** returnedPort);

/*
 * @NAME: borrarDirectorioDeBill
 * @DESC: Borra el contenido del directorio de bill..!
 */
void borrarDirectorioDeBill(t_entrenadorFisico * unEntrenador);

/*
 * @NAME: borrarDirectorioDeMedallas
 * @DESC: Borra el contenido del directorio de medallas..!
 */
void borrarDirectorioDeMedallas(t_entrenadorFisico * unEntrenador);

/*
 * @NAME: crearFolderDirDeBill
 * @DESC: crea la carpeta Dir de Bill
 */
void crearFolderDirDeBill(t_entrenadorFisico * unEntrenador);

/*
 * @NAME: crearFolderMedallas
 * @DESC: crea la carpeta Dir de Medallas
 */
void crearFolderMedallas(t_entrenadorFisico * unEntrenador);
//------------------------------------------//






#endif /* LIB_LIBCONFIGSTRUCT_H_ */

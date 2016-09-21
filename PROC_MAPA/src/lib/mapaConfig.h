/*
 * mapaConfig.h
 *
 *  Created on: 14/9/2016
 *      Author: utnso
 */

#ifndef LIB_MAPACONFIG_H_
#define LIB_MAPACONFIG_H_





#include <so/libConfig.h>
#include <stdlib.h>
#include <commons/string.h>
#include "estructurasMapa.h"

/*
 * @NAME: _mapa_configLeerInt
 * @DESC: Implementa configLeerInt para el proceso mapa.
 */
uint16_t _mapa_configLeerInt (t_config * archivoConfig, char nombreDeLaPropiedad[50], t_mapa * unMapa, void (*fc) (t_mapa *));

/*
 * @NAME: _mapa_configLeerString
 * @DESC: Implementa configLeerString para el proceso mapa.
 */
char * _mapa_configLeerString (t_config * archivoConfig, char nombreDeLaPropiedad[50], t_mapa * unMapa, void (*fc) (t_mapa *));

/*
 * @NAME: _mapa_newConfigType
 * @DESC: Implementa _newConfigType para el proceso mapa.
 */
t_config * _mapa_newConfigType (char * directorio, t_mapa * unMapa, void (*fc) (t_mapa *));


#endif /* LIB_MAPACONFIG_H_ */

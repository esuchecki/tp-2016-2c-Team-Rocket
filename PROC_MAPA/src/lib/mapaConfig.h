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

/*
 * @NAME: _mapa_configLeerInt
 * @DESC: Implementa configLeerInt para el proceso mapa.
 */
uint16_t _mapa_configLeerInt (t_config * archivoConfig, char nombreDeLaPropiedad[50]);

/*
 * @NAME: _mapa_configLeerString
 * @DESC: Implementa configLeerString para el proceso mapa.
 */
char * _mapa_configLeerString (t_config * archivoConfig, char nombreDeLaPropiedad[50]);

/*
 * @NAME: _mapa_newConfigType
 * @DESC: Implementa _newConfigType para el proceso mapa.
 */
t_config * _mapa_newConfigType (char * directorio);


#endif /* LIB_MAPACONFIG_H_ */

/*
 * entrenadorConfig.h
 *
 *  Created on: 20/9/2016
 *      Author: utnso
 */

#ifndef LIB_ENTRENADORCONFIG_H_
#define LIB_ENTRENADORCONFIG_H_



#include <so/libConfig.h>
#include <stdlib.h>
#include <commons/string.h>
#include "estructurasEntrenador.h"



/*
 * @NAME: _entrenador_configLeerInt
 * @DESC: Implementa configLeerInt para el proceso entrenador.
 */
uint16_t _entrenador_configLeerInt (t_config * archivoConfig, char nombreDeLaPropiedad[50], t_entrenadorFisico * unEntrenador, void (*fc) (t_entrenadorFisico *));

/*
 * @NAME: _entrenador_configLeerString
 * @DESC: Implementa configLeerString para el proceso entrenador.
 */
char * _entrenador_configLeerString (t_config * archivoConfig, char nombreDeLaPropiedad[50], t_entrenadorFisico * unEntrenador, void (*fc) (t_entrenadorFisico *));

/*
 * @NAME: _entrenador_newConfigType
 * @DESC: Implementa _newConfigType para el proceso entrenador.
 */
t_config * _entrenador_newConfigType (char * directorio, t_entrenadorFisico * unEntrenador, void (*fc) (t_entrenadorFisico *));

/*
 * @NAME: _entrenador_configLeerArray
 * @DESC: Implementa configLeerArray para el proceso entrenador.
 *  	ejemplo de uso
 *
 *
 *  int i = 0;
 *  while (miVariable[i] != NULL) {
 *  printf("%s\n", ((miVariable[i])));
 *  i++;
 *  }
 *
 */
char ** _entrenador_configLeerArray (t_config * archivoConfig, char nombreDeLaPropiedad[50], t_entrenadorFisico * unEntrenador, void (*fc) (t_entrenadorFisico *));




#endif /* LIB_ENTRENADORCONFIG_H_ */

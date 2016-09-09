/*
 * libConfig.h
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#ifndef LIBCONFIG_H_
#define LIBCONFIG_H_


//----------- Sector Include ---------------//

#include <stdlib.h>
#include <stdint.h>
#include <string.h>



//agreagado de librerias utn.so
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>

//agregado de librerias para leer directorios
#include <sys/types.h>
#include <dirent.h>		/* "readdir" etc. are defined here. */
#include <limits.h>		/* limits.h defines "PATH_MAX". */


//nuestras librerias
#include "libGrafica.h"

//------------------------------------------//



//------------------------------------------//
/* ********************************************	*/
//----------- Sector Estructuras -------------//

extern t_log* myArchivoDeLog;

//------------------------------------------//


//------------------------------------------//
/* ********************************************	*/
//----------- Sector Funciones -------------//

/*
 * @NAME: metadata_finalizar
 * @DESC: borra toda la metadata de un archivo de configuracion (tipo: t_config)
 */
void metadata_finalizar (t_config *unArchivo);


/*
 * @NAME: configLeerInt
 * @DESC: Lee un valor de tipo Int en el archivo de config
 */
uint16_t configLeerInt (t_config * archivoConfig, char nombreDeLaPropiedad[50]);



/*
 * @NAME: configLeerString
 * @DESC: Lee un valor de tipo String en el archivo de config
 */
char * configLeerString (t_config * archivoConfig, char nombreDeLaPropiedad[50]);


/*
 * @NAME: newConfigType
 * @DESC: Recibe la ubicacion de un archivo del tipo de configuracion y devuelve la estructura creada (tipo: t_config)
 */
t_config * newConfigType (char * direccionArchivo);



/*
 * @NAME: buscamePokeNestEnEsteDirectorio
 * @DESC: Recibe un directorio inicial (nombreDirectorio), y busca en todos los subdirectorios el (nombreArchivo) y llama a la funcion levantarConfigPokeNest con la ubicacion de ese archivo.
 *
 *
 * Se baso en estas webs:
 * 	//http://www.lemoda.net/c/recursive-directory/
 *	//https://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html
 *
 *
 */
void buscamePokeNestEnEsteDirectorio (  const char * nombreDirectorio, void (*fc) (const char *, const char *)  );



//------------------------------------------//



#endif /* LIBCONFIG_H_ */

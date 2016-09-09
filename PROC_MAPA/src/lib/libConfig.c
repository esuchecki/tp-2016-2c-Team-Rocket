/*
 * libConfig.c
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#include "libConfig.h"






void metadata_finalizar (t_config *unArchivo)
{
	//TODO: borrar todo...
	log_info(myArchivoDeLog, "Se borro una estructura de configuracion");
	config_destroy (unArchivo);
}


uint16_t configLeerInt (t_config * archivoConfig, char nombreDeLaPropiedad[50])
{
	//leo ints
	if (config_has_property(archivoConfig, nombreDeLaPropiedad))
	{
		return config_get_int_value(archivoConfig, nombreDeLaPropiedad);
	}
	else
	{
		//TODO: errorSintacticoSemantico no se pudo levantar el archivo config
		finalizarGui(NULL);
		exit(EXIT_FAILURE);
	}
}


char * configLeerString (t_config * archivoConfig, char nombreDeLaPropiedad[50])
{
	//leo strings
	if (config_has_property(archivoConfig, nombreDeLaPropiedad))
	{
		return config_get_string_value(archivoConfig, nombreDeLaPropiedad);
	}
	else
	{
		//TODO: errorSintacticoSemantico no se pudo levantar el archivo config
		finalizarGui(NULL);
		exit(EXIT_FAILURE);
	}
}

t_config * newConfigType (char * directorio)
{
	t_config * newConfigType = config_create(directorio);

	if (newConfigType == NULL || config_keys_amount(newConfigType) < 0 )
	{
		//TODO: errorSintacticoSemantico no se pudo levantar el archivo config
		finalizarGui(NULL);
		exit(EXIT_FAILURE);
	}

	return newConfigType;

}




void buscamePokeNestEnEsteDirectorio (  const char * nombreDirectorio, void (*fc) (const char *, const char *)  )
{


	DIR * d;
    d = opendir (nombreDirectorio);
	//Reviso si lo pudo abrir
    if (d == NULL)
	{
		//TODO: loguear error no se pudo abrir el directorio.
    	//puts (nombreDirectorio);
    	finalizarGui(NULL);
	}

	while (1)
	{
		struct dirent * entry;
		const char * d_name;

		//leemos el contenido del directorio d.
	   	entry = readdir (d);
	   	if (entry == NULL)
	   	{
	   		//No hay mas entradas
	       	break;
		}

	   	d_name = entry->d_name;
	   	/*
	   	//reviso que este directorio que contiene las pokenest, tenga un archivo de "metadata"
	   	if ( (strcmp (d_name, nombreArchivoMetadata )) == 0 )
	   		levantarConfigPokeNest( (char *)nombreDirectorio, nuevoMapa );
	   	//El archivo actual esta en nombreDirectorio/d_name
		*/
	   	(*fc) (d_name, nombreDirectorio);

	   	//solo listo los directorios (tipo=DT_DIR)
        if (entry->d_type & DT_DIR)
        {
        	//reviso que no sean los directorios "." y ".."

        	if ( ( strcmp (d_name, "..") != 0 ) && (strcmp (d_name, ".") != 0) )
        	{
        		int path_length;
	           	char path[PATH_MAX];

	           	path_length = snprintf (path, PATH_MAX, "%s/%s", nombreDirectorio, d_name);

	           	//printf ("%s\n", path);	cargarPokenest


	            if (path_length >= PATH_MAX)
	            {
	               	fprintf (stderr, "Path length has got too long.\n");
	               	finalizarGui(NULL);
	            }

	            //recorremos recursivamente archivos dentro de este directorio
	            buscamePokeNestEnEsteDirectorio (path,(void *) fc);
        	}
		}
	}
	//cerramos el directorio
	if (closedir (d))
	{
		//fprintf (stderr, "Could not close '%s': %s\n",
		//         dir_name, strerror (errno));
		fprintf (stderr, "Error al cerrar un directorio.\n");
		finalizarGui(NULL);
	}

}


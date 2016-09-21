/*
 * libConfig.c
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#include "../so/libConfig.h"






void metadata_finalizar (t_config *unArchivo)
{
	//TODO: borrar todo...
	//log_info(myArchivoDeLog, "Se borro una estructura de configuracion");
	config_destroy (unArchivo);
}


uint16_t configLeerInt (t_config * archivoConfig, char nombreDeLaPropiedad[50], bool * devolvioError)
{
	//leo ints
	if (config_has_property(archivoConfig, nombreDeLaPropiedad))
	{
		*devolvioError = false;
		return config_get_int_value(archivoConfig, nombreDeLaPropiedad);
	}
	else
	{
		*devolvioError = true;
		return 0;
	}
}


char * configLeerString (t_config * archivoConfig, char nombreDeLaPropiedad[50])
{
	//leo strings
	if (config_has_property(archivoConfig, nombreDeLaPropiedad))
		return config_get_string_value(archivoConfig, nombreDeLaPropiedad);
	else
		return NULL;
}

t_config * newConfigType (char * directorio)
{
	t_config * newConfigType = config_create(directorio);

	if (newConfigType == NULL || config_keys_amount(newConfigType) < 1 )
		return NULL;

	return newConfigType;

}

char ** configLeerArray (t_config * archivoConfig, char nombreDeLaPropiedad[50])
{
	//leo un array
	 if (config_has_property(archivoConfig, nombreDeLaPropiedad))
	 {
		 char** contenido = config_get_array_value(archivoConfig, nombreDeLaPropiedad);
		 return contenido;
	 }
	 else
	 {
		 return NULL;
	 }
}



int encontrarEnUnDirectorio (  const char * nombreDirectorio, void (*fc) (const char *, const char *)  )
{


	DIR * d;
    d = opendir (nombreDirectorio);
	//Reviso si lo pudo abrir
    if (d == NULL)
	{
		//TODO: loguear error no se pudo abrir el directorio.
    	return 0;
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
	               	//fprintf (stderr, "Path length has got too long.\n");
	            	return 0;
	            }

	            //recorremos recursivamente archivos dentro de este directorio, si hubo error aborta.
	            if ( (encontrarEnUnDirectorio (path,(void *) fc)) == 0)
	            	return 0;
        	}
		}
	}
	//cerramos el directorio
	if (closedir (d))
	{
    	return 0;
	}


	//todo correcto..
	return 1;

}


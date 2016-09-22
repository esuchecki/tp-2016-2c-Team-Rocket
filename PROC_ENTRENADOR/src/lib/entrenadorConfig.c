/*
 * entrenadorConfig.c
 *
 *  Created on: 20/9/2016
 *      Author: utnso
 */

#include "entrenadorConfig.h"

uint16_t _entrenador_configLeerInt (t_config * archivoConfig, char nombreDeLaPropiedad[50], t_entrenadorFisico * unEntrenador, void (*fc) (t_entrenadorFisico *))
{
	uint16_t aux;
	bool * devolvioError = malloc (sizeof(bool));
	aux = (uint16_t) configLeerInt (archivoConfig, nombreDeLaPropiedad, devolvioError);

	if ( *devolvioError == true )
	{
		log_error(myArchivoDeLog,"Problemas en _entrenador_configLeerInt= %s", nombreDeLaPropiedad);
		free (devolvioError);
		fc(unEntrenador);	//fc para finalizar el entrenador ante un error.
		return 0;
	}
	else
	{
		free (devolvioError);
		return aux;
	}
}

char * _entrenador_configLeerString (t_config * archivoConfig, char nombreDeLaPropiedad[50], t_entrenadorFisico * unEntrenador, void (*fc) (t_entrenadorFisico *))
{
	if ( configLeerString (archivoConfig, nombreDeLaPropiedad) == NULL )
	{
		log_error(myArchivoDeLog,"Problemas en _entrenador_configLeerString= %s", nombreDeLaPropiedad);
		//free (aux);
		fc(unEntrenador);	//fc para finalizar el entrenador ante un error.
		return 0;
	}
	else
	{
		char * aux;
		aux = string_duplicate( configLeerString (archivoConfig, nombreDeLaPropiedad) );
		return aux;
	}
}

t_config * _entrenador_newConfigType (char * directorio, t_entrenadorFisico * unEntrenador, void (*fc) (t_entrenadorFisico *))
{
	t_config * aux;
	aux = newConfigType (directorio);

	if ( aux == NULL )
	{
		log_error(myArchivoDeLog,"Problemas en _entrenador_newConfigType= %s", directorio);
		free (aux);
		fc(unEntrenador);	//fc para finalizar el entrenador ante un error.
		return 0;
	}
	else
	{
		//free (aux);
		return aux;
	}
}

char ** _entrenador_configLeerArray (t_config * archivoConfig, char nombreDeLaPropiedad[50], t_entrenadorFisico * unEntrenador, void (*fc) (t_entrenadorFisico *))
{
	char ** aux1 = configLeerArray (archivoConfig, nombreDeLaPropiedad);

	if ( aux1 == NULL )
	{
		log_error(myArchivoDeLog,"Problemas en _entrenador_configLeerArray= %s", nombreDeLaPropiedad);
		free (aux1);
		fc(unEntrenador);	//fc para finalizar el entrenador ante un error.
		return 0;
	}
	else
	{
		char ** aux2 = malloc (sizeof(aux1));
		//aux2 = string_duplicate(aux1);
		int i = 0;
		while (aux1[i] != NULL)
		{
			aux2[i] = string_duplicate (aux1[i]);
			i++;
		}
		aux2[i]=NULL;	//Luego de que termino la ultima iteracion, le copio el NULL al final
		free (aux1);
		return aux2;
	}
}


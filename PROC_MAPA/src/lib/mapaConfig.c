/*
 * mapaConfig.c
 *
 *  Created on: 14/9/2016
 *      Author: utnso
 */

#include "mapaConfig.h"
#include "libGrafica.h"

uint16_t _mapa_configLeerInt (t_config * archivoConfig, char nombreDeLaPropiedad[50])
{
	uint16_t aux;
	bool * devolvioError = malloc (sizeof(bool));
	aux = (uint16_t) configLeerInt (archivoConfig, nombreDeLaPropiedad, devolvioError);

	if ( *devolvioError == true )
	{

		//TODO: resolver el warning de finalizarGui (en todo este archivo).
		//resuelto, habia que incluir libGrafica.h porque sino no la reconocia la funcion
		free (devolvioError);
		finalizarGui(NULL);
		return 0;
	}
	else
	{
		free (devolvioError);
		return aux;
	}
}

char * _mapa_configLeerString (t_config * archivoConfig, char nombreDeLaPropiedad[50])
{
	char * aux;
	aux = string_duplicate( configLeerString (archivoConfig, nombreDeLaPropiedad) );

	if ( aux == NULL )
	{
		free (aux);
		finalizarGui(NULL);
		return 0;
	}
	else
	{
		//free (aux);
		return aux;
	}
}

t_config * _mapa_newConfigType (char * directorio)
{
	t_config * aux;
	aux = newConfigType (directorio);

	if ( aux == NULL )
	{
		free (aux);
		finalizarGui(NULL);
		return 0;
	}
	else
	{
		//free (aux);
		return aux;
	}
}

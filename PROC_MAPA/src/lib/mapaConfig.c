/*
 * mapaConfig.c
 *
 *  Created on: 14/9/2016
 *      Author: utnso
 */

#include "mapaConfig.h"


uint16_t _mapa_configLeerInt (t_config * archivoConfig, char nombreDeLaPropiedad[50], t_mapa * unMapa, void (*fc) (t_mapa *))
{
	uint16_t aux;
	bool * devolvioError = malloc (sizeof(bool));
	aux = (uint16_t) configLeerInt (archivoConfig, nombreDeLaPropiedad, devolvioError);

	if ( *devolvioError == true )
	{
		free (devolvioError);
		fc(unMapa);	//fc para finalizar la gui ante un error.
		return 0;
	}
	else
	{
		free (devolvioError);
		return aux;
	}
}

char * _mapa_configLeerString (t_config * archivoConfig, char nombreDeLaPropiedad[50], t_mapa * unMapa, void (*fc) (t_mapa *))
{
	char * aux;
	aux = string_duplicate( configLeerString (archivoConfig, nombreDeLaPropiedad) );

	if ( aux == NULL )
	{
		free (aux);
		fc(unMapa);	//fc para finalizar la gui ante un error.
		return 0;
	}
	else
	{
		//free (aux);
		return aux;
	}
}

t_config * _mapa_newConfigType (char * directorio, t_mapa * unMapa, void (*fc) (t_mapa *))
{
	t_config * aux;
	aux = newConfigType (directorio);

	if ( aux == NULL )
	{
		free (aux);
		fc(unMapa);	//fc para finalizar la gui ante un error.
		return 0;
	}
	else
	{
		//free (aux);
		return aux;
	}
}

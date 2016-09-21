/*
 * libConfigStruct.c
 *
 *  Created on: 20/9/2016
 *      Author: utnso
 */


#include "libConfigStruct.h"

void borrarEntrenador (t_entrenadorFisico * nuevoEntrenador)
{
	if (nuevoEntrenador == NULL)
		return;

	//TODO:...
}

t_entrenadorFisico * inicializarEstructurasDelEntrenador (char *nombreEntrenador, char *directorioPokeDex)
{
	t_entrenadorFisico * nuevoEntrenador = malloc(sizeof(t_entrenadorFisico));	//pedir malloc
	nuevoEntrenador->nombre = nombreEntrenador;
	nuevoEntrenador->directorioPokeDex = directorioPokeDex;

	leerMetadataDelEntrenador (nuevoEntrenador);
	//leerRecursivamenteLasPokenest (nuevoMapa);


	return nuevoEntrenador;
}

void finalizarEntrenador (t_entrenadorFisico * nuevoEntrenador)
{
	log_info(myArchivoDeLog, "voy a finalizar al entrenador");
	borrarEntrenador (nuevoEntrenador);
	log_destroy(myArchivoDeLog);
	exit(EXIT_FAILURE);
}

void leerMetadataDelEntrenador (t_entrenadorFisico * nuevoEntrenador)
{
    log_info(myArchivoDeLog,"Voy a leer la metadata del Entrenador.");
	t_config *metadataEntrenador;		//tiene info sobre el archivo config "metadata".
	metadataEntrenador = config_create_for_metadataEntrenador(nuevoEntrenador);
	t_metadataEntrenador * nuevaMetadataEntrenador = malloc(sizeof(t_metadataEntrenador));	//pedir malloc

	nuevaMetadataEntrenador->reintentos = _entrenador_configLeerInt(metadataEntrenador, __nombreEnConfig_Reintentos, nuevoEntrenador, (void *) finalizarEntrenador);
	nuevaMetadataEntrenador->vidas = _entrenador_configLeerInt(metadataEntrenador, __nombreEnConfig_Vidas, nuevoEntrenador, (void *) finalizarEntrenador);
	//nuevaMetadataEntrenador->algoritmo = _mapa_configLeerString(metadataMapa, __nombreEnConfig_Algoritmo, nuevoMapa, (void *) finalizarGui);
	//nuevaMetadataEntrenador->quantum = _mapa_configLeerInt(metadataMapa, __nombreEnConfig_Quantum, nuevoMapa, (void *) finalizarGui);
	//nuevaMetadataEntrenador->retardo = _mapa_configLeerInt(metadataMapa, __nombreEnConfig_Retardo, nuevoMapa, (void *) finalizarGui);
	//nuevaMetadataEntrenador->ip = _mapa_configLeerString(metadataMapa, __nombreEnConfig_IP, nuevoMapa, (void *) finalizarGui);
	//nuevaMetadataEntrenador->puerto = _mapa_configLeerString(metadataMapa, __nombreEnConfig_Puerto, nuevoMapa, (void *) finalizarGui);


	nuevoEntrenador->metadata = nuevaMetadataEntrenador;

	metadata_finalizar (metadataEntrenador);
	loguearEstructuraDelEntrenador(nuevoEntrenador);

	log_info(myArchivoDeLog,"Finaliza la lectura de metadata del mapa.");
}



t_config *config_create_for_metadataEntrenador(t_entrenadorFisico * nuevoEntrenador)
{
	char * directorioEntrenador;
	directorioEntrenador = malloc ( (sizeof (char)) * PATH_MAX +1);

	sprintf(directorioEntrenador, "/%s/%s/%s/%s", nuevoEntrenador->directorioPokeDex, __ubicacionEntrenadores, nuevoEntrenador->nombre, __ubicacionMetadataEntrenadores);


	t_config * metadataEntrenador = _entrenador_newConfigType(directorioEntrenador, nuevoEntrenador, (void *) finalizarEntrenador);
	free (directorioEntrenador);


	return metadataEntrenador;
}

void loguearEstructuraDelEntrenador(t_entrenadorFisico * nuevoEntrenador)
{
	log_info(myArchivoDeLog,"Vidas= %s", string_itoa(nuevoEntrenador->metadata->vidas) );
	log_info(myArchivoDeLog,"Reintentos= %s", string_itoa(nuevoEntrenador->metadata->reintentos ));
	//log_info(myArchivoDeLog,"Algoritmo= %s", nuevoMapa->metadata->algoritmo );
	//log_info(myArchivoDeLog,"Quantum= %s", string_itoa(nuevoMapa->metadata->quantum) );
	//log_info(myArchivoDeLog,"Retardo= %s", string_itoa(nuevoMapa->metadata->retardo) );
	//log_info(myArchivoDeLog,"IP= %s", nuevoMapa->metadata->ip );
	//log_info(myArchivoDeLog,"Puerto= %s", nuevoMapa->metadata->puerto );
}

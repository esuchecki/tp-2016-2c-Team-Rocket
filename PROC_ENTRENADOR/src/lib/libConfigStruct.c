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



	//list_clean(nuevoEntrenador->metadata->hojaDeViaje);
	if (nuevoEntrenador->metadata->hojaDeViaje != NULL)
		list_clean_and_destroy_elements(nuevoEntrenador->metadata->hojaDeViaje, free);
	//list_destroy(nuevoEntrenador->metadata->hojaDeViaje);

	free(nuevoEntrenador->metadata);
	free(nuevoEntrenador);
	//free(nuevoEntrenador->directorioPokeDex);
	//free(nuevoEntrenador->nombre);


	//TODO:...
}

t_entrenadorFisico * inicializarEstructurasDelEntrenador (char *nombreEntrenador, char *directorioPokeDex)
{
	t_entrenadorFisico * nuevoEntrenador = malloc(sizeof(t_entrenadorFisico));	//pedir malloc
	nuevoEntrenador->nombre = nombreEntrenador;
	nuevoEntrenador->directorioPokeDex = directorioPokeDex;
	nuevoEntrenador->metadata = NULL;

	leerMetadataDelEntrenador (nuevoEntrenador);

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

	nuevoEntrenador->metadata = nuevaMetadataEntrenador;
	nuevoEntrenador->metadata->hojaDeViaje = NULL;

	nuevaMetadataEntrenador->reintentos = _entrenador_configLeerInt(metadataEntrenador, __nombreEnConfig_Reintentos, nuevoEntrenador, (void *) finalizarEntrenador);
	nuevaMetadataEntrenador->vidas = _entrenador_configLeerInt(metadataEntrenador, __nombreEnConfig_Vidas, nuevoEntrenador, (void *) finalizarEntrenador);
	nuevaMetadataEntrenador->simbolo = (_entrenador_configLeerString(metadataEntrenador, __nombreEnConfig_Simbolo, nuevoEntrenador, (void *) finalizarEntrenador))[0];

	inicializarHojaDeViaje(metadataEntrenador, nuevoEntrenador);


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
	log_info(myArchivoDeLog,"Nombre= %s", nuevoEntrenador->nombre );
	log_info(myArchivoDeLog,"Simbolo= %c", nuevoEntrenador->metadata->simbolo );
	log_info(myArchivoDeLog,"Vidas= %s", string_itoa(nuevoEntrenador->metadata->vidas) );
	log_info(myArchivoDeLog,"Reintentos= %s", string_itoa(nuevoEntrenador->metadata->reintentos ));

	//Nota: Esto se podria haber logueado mientras se creaba, pero asi chequeamos que este ok.
	log_info(myArchivoDeLog,"...Hoja de Viaje...");

	list_iterate(nuevoEntrenador->metadata->hojaDeViaje, (void*) loguearHojaDeViajeDeUnMapa);
}

void inicializarHojaDeViaje (t_config *metadataEntrenador, t_entrenadorFisico * miEntrenador)
{
	//TODO: consultar si tengo que validar que entre los objetivos se pueda conectar 2 veces al mismo mapa...
	char ** aQueMapasMeTengoQueConectar;
	/* Ejemplo:
	 *     VALORES=[1,2,3,4,5]
	 *     El array que devuelve termina en NULL
	*/

	aQueMapasMeTengoQueConectar = _entrenador_configLeerArray(metadataEntrenador, __nombreEnConfig_HojaViajes, miEntrenador, (void *) finalizarEntrenador);

	t_list * hojaDeViaje = list_create();
	if (!(list_is_empty(hojaDeViaje)))
	{
		log_error(myArchivoDeLog,"Error al inicializar la lista 'hojaDeViaje'.");
		finalizarEntrenador(miEntrenador);
	}

	int i = 0;
	while (aQueMapasMeTengoQueConectar[i] != NULL)
	{
		//printf("%s\n", aQueMapasMeTengoQueConectar[i]);
		//TODO: validar que si hay 2 pokenest del mismo tipo seguidas es un error sintactico.
		list_add(hojaDeViaje, crearNuevoNodo(aQueMapasMeTengoQueConectar[i], metadataEntrenador, miEntrenador));
		i++;
	}

	miEntrenador->metadata->hojaDeViaje = hojaDeViaje;
	//free(aQueMapasMeTengoQueConectar);

}

t_mapa * crearNuevoNodo (char * aQueMapasMeTengoQueConectar, t_config *metadataEntrenador, t_entrenadorFisico * miEntrenador)
{
	t_mapa * nuevoNodo = malloc (sizeof(t_mapa));
	nuevoNodo->nombreMapa = aQueMapasMeTengoQueConectar;


	char * keyObjMapa= malloc(200 * sizeof(char));
	sprintf(keyObjMapa, __nombreEnConfig_ObjMapa, aQueMapasMeTengoQueConectar);

	char ** objetivoDeEsteMapa;
	objetivoDeEsteMapa = _entrenador_configLeerArray(metadataEntrenador, keyObjMapa, miEntrenador, (void *) finalizarEntrenador);

	nuevoNodo->objetivosDelMapa = objetivoDeEsteMapa;

	//free (keyObjMapa);

	return nuevoNodo;
}

void loguearHojaDeViajeDeUnMapa (t_mapa * nodoActual)
{
	log_info(myArchivoDeLog,"\t\tMapa= %s", nodoActual->nombreMapa);
	log_info(myArchivoDeLog,"\t\tObjetivos:");


	char * objetivosConcatenados = malloc(200 * sizeof(char));
	objetivosConcatenados[0] = '\0';	//me aseguro que el string este vacio para poder concatenarlo..

	int i=0;
	while (nodoActual->objetivosDelMapa[i] != NULL)
	{
		sprintf(objetivosConcatenados, "%s,%s", objetivosConcatenados, nodoActual->objetivosDelMapa[i]);
		i++;
	}
	log_info(myArchivoDeLog,"\t\t[%s]", objetivosConcatenados);
	free(objetivosConcatenados);
}

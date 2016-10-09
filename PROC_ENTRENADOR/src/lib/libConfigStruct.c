/*
 * libConfigStruct.c
 *
 *  Created on: 20/9/2016
 *      Author: utnso
 */

#include "libConfigStruct.h"

/*
 * @NAME: borrarEntrenador
 * @DESC: libera de memoria la estructura de un entrenador (uso interno)
 */
void borrarEntrenador();

/*
 * @NAME: leerMetadataDelEntrenador
 * @DESC: Cargar la parte de t_metadata de un entrenador (uso interno)
 */
void leerMetadataDelEntrenador(t_entrenadorFisico * nuevoEntrenador);

/*
 * @NAME: config_create_for_metadataEntrenador
 * @DESC: Crea el archivo de config de un entrenador (uso interno)
 */
t_config *config_create_for_metadataEntrenador(
		t_entrenadorFisico * nuevoEntrenador);

/*
 * @NAME: inicializarHojaDeViaje
 * @DESC: Se encarga de cargar la hoja de viaje en memoria (uso interno)
 */
void inicializarHojaDeViaje(t_config *metadataEntrenador,
		t_entrenadorFisico * miEntrenador);

/*
 * @NAME: crearNuevoNodo
 * @DESC: Funcion auxiliar para inicializar la hoja de viajes.  (uso interno)
 */
t_mapa * crearNuevoNodo(char * aQueMapasMeTengoQueConectar,
		t_config *metadataEntrenador, t_entrenadorFisico * miEntrenador);

/*
 * @NAME: loguearHojaDeViajeDeUnMapa
 * @DESC: Loguea exclusivamente la hoja de viajes en un log
 */
void loguearHojaDeViajeDeUnMapa(t_mapa * nodoActual);

/*
 * @NAME: hayAlgunObjetivoDuplicado
 * @DESC: Valida si en los objetivos de un mapa pasa esto:
 * 		obj[Mapa8]=[P,P,F] (error!)
 * 		obj[Mapa8]=[P,F,P,F,P,F] (ok!)
 */
void hayAlgunObjetivoDuplicado(char ** objetivoDeEsteMapa,
		t_entrenadorFisico * miEntrenador, t_mapa * nodoActual);

/*
 * @NAME: config_create_for_metadataMapa
 * @DESC: Crea el archivo de config de un mapa (uso interno para leer IPs y Puertos)
 */
t_config *config_create_for_metadataMapa(t_entrenadorFisico * miEntrenador,
		char * nombreMapa);
//------------------------------------------//








void borrarEntrenador(t_entrenadorFisico * nuevoEntrenador) {
	if (nuevoEntrenador == NULL)
		return;

	//list_clean(nuevoEntrenador->metadata->hojaDeViaje);
	if (nuevoEntrenador->metadata->hojaDeViaje != NULL)
		list_clean_and_destroy_elements(nuevoEntrenador->metadata->hojaDeViaje, &free);
	//list_destroy(nuevoEntrenador->metadata->hojaDeViaje);

	free(nuevoEntrenador->metadata);
	free(nuevoEntrenador);
	//free(nuevoEntrenador->directorioPokeDex);
	//free(nuevoEntrenador->nombre);

	//TODO:...
}

t_entrenadorFisico * inicializarEstructurasDelEntrenador(char *nombreEntrenador,
		char *directorioPokeDex) {
	t_entrenadorFisico * nuevoEntrenador = malloc(sizeof(t_entrenadorFisico));//pedir malloc
	nuevoEntrenador->nombre = nombreEntrenador;
	nuevoEntrenador->directorioPokeDex = directorioPokeDex;
	nuevoEntrenador->metadata = NULL;
	nuevoEntrenador->moverseEnMapa = NULL;

	leerMetadataDelEntrenador(nuevoEntrenador);

	return nuevoEntrenador;
}



void finalizarEntrenador(t_entrenadorFisico * nuevoEntrenador) {
	log_info(myArchivoDeLog, "voy a finalizar al entrenador");
	borrarEntrenador(nuevoEntrenador);
	log_destroy(myArchivoDeLog);
	exit(EXIT_FAILURE);
}

void leerMetadataDelEntrenador(t_entrenadorFisico * nuevoEntrenador) {
	log_info(myArchivoDeLog, "Voy a leer la metadata del Entrenador.");
	t_config *metadataEntrenador;//tiene info sobre el archivo config "metadata".
	metadataEntrenador = config_create_for_metadataEntrenador(nuevoEntrenador);
	t_metadataEntrenador * nuevaMetadataEntrenador = malloc(
			sizeof(t_metadataEntrenador));	//pedir malloc

	nuevoEntrenador->metadata = nuevaMetadataEntrenador;
	nuevoEntrenador->metadata->hojaDeViaje = NULL;

	nuevaMetadataEntrenador->reintentos = _entrenador_configLeerInt(
			metadataEntrenador, __nombreEnConfig_Reintentos, nuevoEntrenador,
			(void *) finalizarEntrenador);
	nuevaMetadataEntrenador->vidas = _entrenador_configLeerInt(
			metadataEntrenador, __nombreEnConfig_Vidas, nuevoEntrenador,
			(void *) finalizarEntrenador);
	nuevaMetadataEntrenador->simbolo = (_entrenador_configLeerString(
			metadataEntrenador, __nombreEnConfig_Simbolo, nuevoEntrenador,
			(void *) finalizarEntrenador))[0];

	inicializarHojaDeViaje(metadataEntrenador, nuevoEntrenador);

	metadata_finalizar(metadataEntrenador);
	loguearEstructuraDelEntrenador(nuevoEntrenador);

	log_info(myArchivoDeLog, "Finaliza la lectura de metadata del mapa.");
}

t_config *config_create_for_metadataEntrenador(
		t_entrenadorFisico * nuevoEntrenador) {
	char * directorioEntrenador;
	directorioEntrenador = malloc((sizeof(char)) * PATH_MAX + 1);

	sprintf(directorioEntrenador, "/%s/%s/%s/%s",
			nuevoEntrenador->directorioPokeDex, __ubicacionEntrenadores,
			nuevoEntrenador->nombre, __ubicacionMetadataEntrenadores);

	t_config * metadataEntrenador = _entrenador_newConfigType(
			directorioEntrenador, nuevoEntrenador,
			(void *) finalizarEntrenador);
	free(directorioEntrenador);

	return metadataEntrenador;
}

void loguearEstructuraDelEntrenador(t_entrenadorFisico * nuevoEntrenador) {
	log_info(myArchivoDeLog, "Nombre= %s", nuevoEntrenador->nombre);
	log_info(myArchivoDeLog, "Simbolo= %c", nuevoEntrenador->metadata->simbolo);
	log_info(myArchivoDeLog, "Vidas= %s",
			string_itoa(nuevoEntrenador->metadata->vidas));
	log_info(myArchivoDeLog, "Reintentos= %s",
			string_itoa(nuevoEntrenador->metadata->reintentos));

	//Nota: Esto se podria haber logueado mientras se creaba, pero asi chequeamos que este ok.
	log_info(myArchivoDeLog, "...Hoja de Viaje...");

	list_iterate(nuevoEntrenador->metadata->hojaDeViaje,
			(void*) loguearHojaDeViajeDeUnMapa);
}

void inicializarHojaDeViaje(t_config *metadataEntrenador,
		t_entrenadorFisico * miEntrenador) {
	//TODO: consultar si tengo que validar que entre los objetivos se pueda conectar 2 veces al mismo mapa...
	char ** aQueMapasMeTengoQueConectar;
	/* Ejemplo:
	 *     VALORES=[1,2,3,4,5]
	 *     El array que devuelve termina en NULL
	 */

	aQueMapasMeTengoQueConectar = _entrenador_configLeerArray(
			metadataEntrenador, __nombreEnConfig_HojaViajes, miEntrenador,
			(void *) finalizarEntrenador);

	t_list * hojaDeViaje = list_create();
	if (!(list_is_empty(hojaDeViaje))) {
		log_error(myArchivoDeLog,
				"Error al inicializar la lista 'hojaDeViaje'.");
		finalizarEntrenador(miEntrenador);
	}

	int i = 0;
	while (aQueMapasMeTengoQueConectar[i] != NULL) {
		list_add(hojaDeViaje,
				crearNuevoNodo(aQueMapasMeTengoQueConectar[i],
						metadataEntrenador, miEntrenador));
		i++;
	}

	miEntrenador->metadata->hojaDeViaje = hojaDeViaje;
	log_info(myArchivoDeLog,
			"Se valido que no hubiera pokemones consecutivos duplciados.");
	//free(aQueMapasMeTengoQueConectar);

}

t_mapa * crearNuevoNodo(char * aQueMapasMeTengoQueConectar,
		t_config *metadataEntrenador, t_entrenadorFisico * miEntrenador) {
	t_mapa * nuevoNodo = malloc(sizeof(t_mapa));
	nuevoNodo->nombreMapa = aQueMapasMeTengoQueConectar;

	char * keyObjMapa = malloc(200 * sizeof(char));
	sprintf(keyObjMapa, __nombreEnConfig_ObjMapa, aQueMapasMeTengoQueConectar);

	char ** objetivoDeEsteMapa;
	objetivoDeEsteMapa = _entrenador_configLeerArray(metadataEntrenador,
			keyObjMapa, miEntrenador, (void *) finalizarEntrenador);

	nuevoNodo->objetivosDelMapa = objetivoDeEsteMapa;

	hayAlgunObjetivoDuplicado(objetivoDeEsteMapa, miEntrenador, nuevoNodo);
	//free (keyObjMapa);

	return nuevoNodo;
}

void loguearHojaDeViajeDeUnMapa(t_mapa * nodoActual) {
	log_info(myArchivoDeLog, "\t\tMapa= %s", nodoActual->nombreMapa);
	log_info(myArchivoDeLog, "\t\tObjetivos:");

	char * objetivosConcatenados = malloc(200 * sizeof(char));
	objetivosConcatenados[0] = '\0';//me aseguro que el string este vacio para poder concatenarlo..

	int i = 0;
	while (nodoActual->objetivosDelMapa[i] != NULL) {
		sprintf(objetivosConcatenados, "%s,%s", objetivosConcatenados,
				nodoActual->objetivosDelMapa[i]);
		i++;
	}
	log_info(myArchivoDeLog, "\t\t[%s]", objetivosConcatenados);
	free(objetivosConcatenados);
}

void hayAlgunObjetivoDuplicado(char ** objetivoDeEsteMapa,
		t_entrenadorFisico * miEntrenador, t_mapa * nodoActual) {
	int i = 0;
	while (objetivoDeEsteMapa[i + 1] != NULL) {
		if (*objetivoDeEsteMapa[i] == *objetivoDeEsteMapa[i + 1]) {
			log_error(myArchivoDeLog,
					"Hay 2 pokemones consecutivos identicos en los objetivos del mapa %s, objetivo %c",
					nodoActual->nombreMapa, *objetivoDeEsteMapa[i]);
			finalizarEntrenador(miEntrenador);
		}
		i++;
	}
}

void cualEsLaIpDeEsteMapa(t_entrenadorFisico * miEntrenador, char * nombreMapa,
		char ** returnedIp, char ** returnedPort)
{
	log_info(myArchivoDeLog, "Voy a averiguar la IP y Puerto del mapa: %s",
			nombreMapa);
	t_config *metadataMapa;		//tiene info sobre el archivo config "metadata".
	metadataMapa = config_create_for_metadataMapa(miEntrenador, nombreMapa);

	//returnedIp = NULL;
	//returnedPort = NULL;

	*returnedIp = _entrenador_configLeerString(metadataMapa, __nombreEnConfig_IP,
			miEntrenador, (void *) finalizarEntrenador);
	*returnedPort = _entrenador_configLeerString(metadataMapa,
			__nombreEnConfig_Puerto, miEntrenador,
			(void *) finalizarEntrenador);

	log_info(myArchivoDeLog, "IP= %s   | Puerto= %s", *returnedIp, *returnedPort);
	metadata_finalizar(metadataMapa);
}

t_config *config_create_for_metadataMapa(t_entrenadorFisico * miEntrenador,
		char * nombreMapa) {
	char * directorioMapa;
	directorioMapa = malloc((sizeof(char)) * PATH_MAX + 1);

	sprintf(directorioMapa, "/%s/%s/%s/%s", miEntrenador->directorioPokeDex,
			__ubicacionMapas, nombreMapa, __ubicacionMetadataMapas);

	t_config * metadataMapa = _entrenador_newConfigType(directorioMapa,
			miEntrenador, (void *) finalizarEntrenador);
	free(directorioMapa);

	return metadataMapa;
}



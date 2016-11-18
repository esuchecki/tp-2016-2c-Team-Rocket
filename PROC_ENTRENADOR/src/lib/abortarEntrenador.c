/*
 * abortarEntrenador.c
 *
 *  Created on: 21/10/2016
 *      Author: utnso
 */


#include "abortarEntrenador.h"
#include <commons/collections/list.h>
#include <commons/log.h>
#include <unistd.h>
#include <stdlib.h>



/*
 * @NAME: borrarEntrenador
 * @DESC: libera de memoria la estructura de un entrenador (uso interno)
 */
void borrarEntrenador(t_entrenadorFisico * nuevoEntrenador);



void cerrarConexionesActivas(t_entrenadorFisico * nuevoEntrenador);






void finalizarEntrenador(t_entrenadorFisico * nuevoEntrenador) {
	printf("***] Me estoy cerrando..\n");
	log_info(myArchivoDeLog, "voy a finalizar al entrenador");
	cerrarConexionesActivas(nuevoEntrenador);
	borrarEntrenador(nuevoEntrenador);
	log_destroy(myArchivoDeLog);
	exit(0);
}

void borrarEntrenador(t_entrenadorFisico * nuevoEntrenador) {
	if (nuevoEntrenador == NULL)
		return;

	//list_clean(nuevoEntrenador->metadata->hojaDeViaje);
	if (nuevoEntrenador->metadata != NULL)
	{
		if (nuevoEntrenador->metadata->hojaDeViaje != NULL)
			list_clean_and_destroy_elements(nuevoEntrenador->metadata->hojaDeViaje, &free);
		free(nuevoEntrenador->metadata);
	}
	free(nuevoEntrenador);
	//free(nuevoEntrenador->directorioPokeDex);
	//free(nuevoEntrenador->nombre);

	//TODO:...
}

void cerrarConexionesActivas(t_entrenadorFisico * nuevoEntrenador)
{
	//TODO:...
}

/*
 * PROC_POKEDEX_SERVIDOR.c
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "lib/conexiones.h"

int main(int argc, char*argv[]) {

	//char * Port = malloc(10);
	//TODO: Conseguir el puerto de escucha por archivo de config

	atenderConexiones();

	return 0;
}

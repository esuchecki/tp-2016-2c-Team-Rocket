/*
 ============================================================================
 Name        : PROC_ENTRENADOR.c
 Author      : TEAM ROCKET
 Version     :
 Copyright   : TEAM ROCKET
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//agreagado de librerias utn.so
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/string.h>
#include <so/libSockets.h>
#include <so/libPlanificador.h>
#include <so/libConfig.h>
#include <unistd.h>

/*
 * @NAME: levantarConfig
 * @DESC: levanta la config de entrenador.
 * void levantarConfig(void);
 */

int main() {
	//TODO: subo este codigo para que no se pierda, hay que aprolijar... Disculpen chicos!
//	levantarConfig();

	int socket = connect_to("localhost", "7400");
	if (socket == -1) {
		printf("No se pudo conectar");
		exit(EXIT_FAILURE);
	} else {
		printf("socket: %d\n", socket);
		int simbolo = 0;
		t_data * info = pedirPaquete(99, 0, &simbolo);

		common_send(socket, info);

		while (1) {
			info = leer_paquete(socket);

			switch (info->header) {
			case 1:
				printf("Es tu turno\n");
				t_data * envio = pedirPaquete(10, 0, NULL);
				common_send(socket, envio);
				break;
			case 2:
				printf("mensajeMapa: %s\n", (char *) info->data);
				break;
			}
		}
	}
	close(socket);
	return EXIT_SUCCESS;
}

/*void levantarConfig() {
 char directorioPokeDex[250] =
 "/home/utnso/git/tp-2016-2c-Team-Rocket/PROC_ENTRENADOR/test_files/";

 //TODO: chequear la longitud del string, por ejemplo si es de 10 y le ingreso 15 tengo problemas...
 char directorioMapa[250];
 strcpy(directorioMapa, directorioPokeDex);

 //TODO: usar alguna v.global o preprocesador para esta config.
 //TODO: investigar string_from_format
 strcat(directorioMapa, "/Entrenadores/");
 strcat(directorioMapa, "Ash");
 strcat(directorioMapa, "/metadata");

 t_config *metadataEntrenador;
 metadataEntrenador = config_create(directorioMapa);

 if (metadataEntrenador == NULL
 || config_keys_amount(metadataEntrenador) < 0) {
 //TODO: errorSintacticoSemantico no se pudo levantar el archivo config
 exit(EXIT_FAILURE);
 }

 //leo strings
 if (config_has_property(metadataEntrenador, "nombre")) {
 char nombre[50];
 strcpy(nombre, (config_get_string_value(metadataEntrenador, "nombre")));
 printf("Mi nombre: %s\n", nombre);

 } else {
 //TODO: errorSintacticoSemantico no se pudo levantar el archivo config
 exit(EXIT_FAILURE);
 }

 //leo ints
 if (config_has_property(metadataEntrenador, "vidas")) {
 int vidas;
 vidas = config_get_int_value(metadataEntrenador, "vidas");
 printf("Vidas: %d\n", vidas);

 } else {
 //TODO: errorSintacticoSemantico no se pudo levantar el archivo config
 exit(EXIT_FAILURE);
 }

 //leo un array
 if (config_has_property(metadataEntrenador, "hojaDeViaje")) {
 char** numbers = config_get_array_value(metadataEntrenador,
 "hojaDeViaje");
 if (numbers == NULL) {
 //TODO: errorSintacticoSemantico no se pudo levantar el archivo config
 }

 printf("La hoja de viaje:");
 int i = 0;
 while (numbers[i] != NULL) {
 printf("%s\n", ((numbers[i])));
 i++;
 }

 string_iterate_lines(numbers, (void*) free);
 free(numbers);
 //TODO: parsear el contenido..
 } else {
 //TODO: errorSintacticoSemantico no se pudo levantar el archivo config
 exit(EXIT_FAILURE);
 }

 //TODO: liberar memoria..

 }
 */

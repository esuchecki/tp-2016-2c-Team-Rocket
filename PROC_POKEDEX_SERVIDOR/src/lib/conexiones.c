/*
 * conexiones.c
 *
 *  Created on: 26/9/2016
 *      Author: utnso
 */
#include "conexiones.h"

#include <so/libSockets.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "so/tiempos.h"
#include <commons/string.h>
//TODO: cambiar estos include de lugar!.
#include "../OSADA_FS/src/OSADA_Constants.h"
#include "../OSADA_FS/src/osada-utils/osada.h"
#include "../OSADA_FS/src/osada_functions.h"
#include "../libTeamRocket/so/libSockets.h"


void atender(int socket);
const char* getRespuestasOSADA(enum respuestasOSADA unNumero) ;
//void eliminarNodoAperturasArchivos (nodo_archivo * arch);
int posicionDelSemaforoConcurrenciaArchivo (char * path);
bool existeEsteArchivo(char * path);
void fcEspecial1 (int archivo);
void fcEspecial2 (int archivo);



void atenderConexion(int socket_conexion) {
	//Me llega un mensaje del pokedex cliente

	//printf("entro a atender conexion\t");
	t_data *paqueteEntrada = leer_paquete(socket_conexion);
	t_data *paqueteSalida;
	//printf("|\tLeyo paquete\n");
	char * var_itoa;
	char * path = paqueteEntrada->data;
	size_t size = 0;
	off_t offset = 0;
	uint32_t ultimaFecha;
	int null_data;
//	nodo_archivo * arch;
	int ubicacionSemaforo=archivoNoEncontrado;

	switch (paqueteEntrada->header) {
	case poke_solicitudReadDir:
		;
		//printf("***]->   readDir de: %s\n", path);
		log_info(logServidor, "***]->   readDir de: %s",path);

		ubicacionSemaforo = posicionDelSemaforoConcurrenciaArchivo(path);
		if (ubicacionSemaforo == archivoNoEncontrado)
		{
			paqueteSalida = pedirPaquete(poke_errorReadDir, sizeof(int), &null_data);
			common_send(socket_conexion, paqueteSalida);
			//printf("\t\tRta: %s\n", getRespuestasOSADA(poke_errorReadDir));
			log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(poke_errorReadDir) );
			break;
		}

//		arch = verificarAperturasArchivos(path);
//		if (arch == NULL)
//		{
//			paqueteSalida = pedirPaquete(poke_errorReadDir, sizeof(int), &null_data);
//			common_send(socket_conexion, paqueteSalida);
//			//printf("\t\tRta: %s\n", getRespuestasOSADA(poke_errorReadDir));
//			log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(poke_errorReadDir) );
//			break;
//		}
//
//		if(arch != NULL){
//			pthread_rwlock_rdlock(arch->sem_rw);
//		}
		if ((ubicacionSemaforo != rootPath) && (ubicacionSemaforo != ROOT_INDEX))
		{
			pthread_rwlock_rdlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );

		}

		char ** directorios = leerDirectorio(path);

		if ((ubicacionSemaforo != rootPath) && (ubicacionSemaforo != ROOT_INDEX))
		{
			pthread_rwlock_unlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );
			//phtread_rw (ubicacionSemaforo);
		}
//		if(arch != NULL){
//			pthread_rwlock_unlock(arch->sem_rw);
//		}

		int temp = 0;
		int aux = sizeof(char) * (OSADA_FILENAME_LENGTH + 1);
		char * buffer = malloc(sizeof(char));
		//char * buffer = malloc(sizeof(char) * (OSADA_FILENAME_LENGTH+1) );
		while (directorios[temp] != NULL) {
			buffer = realloc(buffer, aux * (temp + 1));
			memcpy(buffer + (aux * temp), directorios[temp],
			OSADA_FILENAME_LENGTH);
			//Me aseguro que en la ultima posicion haya un '\0'
			char a = '\0';
			memcpy(buffer + (aux * temp) + OSADA_FILENAME_LENGTH, &a,
					sizeof(char));
			temp++;
		}

		if (temp > 0)	//Tenia al menos un archivo el directorio.
		{
			paqueteSalida = pedirPaquete(poke_respuestaReadDir, aux * (temp), buffer);
			common_send(socket_conexion, paqueteSalida);
			//printf("\t\tRta, cant de archivos: %d\n", temp);
			var_itoa= string_itoa(temp);
			log_info(logServidor, "\t\tRta, cant de archivos: %s", var_itoa );
			free(var_itoa);
		} else {	//No tiene archivos.
			paqueteSalida = pedirPaquete(poke_errorReadDir, sizeof(int), &null_data);
			common_send(socket_conexion, paqueteSalida);
			//printf("\t\tRta: %s\n", getRespuestasOSADA(poke_errorReadDir));
			log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(poke_errorReadDir) );
		}

		//le pongo un NULL al final.
		//temp++;
		//buffer = realloc(buffer, sizeof(char) * (temp) * (OSADA_FILENAME_LENGTH+1) );
		//buffer[temp-1] = NULL;

//		char hola[OSADA_FILENAME_LENGTH+1];
//		strcpy(hola, directorios[0]);
		//= directorios[0];
		//sendeamos
		//size_t size;

		//libero memoria
		free(buffer);
		free(directorios);

		break;
	case poke_solicitudGetAttr:
		;

		//printf("***]->   getAttr de: %s\n", path);
		log_info(logServidor, "***]->   getAttr de: %s", path );


		ubicacionSemaforo = posicionDelSemaforoConcurrenciaArchivo(path);
		if (ubicacionSemaforo == archivoNoEncontrado)
		{
			null_data = 0;
			paqueteSalida = pedirPaquete(poke_errorGetAttr, sizeof(int), &null_data);
			common_send(socket_conexion, paqueteSalida);
			//printf("\t\tRta: %s\n", getRespuestasOSADA(poke_errorGetAttr));
			log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(poke_errorGetAttr) );
			break;
		}

//		arch = verificarAperturasArchivos(path);
//		if (arch == NULL)
//		{
//			null_data = 0;
//			paqueteSalida = pedirPaquete(poke_errorGetAttr, sizeof(int), &null_data);
//			common_send(socket_conexion, paqueteSalida);
//			//printf("\t\tRta: %s\n", getRespuestasOSADA(poke_errorGetAttr));
//			log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(poke_errorGetAttr) );
//			break;
//		}
//
//		if(arch != NULL){
//			pthread_rwlock_rdlock(arch->sem_rw);
//		}

		pthread_rwlock_rdlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );
		long* atributos = obtenerAtributos(path);
		pthread_rwlock_unlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );

//		if(arch != NULL){
//			pthread_rwlock_unlock(arch->sem_rw);
//		}

		switch (atributos[0]) {
		case REGULAR:
			;
			//size= (size_t) atributos[1];
			//Envio paquete por sockets
			ultimaFecha = obtenerUltimaModificacion(path);
			char * buffer = malloc( (sizeof(long) + sizeof(uint32_t)) *sizeof(char));
			memcpy(buffer, &atributos[1], sizeof(long));
			memcpy(buffer+ sizeof(long), &ultimaFecha, sizeof(uint32_t));
			paqueteSalida = pedirPaquete(poke_respuestaPorArchivo, sizeof(long) + sizeof(uint32_t), buffer);
			common_send(socket_conexion, paqueteSalida);

			//printf("%d", ultimaFecha);
			free(buffer);

			//Lo muestro en pantalla.
			struct tm tiempo;
			localtime_r((time_t *)&ultimaFecha, &tiempo);
			//printf("\t\tRta: %s\n", "REGULAR");
			//printf("\t\tTamanio: %ld [bytes]\n", atributos[1]);
			//printf("\t\tFecha Mod: %s", asctime(&tiempo));
			log_info(logServidor, "\t\tRta: %s", "REGULAR" );
			var_itoa= string_itoa(atributos[1]);
			log_info(logServidor, "\t\tTamanio: %s [bytes]", var_itoa );
			free(var_itoa);
			log_info(logServidor,"\t\tFecha Mod: %s", asctime(&tiempo) );
			//size = 0;
			break;
		case DIRECTORY:
			;
			ultimaFecha = obtenerUltimaModificacion(path);
			paqueteSalida = pedirPaquete(poke_respuestaPorDirectorio, sizeof(uint32_t), &ultimaFecha);
			common_send(socket_conexion, paqueteSalida);


			//Lo muestro en pantalla.
			struct tm tiempo2;
			localtime_r((time_t *)&ultimaFecha, &tiempo2);
			//printf("\t\tRta: %s\n", "DIRECTORY");
			//printf("\t\tFecha Mod: %s", asctime(&tiempo2));
			log_info(logServidor, "\t\tRta: %s", "DIRECTORY" );
			log_info(logServidor,"\t\tFecha Mod: %s", asctime(&tiempo2) );

			break;
		case DELETED:
			;
			null_data = 0;
			paqueteSalida = pedirPaquete(poke_errorGetAttr, sizeof(int), &null_data);
			common_send(socket_conexion, paqueteSalida);
			//printf("\t\tRta: %s\n", "DELETED");
			log_info(logServidor, "\t\tRta: %s", "DELETED" );
			break;
		default:
			;
			null_data = 0;
			paqueteSalida = pedirPaquete(poke_errorGetAttr, sizeof(int), &null_data);
			common_send(socket_conexion, paqueteSalida);
			//printf("\t\tRta: %s\n", getRespuestasOSADA(poke_errorGetAttr));
			log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(poke_errorGetAttr) );
			break;
		}

		free(atributos);
		break;
	case poke_crearDirectorio:
		;
		//printf("***]->   crearDir: %s\n", path);
		log_info(logServidor, "***]->   crearDir: %s", path );
		int directorio = -1;

//		arch = verificarAperturasArchivos(path);
//		if (arch != NULL)
//			pthread_rwlock_wrlock(arch->sem_rw);
//		//TODO: bloquear recursos
		directorio = crearDirectorio(path);
//		if (arch != NULL)
//			pthread_rwlock_unlock(arch->sem_rw);
//		//printf("Rta= %d\n", directorio);

		paqueteSalida = pedirPaquete(poke_respuestaCreacion, sizeof(int),
				&directorio);
		common_send(socket_conexion, paqueteSalida);
		//printf("\t\tRta: %s\n", getRespuestasOSADA(directorio));
		log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(directorio) );
		break;

	case poke_borrarDirectorio:
		;
		//printf("***]->   borrarDir: %s\n", path);
		log_info(logServidor, "***]->   borrarDir: %s", path );

		ubicacionSemaforo = posicionDelSemaforoConcurrenciaArchivo(path);
		if (ubicacionSemaforo == archivoNoEncontrado)
		{
			null_data = archivoNoEncontrado;
			paqueteSalida = pedirPaquete(poke_respuestaBorrado, sizeof(int), &null_data);
			common_send(socket_conexion, paqueteSalida);
			//printf("\t\tRta: %s\n", getRespuestasOSADA(poke_respuestaBorrado));
			log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(poke_respuestaBorrado) );
			break;
		}

//		arch = verificarAperturasArchivos(path);
//		if (arch == NULL)
//		{
//			null_data = -EAGAIN;
//			paqueteSalida = pedirPaquete(poke_respuestaBorrado, sizeof(int), &null_data);
//			common_send(socket_conexion, paqueteSalida);
//			//printf("\t\tRta: %s\n", getRespuestasOSADA(poke_respuestaBorrado));
//			log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(poke_respuestaBorrado) );
//			break;
//		}
//
//		if(arch != NULL){
//			pthread_rwlock_wrlock(arch->sem_rw);
//		}
		if ((ubicacionSemaforo != rootPath) && (ubicacionSemaforo != ROOT_INDEX))
		{
			pthread_rwlock_wrlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );

		}

		int directorioABorrar = borrarDirectorio(path);

		if ((ubicacionSemaforo != rootPath) && (ubicacionSemaforo != ROOT_INDEX))
		{
			pthread_rwlock_unlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );

		}

//		if(arch != NULL){
//			pthread_rwlock_unlock(arch->sem_rw);
//		}

		paqueteSalida = pedirPaquete(poke_respuestaBorrado, sizeof(int),&directorioABorrar);
		common_send(socket_conexion, paqueteSalida);
		//printf("\t\tRta: %s\n", getRespuestasOSADA(directorioABorrar));
		log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(directorioABorrar) );

		break;
	case poke_borrarArchivo:
		;
		//printf("***]->   borrarArchivo: %s\n", path);
		log_info(logServidor, "***]->   borrarArchivo: %s", path );

		ubicacionSemaforo = posicionDelSemaforoConcurrenciaArchivo(path);
		if (ubicacionSemaforo == archivoNoEncontrado)
		{
			null_data = archivoNoEncontrado;
			paqueteSalida = pedirPaquete(poke_respuestaBorradoArchivo, sizeof(int), &null_data);
			common_send(socket_conexion, paqueteSalida);
			//printf("\t\tRta: %s\n", getRespuestasOSADA(poke_respuestaBorradoArchivo));
			log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(poke_respuestaBorradoArchivo) );
			break;
		}

//		arch = verificarAperturasArchivos(path);
//		if (arch == NULL)
//		{
//			null_data = -EAGAIN;
//			paqueteSalida = pedirPaquete(poke_respuestaBorradoArchivo, sizeof(int), &null_data);
//			common_send(socket_conexion, paqueteSalida);
//			//printf("\t\tRta: %s\n", getRespuestasOSADA(poke_respuestaBorradoArchivo));
//			log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(poke_respuestaBorradoArchivo) );
//			break;
//		}
//
//		if(arch != NULL){
//			pthread_rwlock_wrlock(arch->sem_rw);
//		}

		if ((ubicacionSemaforo != rootPath) && (ubicacionSemaforo != ROOT_INDEX))
		{
			pthread_rwlock_wrlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );

		}

		int archivoABorrar = borrarArchivo(path);

		if ((ubicacionSemaforo != rootPath) && (ubicacionSemaforo != ROOT_INDEX))
		{
			pthread_rwlock_unlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );

		}

//		if(arch != NULL){
//			pthread_rwlock_unlock(arch->sem_rw);
//		}

		paqueteSalida = pedirPaquete(poke_respuestaBorradoArchivo, sizeof(int),
				&archivoABorrar);
		common_send(socket_conexion, paqueteSalida);
		//printf("\t\tRta: %s\n", getRespuestasOSADA(archivoABorrar));
		log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(archivoABorrar) );

		break;
	case poke_abrirArchivo:
		//este no se si va a ser un mensaje que le pida el cliente
		;
		t_data *paqueteAbrirArchivo = leer_paquete(socket_conexion);
		uint8_t tipoApertura = *((uint8_t *)paqueteAbrirArchivo->data);
		free(paqueteAbrirArchivo->data);
		free(paqueteAbrirArchivo);
		//printf("***]->   abrirArchivo: %s\n", path);
		//printf("\t\tTipoApertura: %s\n", getRespuestasOSADA(tipoApertura));
		log_info(logServidor, "***]->   abrirArchivo: %s", path );
		log_info(logServidor, "\t\tTipoApertura: %s", getRespuestasOSADA(tipoApertura) );



//TODO: mutex tabla de archivos.
//		int result = buscarArchivoPorPath(paqueteEntrada->data,false);
//		if(result == archivoNoEncontrado){
//			null_data = archivoNoEncontrado;
//			paqueteSalida = pedirPaquete(poke_respuestaApertura, sizeof(int), &null_data);
//			common_send(socket_conexion, paqueteSalida);
//			printf("\t\tRta: %s\n", getRespuestasOSADA(archivoNoEncontrado));
//
//			break;
//		}else {
			if (tipoApertura == poke_respuestaPorDirectorio)
			{
				if (buscarArchivoPorPath(path, false) != rootPath)
				{
					//TODO: mutex tabla de archivos.
					long* atributos = obtenerAtributos(path);
					if (atributos[0] != DIRECTORY)
					{
						null_data = archivoNoEncontrado;
						paqueteSalida = pedirPaquete(poke_respuestaApertura, sizeof(int), &null_data);
						common_send(socket_conexion, paqueteSalida);
						free(atributos);
						//printf("\t\tRta: %s\n", getRespuestasOSADA(archivoNoEncontrado));
						log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(archivoNoEncontrado) );
						break;
					}
					free(atributos);
				}
			}

			if (tipoApertura == poke_respuestaPorArchivo)
			{
				//TODO: mutex tabla de archivos.
				long* atributos = obtenerAtributos(path);
				if (atributos[0] != REGULAR)
				{
					null_data = archivoNoEncontrado;
					paqueteSalida = pedirPaquete(poke_respuestaApertura, sizeof(int), &null_data);
					common_send(socket_conexion, paqueteSalida);
					free(atributos);
					//printf("\t\tRta: %s\n", getRespuestasOSADA(archivoNoEncontrado));
					log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(archivoNoEncontrado) );
					break;
				}
				free(atributos);
			}

			//Si ninguno de los anteriores fue (osea ni directorio, ni archivo)
			//entonces el tipoApertura es = poke_solicitudGetAttr.

			if (existeEsteArchivo(path))
			{
//				arch = verificarAperturasArchivos(path);
//				if(arch == NULL){
//					arch = malloc(sizeof(nodo_archivo));
//					arch->sem_rw = malloc(sizeof(pthread_rwlock_t));
//					arch->ubicacionEnTablaArchivos = buscarArchivoPorPath(path, false);
//					arch->cantidadDeVecesAbierta++;
//					pthread_rwlock_init(arch->sem_rw,NULL);
//					list_add(tablaArchivosAbiertos,arch);
//				}else{
//					arch->cantidadDeVecesAbierta++;
//				}

				null_data = operacionExitosa;
				paqueteSalida = pedirPaquete(poke_respuestaApertura, sizeof(int), &null_data);
				common_send(socket_conexion, paqueteSalida);
				//printf("\t\tRta: %s\n", getRespuestasOSADA(operacionExitosa));
				log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(operacionExitosa) );
			}
			else
			{
				null_data = archivoNoEncontrado;
				paqueteSalida = pedirPaquete(poke_respuestaApertura, sizeof(int), &null_data);
				common_send(socket_conexion, paqueteSalida);
				//printf("\t\tRta: %s\n", getRespuestasOSADA(archivoNoEncontrado));
				log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(archivoNoEncontrado) );
				break;
			}
			//printf("\t\tRta: %d [bytes]\n", 0);
//		}
		break;
	case poke_cerrarArchivo:
		;
		t_data *paqueteCerrarArchivo = leer_paquete(socket_conexion);
		uint8_t tipoApertura2 = *((uint8_t *)paqueteCerrarArchivo->data);
		free(paqueteCerrarArchivo->data);
		free(paqueteCerrarArchivo);

		//printf("***]->   cerrarArchivo: %s\n", path);
		//printf("\t\tTipoCierre: %s\n", getRespuestasOSADA(tipoApertura2));
		log_info(logServidor, "***]->   cerrarArchivo: %s", path );
		log_info(logServidor, "\t\tTipoCierre: %s", getRespuestasOSADA(tipoApertura2) );



		if (tipoApertura2 == poke_respuestaPorDirectorio)
		{
			if (buscarArchivoPorPath(path, false) != rootPath)
			{
				//TODO: mutex tabla de archivos.
				long* atributos = obtenerAtributos(path);
				if (atributos[0] != DIRECTORY)
				{
					null_data = archivoNoEncontrado;
					paqueteSalida = pedirPaquete(poke_respuestaClose, sizeof(int), &null_data);
					common_send(socket_conexion, paqueteSalida);
					free(atributos);
					//printf("\t\tRta: %s\n", getRespuestasOSADA(archivoNoEncontrado));
					log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(archivoNoEncontrado) );
					break;
				}
				free(atributos);
			}
		}

		if (tipoApertura2 == poke_respuestaPorArchivo)
		{
			//TODO: mutex tabla de archivos.
			long* atributos = obtenerAtributos(path);
			if (atributos[0] != REGULAR)
			{
				null_data = archivoNoEncontrado;
				paqueteSalida = pedirPaquete(poke_respuestaClose, sizeof(int), &null_data);
				common_send(socket_conexion, paqueteSalida);
				free(atributos);
				//printf("\t\tRta: %s\n", getRespuestasOSADA(archivoNoEncontrado));
				log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(archivoNoEncontrado) );
				break;
			}
			free(atributos);
		}

		//Si ninguno de los anteriores fue (osea ni directorio, ni archivo)
		//entonces el tipoApertura es = poke_solicitudGetAttr.

		if (existeEsteArchivo(path))
		{
			//TODO: verificar que estoy cerrando ok.
//			arch = verificarAperturasArchivos(paqueteEntrada->data);
//			if(arch != NULL){
//				if (arch->cantidadDeVecesAbierta > 1)
//				{
//					arch->cantidadDeVecesAbierta--;
//				}
//				else
//				{
//					eliminarNodoAperturasArchivos(arch);
//				}
				null_data = operacionExitosa;
				paqueteSalida = pedirPaquete(poke_respuestaClose, sizeof(int), &null_data);
				common_send(socket_conexion, paqueteSalida);
				//printf("\t\tRta: %s\n", getRespuestasOSADA(operacionExitosa));
				log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(operacionExitosa) );
				break;
//			}else{
//				null_data = archivoNoEncontrado;
//				paqueteSalida = pedirPaquete(poke_respuestaClose, sizeof(int), &null_data);
//				common_send(socket_conexion, paqueteSalida);
//				//printf("\t\tRta: %s\n", getRespuestasOSADA(archivoNoEncontrado));
//				log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(archivoNoEncontrado) );
//				break;
//			}
		}
		else
		{
			null_data = archivoNoEncontrado;
			paqueteSalida = pedirPaquete(poke_respuestaClose, sizeof(int), &null_data);
			common_send(socket_conexion, paqueteSalida);
			//printf("\t\tRta: %s\n", getRespuestasOSADA(archivoNoEncontrado));
			log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(archivoNoEncontrado) );
			break;
		}


		break;
	case poke_leerArchivo:
		;
		//char * path = paquete->data;
		t_data * posiciones = leer_paquete(socket_conexion);
		memcpy(&size, posiciones->data, sizeof(size_t));
		memcpy(&offset, posiciones->data + sizeof(size_t), sizeof(off_t));

		//printf("***]->   read: %s\n", path);
		//printf("size: %zu, offset: %jd\n", size, (intmax_t) offset);
		log_info(logServidor, "***]->   read: %s", path );
		char * logearSizeOffset = malloc(100);
		sprintf(logearSizeOffset, "size: %zu, offset: %jd", size, (intmax_t) offset);
		log_info(logServidor, "%s", logearSizeOffset );
		free(logearSizeOffset);

		uint32_t tamanioCopiarSockets = 0;

		ubicacionSemaforo = posicionDelSemaforoConcurrenciaArchivo(path);
		if (ubicacionSemaforo == archivoNoEncontrado)
		{
			null_data = 0;
			paqueteSalida = pedirPaquete(poke_errorEnLectura, sizeof(int), &null_data);
			common_send(socket_conexion, paqueteSalida);
			//printf("\t\tRta: %d [bytes]\n", 0);
			log_info(logServidor, "\t\tRta: %s  [bytes]", &"0" );
		}

//		arch = verificarAperturasArchivos(path);
//		if(arch != NULL){
//			pthread_rwlock_rdlock(arch->sem_rw);
//		}
		if ((ubicacionSemaforo != rootPath) && (ubicacionSemaforo != ROOT_INDEX))
		{
			pthread_rwlock_rdlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );

		}

		osada_block* archivo = obtenerArchivoPorPath(path, size, offset, &tamanioCopiarSockets);

		if ((ubicacionSemaforo != rootPath) && (ubicacionSemaforo != ROOT_INDEX))
		{
			pthread_rwlock_unlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );

		}

//		if(arch != NULL){
//			pthread_rwlock_unlock(arch->sem_rw);
//		}
		if ( (tamanioCopiarSockets >0) )
		{
			paqueteSalida = pedirPaquete(poke_respuestaLectura, tamanioCopiarSockets , archivo);
			common_send(socket_conexion, paqueteSalida);
			//printf("\t\tRta: %d  [bytes]\n", tamanioCopiarSockets);
			var_itoa= string_itoa(tamanioCopiarSockets);
			log_info(logServidor, "\t\tRta: %s  [bytes]", var_itoa );
			free(var_itoa);
			free(archivo);
		}
		else
		{
			null_data = 0;
			paqueteSalida = pedirPaquete(poke_errorEnLectura, sizeof(int), &null_data);
			common_send(socket_conexion, paqueteSalida);
			//printf("\t\tRta: %d [bytes]\n", 0);
			log_info(logServidor, "\t\tRta: %s  [bytes]", &"0" );
		}
		free(posiciones->data);
		free(posiciones);

		break;
	case poke_escribirArchivo:
		;
		//char * path = paquete->data;
		t_data * paqueteBuffer = leer_paquete(socket_conexion);
		char *buf = malloc(paqueteBuffer->tamanio);
		memcpy(buf, paqueteBuffer->data, paqueteBuffer->tamanio);

		//printf("\n%s\n", buf);

		t_data * posiciones2 = leer_paquete(socket_conexion);
		memcpy(&size, posiciones2->data, sizeof(size_t));
		memcpy(&offset, posiciones2->data + sizeof(size_t), sizeof(off_t));

		//printf("***]->   write: %s\n", path);
		//printf("size: %zu, offset: %jd\n", size, (intmax_t) offset);
			char * logearSizeOffset2= malloc(100);
			sprintf(logearSizeOffset2, "size: %zu, offset: %jd", size, (intmax_t) offset);
		log_info(logServidor, "***]->   write: %s", path );
		log_info(logServidor, "%s", logearSizeOffset2 );
			free(logearSizeOffset2);

//		arch = verificarAperturasArchivos(path);
//		if(arch != NULL){
//			pthread_rwlock_wrlock(arch->sem_rw);
//		}
		ubicacionSemaforo = posicionDelSemaforoConcurrenciaArchivo(path);
		int resultado;
		if (ubicacionSemaforo != archivoNoEncontrado)
		{

			if ((ubicacionSemaforo != rootPath) && (ubicacionSemaforo != ROOT_INDEX))
			{
				pthread_rwlock_wrlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );

			}

			resultado = escribir(path, buf, size, offset);

			if ((ubicacionSemaforo != rootPath) && (ubicacionSemaforo != ROOT_INDEX))
			{
				pthread_rwlock_unlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );

			}

		}else{
			resultado = archivoNoEncontrado;
		}



//		if(arch != NULL){
//			pthread_rwlock_unlock(arch->sem_rw);
//		}


		paqueteSalida = pedirPaquete(poke_respuestaEscritura, sizeof(int) , &resultado);
		common_send(socket_conexion, paqueteSalida);
		//printf("\t\tRta: %s\n", getRespuestasOSADA(resultado));
		log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(resultado) );

		free(buf);
		free(paqueteBuffer->data);
		free(paqueteBuffer);
		free(posiciones2->data);
		free(posiciones2);

		break;
		//case -1:
	case poke_renombrar:
		;
		/*
		 *
		 * ESTO NO IRIA... VERSION CON BUFFER
		 int largoPath;

		 memcpy(&largoPath, paquete->data,sizeof(int));

		 char* path = malloc(largoPath);

		 memcpy(&path , paquete->data+sizeof(int), largoPath);

		 int largoName;

		 memcpy(&largoName, paquete->data + sizeof(int) + largoPath,sizeof(int));

		 char* nombre = malloc(largoName);

		 memcpy(&nombre , paquete->data+sizeof(int)+largoPath+largoName, largoName);
		 */
		t_data *paquete2 = leer_paquete(socket_conexion);
		//printf("leyo paquete2 \n");

		char* nombre = paquete2->data;
		//printf("***]->   renombrarArchivo: \n Path1: %s\n Path2:%s \n", path, nombre);
		log_info(logServidor, "***]->   renombrarArchivo: \n Path1: %s\n Path2:%s ", path, nombre );

//		arch = verificarAperturasArchivos(path);
//		if (arch == NULL)
//		{
//			null_data = -EAGAIN;
//			paqueteSalida = pedirPaquete(poke_respuestaRenombrado, sizeof(int), &null_data);
//			common_send(socket_conexion, paqueteSalida);
//			//printf("\t\tRta: %s\n", getRespuestasOSADA(poke_respuestaRenombrado));
//			log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(poke_respuestaRenombrado) );
//			break;
//		}
//		if(arch != NULL){
//			pthread_rwlock_wrlock(arch->sem_rw);
//		}

		ubicacionSemaforo = posicionDelSemaforoConcurrenciaArchivo(path);
		int renombro;
		if (ubicacionSemaforo != archivoNoEncontrado)
		{

			if ((ubicacionSemaforo != rootPath) && (ubicacionSemaforo != ROOT_INDEX))
			{
				pthread_rwlock_wrlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );

			}

			renombro = cambiarNombre(path, nombre);

			if ((ubicacionSemaforo != rootPath) && (ubicacionSemaforo != ROOT_INDEX))
			{
				pthread_rwlock_unlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );

			}

		}else{
			renombro = archivoNoEncontrado;
		}



//		int renombro = cambiarNombre(path, nombre);
//		if(arch != NULL){
//			pthread_rwlock_unlock(arch->sem_rw);
//		}

		//int renombro = cambiarNombre( lectura2->data, path);
		paqueteSalida = pedirPaquete(poke_respuestaRenombrado, sizeof(int),
				&renombro);
		common_send(socket_conexion, paqueteSalida);
		//printf("\t\tRta: %s\n", getRespuestasOSADA(renombro));
		log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(renombro) );

		free(nombre);
		free(paquete2);
		break;

	case poke_truncar:
		;
		t_data *paquete3 = leer_paquete(socket_conexion);
		offset = *((off_t *) paquete3->data);

		//printf("***]->   truncate: %s, al largo %jd\n", path, (intmax_t)offset);
			char * logearSizeOffset3 = malloc(100);
			sprintf(logearSizeOffset3, "***]->   truncate: %s, al largo %jd", path, (intmax_t)offset);
		log_info(logServidor, "%s", logearSizeOffset3 );
			free(logearSizeOffset3);

//		arch = verificarAperturasArchivos(path);
//		//En el caso de que no lo encontro, seguramente el tipo quiere crear el archivo. Entonces no semaforo
//		if (arch != NULL)
//			pthread_rwlock_wrlock(arch->sem_rw);

		//int trunco = truncar(path, (long)offset);
		int trunco = truncarConSemaforos(path, (long)offset, (void *) fcEspecial1, (void *) fcEspecial2);


//		//En el caso de que no lo encontro, seguramente el tipo quiere crear el archivo. Entonces no semaforo
//		if (arch != NULL)
//			pthread_rwlock_unlock(arch->sem_rw);


		paqueteSalida = pedirPaquete(poke_respuestaTruncado, sizeof(int),	&trunco);
		common_send(socket_conexion, paqueteSalida);
		//printf("\t\tRta: %s\n", getRespuestasOSADA(trunco));
		log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(trunco) );

		free(paquete3->data);
		free(paquete3);
		break;
	case poke_utimensat:
		;
		t_data *paqueteUtimensat = leer_paquete(socket_conexion);
		//printf("leyo paquete3 \n");
		uint32_t ultimaFecha;
		memcpy(&ultimaFecha, paqueteUtimensat->data , sizeof(uint32_t));

		//Convierto los segundos a un formato que lo pueda imprimir como string.
		struct tm tiempo;
		localtime_r((time_t *)&ultimaFecha, &tiempo);

		//printf("***]->   utimensat: %s, a: %s\n", path, asctime(&tiempo));
		log_info(logServidor, "***]->   utimensat: %s, a: %s", path, asctime(&tiempo) );

//		arch = verificarAperturasArchivos(path);
//		if (arch == NULL)
//		{
//			null_data = -EAGAIN;
//			paqueteSalida = pedirPaquete(poke_respuestaUtimensat, sizeof(int), &null_data);
//			common_send(socket_conexion, paqueteSalida);
//			//printf("\t\tRta: %s\n", getRespuestasOSADA(poke_respuestaUtimensat));
//			log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(poke_respuestaUtimensat) );
//			break;
//		}
//
//		if(arch != NULL){
//			pthread_rwlock_wrlock(arch->sem_rw);
//		}
		ubicacionSemaforo = posicionDelSemaforoConcurrenciaArchivo(path);
		int utimensat;

		if (ubicacionSemaforo != archivoNoEncontrado)
		{

			if ((ubicacionSemaforo != rootPath) && (ubicacionSemaforo != ROOT_INDEX))
			{
				pthread_rwlock_wrlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );
			}

			utimensat = establecerUltimaModificacion(path, ultimaFecha);

			if ((ubicacionSemaforo != rootPath) && (ubicacionSemaforo != ROOT_INDEX))
			{
				pthread_rwlock_unlock( &(semaforoConcurrenciaArchivo[ubicacionSemaforo]) );
			}

		}else{
			utimensat = archivoNoEncontrado;
		}


//		if(arch != NULL){
//			pthread_rwlock_unlock(arch->sem_rw);
//		}

		paqueteSalida = pedirPaquete(poke_respuestaUtimensat, sizeof(int), &utimensat);
		common_send(socket_conexion, paqueteSalida);
		//printf("\t\tRta: %s\n", getRespuestasOSADA(utimensat));
		log_info(logServidor, "\t\tRta: %s", getRespuestasOSADA(utimensat) );

		free(paqueteUtimensat->data);
		free(paqueteUtimensat);
		break;
	default:
		;
		printf("Se desconecto un cliente. pthread_t = %lu\n", (unsigned long) pthread_self());

		pthread_exit(0);
		break;
	}
	//printf("\n************** -- **************\n");
	log_info(logServidor, "************** -- **************" );
	free(path);
	free(paqueteEntrada);
	free(paqueteSalida);	//No le hago free a la data, porque no le pide malloc a eso.
}

void handshake(int socket_nueva_conexion, fd_set sockets_activos) {

	t_data * paquete = leer_paquete(socket_nueva_conexion);

	if (paquete->header == 70) {

		int null_data = 0;

		paquete = pedirPaquete(71, sizeof(int), &null_data);

		common_send(socket_nueva_conexion, paquete);

	} else {

		printf(
				"Error en la conexion con el proceso pokedex cliente, cierro programa");

		exit(EXIT_FAILURE);

	}
}

int atenderConexiones(char* ip, char* puerto) {

	pthread_mutex_init(&mutex_mayor, NULL);

	int socketEscucha, socketMasGrande;

	fd_set sockets_para_revisar, sockets_activos;

	socketEscucha = setup_listen(ip, puerto);

	printf("Estoy escuchando clientes en el socket %d\n", socketEscucha);

	listen(socketEscucha, 1024);

	socketMasGrande = socketEscucha;
	FD_ZERO(&sockets_activos);
	FD_SET(socketEscucha, &sockets_activos);

	while (1) {
		sockets_para_revisar = sockets_activos;

		int retornoSelect = select(socketMasGrande + 1, &sockets_para_revisar,
		NULL, NULL, NULL);

		if (retornoSelect == -1) {

		}
		int i;

		for (i = 0; i <= socketMasGrande; i++) {
			if (FD_ISSET(i, &sockets_para_revisar)) {
				//hay actividad
				if (i == socketEscucha) {
					//es una nueva conexion sobre el puerto de escucha

					pthread_t manejoCliente;

					pthread_attr_t attrHilo;
					pthread_attr_init(&attrHilo);
					pthread_attr_setdetachstate(&attrHilo,
					PTHREAD_CREATE_DETACHED);

					nodo_hilo * nodo = malloc(sizeof(nodo_hilo));
					nodo->hilo = manejoCliente;
					nodo->socketMasGrande = socketMasGrande;
					nodo->socketEscucha = socketEscucha;
					nodo->sockets_activos = sockets_activos;

					pthread_create(&manejoCliente, &attrHilo,
							(void *) establecerConexion, (void *) nodo);

				} else {
					//la actividad es un puerto ya enlazado, hay que atenderlo
					//atenderConexion(i, sockets_activos);

				}
			}
		}
	}
}

void establecerConexion(void * data) {

	nodo_hilo *datos = data;

	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;
	addrlen = sizeof(remoteaddr);

	int socket_nueva_conexion = accept(datos->socketEscucha,
			(struct sockaddr *) &remoteaddr, &addrlen);

	if (socket_nueva_conexion == -1) {
		//se desconecto o no conecto alguno
		printf("error al asignar socket a la nueva conexion");

	} else {
		//Ponemos al socket nuevo en el set de sockets activos
		FD_SET(socket_nueva_conexion, &datos->sockets_activos);

		pthread_mutex_lock(&mutex_mayor);
		if (socket_nueva_conexion > datos->socketMasGrande) {
			datos->socketMasGrande = socket_nueva_conexion;
		}
		pthread_mutex_unlock(&mutex_mayor);

		handshake(socket_nueva_conexion, datos->sockets_activos);

		printf("Soy una nueva conexion.\n");
	}

	while (1) {
		atenderConexion(socket_nueva_conexion);
	}
}

/*
 char * guardarConCentinela(char **paquete, int * sizeOfBuffer) {
 int i;
 char caracter;
 char centinela = '\0';

 for (i = 0; caracter != centinela; i++) {

 caracter = paquete[i];

 if (caracter == centinela) {
 i++;
 break;
 }
 }

 char * buffer = malloc(sizeof(char) * i);

 caracter = paquete[0];

 for (i = 0; caracter != centinela; i++) {
 caracter = paquete[i];
 if (caracter == centinela) {
 break;
 }
 memcpy(buffer + i, paquete + i, sizeof(char));
 }

 buffer[i] = '\0';

 return buffer;
 }
 */

//Codigo de Maxi Santos
void escucharNuevasConexiones(char* ip, char *puerto) {

	int socketEscucha = setup_listen(ip, puerto);
	int i = 0;

	printf("Estoy escuchando clientes en el socket %d\n", socketEscucha);
	listen(socketEscucha, 1024);

	pthread_t hilos[20];
	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;
	addrlen = sizeof(remoteaddr);

	//Aca abrim

	while (1) {
		int socket_nueva_conexion = accept(socketEscucha,
				(struct sockaddr *) &remoteaddr, &addrlen);
		pthread_create(&hilos[i], NULL,(void *) atender,(void*) socket_nueva_conexion);;
		//atender(socket_nueva_conexion);
		printf("Este es el cliente numero %d, pthread_t=%lu \n", i, (unsigned long)hilos[i]);
		i++;
	}
}

//Codigo de Maxi Santos
void atender(int socket) {
	nodo_hilo * nodo = malloc(sizeof(nodo_hilo));
	handshake(socket, nodo->sockets_activos);
	printf("Nuevo Socket: %d\n", socket);
	while (1) {
		atenderConexion(socket);
	}
}


const char* getRespuestasOSADA(enum respuestasOSADA unNumero)
{
   switch (unNumero)
   {
      case operacionExitosa: return "operacionExitosa";
      case finDeArchivo: return "finDeArchivo";
      case archivoInexistenteConDirCorrecto: return "archivoInexistenteConDirCorrecto";
      case rootPath: return "rootPath";
      case noHayEspacioLibreTablaArchivos: return "noHayEspacioLibreTablaArchivos";
      case elPathNoCorrespondeAUnDirectorio: return "elPathNoCorrespondeAUnDirectorio";
      case noHayBloquesLibres: return "noHayBloquesLibres";
      case bloqueOcupado: return "bloqueOcupado";
      case archivoNoEncontrado: return "archivoNoEncontrado";
      case noEsUnArchivo: return "noEsUnArchivo";
      case tamanioDeArchivoInsuficiente: return "tamanioDeArchivoInsuficiente";
      case elNombreDelArchivoEsMuyGrande: return "elNombreDelArchivoEsMuyGrande";
      case elNombreDelArchivoEsMuyCorto: return "elNombreDelArchivoEsMuyCorto";
      default:
    	  ;
    	  switch ((enum enviosPokedexServidor) unNumero)
    	  {
    	  	  case poke_errorGetAttr: return "poke_errorGetAttr";
    	  	  case poke_errorReadDir: return "poke_errorReadDir";
    	  	  case poke_respuestaPorArchivo: return "poke_respuestaPorArchivo";
    	  	  case poke_respuestaPorDirectorio: return "poke_respuestaPorDirectorio";
    	  	  //case aaaa: return "";
    	  	  default:
    	  		  ;
    	  }

    	  switch ((enum enviosPokedexCliente) unNumero)
    	  {
    	  	  case poke_solicitudGetAttr: return "poke_solicitudGetAttr";
    	  	  default:
    	  		  ;
    	  }
      	  return "noEncontreElNumeroEnElEnum";
   }
}

//nodo_archivo * verificarAperturasArchivos(char * path){
//	//char * nombreArchivo = malloc(18);
//	int ubicacionArchivo = buscarArchivoPorPath(path, false);
//
//	bool estaAbierto(void * datos){
//		return (((nodo_archivo *)datos)->ubicacionEnTablaArchivos == ubicacionArchivo);
//	}
//
//	pthread_mutex_lock(&mutex_archivos);
//	nodo_archivo * archivo = list_find(tablaArchivosAbiertos,estaAbierto);
//	pthread_mutex_unlock(&mutex_archivos);
//
//	return archivo;
//}
//
bool existeEsteArchivo(char * path)
{
	int ubicacionArchivo = buscarArchivoPorPath(path, false);
	if (ubicacionArchivo == archivoNoEncontrado)
		return false;
	return true;
}

//void eliminarNodoAperturasArchivos (nodo_archivo * arch)
//{
//	pthread_mutex_lock(&mutex_archivos);
//
//	bool esEste(void * datos){
//		return (((nodo_archivo *)datos)->ubicacionEnTablaArchivos == arch->ubicacionEnTablaArchivos);
//	}
//
//	pthread_rwlock_destroy(arch->sem_rw);
//	free(arch->sem_rw);
//	list_remove_by_condition(tablaArchivosAbiertos,esEste );
//	free(arch);
//
//
//	pthread_mutex_unlock(&mutex_archivos);
//}

void inicializarSemaforosTablaArchivos ()
{
	int i=0;
	for (i=0; i< 2048; i++)
	{
		pthread_rwlock_init( &(semaforoConcurrenciaArchivo[i]) ,NULL);
		//semaforoConcurrenciaArchivo[i]
	}

}

int posicionDelSemaforoConcurrenciaArchivo (char * path)
{
	int ubicacionArchivo = buscarArchivoPorPath(path, false);
	return ubicacionArchivo;
}

void fcEspecial1 (int archivo)
{
	if ((archivo != rootPath) && (archivo != ROOT_INDEX))
	{
		pthread_rwlock_wrlock( &(semaforoConcurrenciaArchivo[archivo]) );

	}
}


void fcEspecial2 (int archivo)
{
	if ((archivo != rootPath) && (archivo != ROOT_INDEX))
	{
		pthread_rwlock_unlock( &(semaforoConcurrenciaArchivo[archivo]) );

	}
}

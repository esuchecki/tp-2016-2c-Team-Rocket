/*
 * osada_functions.c
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */
#include "osada-utils/osada.h"
#include "OSADA_Constants.h"
#include <stdio.h>
#include <sys/mman.h>
#include <fuse.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include "osada_functions.h"
#include <so/libSockets.h>

#define ROOT_INDEX     65535

int TAMANIO_BYTES;
struct stat osadaStat;
osada_block* bloques_archivo;
osada_header* header;

int totalBloques() {
	return header->fs_blocks;
}

osada_header* obtenerHeader(){
	header = (osada_header*)bloques_archivo;
	return header;
}

int calcularBloques(int estructura) {
	int cantidad_bloques = TAMANIO_BYTES / __tamanioBloque;
	int bloques_bitmap = cantidad_bloques / 8 / __tamanioBloque;
	osada_header* header = obtenerHeader();
	int bloques_asignaciones = header->data_blocks;
	int retorno = 0;
	switch (estructura) {
	case __estructuraHeader:
		retorno = 1;
		break;
	case __estructuraBitmap:
		retorno = bloques_bitmap;
		break;
	case __estructuraTablaArchivos:
		retorno = 1024;
		break;
	case __estructuraTablaAsignaciones:
		retorno = bloques_asignaciones;
		break;
	case __estructuraBloqueDatos:
		retorno = bloques_asignaciones;
		break;
	default:
		retorno = 0;
	}
	return retorno;
}

int calcularCantidadBloques(long tamanioEnBytes){
	int esDivisor = tamanioEnBytes%OSADA_BLOCK_SIZE;
	int resultado = tamanioEnBytes / OSADA_BLOCK_SIZE;
	if(esDivisor > 0){
		resultado = resultado + 1;
	}
	return resultado;
}

void inicializarHeader(){
	if(header == NULL){
		obtenerHeader();
	}
}

int obtenerBytesBloques(int cantidadBloques) {
	return (cantidadBloques * __tamanioBloque);
}

//Retorna la cantidad de bloques que son usadas por las estructuras base
int calcularBloquesEstructurasBase(){
	int header = calcularBloques(__estructuraHeader);
	int bitmap = calcularBloques(__estructuraBitmap);
	int tArchivos = calcularBloques(__estructuraTablaArchivos);
	int tAsignaciones = calcularBloques(__estructuraTablaAsignaciones);
	int total = header + bitmap + tArchivos + tAsignaciones;
	return total;
}

void abrirArchivo(){
	int fd_osada;
	fd_osada= open(__pathArchivo,O_RDWR);
	fstat(fd_osada,&osadaStat);
	bloques_archivo= mmap(0, osadaStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_osada, 0);
}

t_bitarray* obtenerBitmap(){
	osada_header* header = obtenerHeader();
	t_bitarray* bitmap = bitarray_create( (char *) bloques_archivo[1],header->bitmap_blocks);
	return bitmap;
}

osada_file* obtenerTablaArchivos(){
	inicializarHeader();
	osada_file* tablaArchivos = (osada_file*)bloques_archivo[1 + header->bitmap_blocks];
	return tablaArchivos;
}

int* obtenerTablaAsignaciones(){
	inicializarHeader();
	int* tablaAsignaciones = (int*) &bloques_archivo[header->allocations_table_offset];
	return tablaAsignaciones;
}

osada_block* obtenerBloqueDatos(){
	inicializarHeader();
	int bloquesTablaAsignaciones = ((header->fs_blocks - header->allocations_table_offset) * 4 / OSADA_BLOCK_SIZE)+1;
	osada_block * bloqueDatos = &bloques_archivo[header->allocations_table_offset + bloquesTablaAsignaciones];
	return bloqueDatos;
}

void imprimirEstructuraArchivos(){
	osada_file* tablaArchivos = obtenerTablaArchivos();
	osada_file_state j = REGULAR;
	int i,k;
	char* guion = "-";
	for ( i = 0 ; i<2048 ; i++ ) {
	   j = tablaArchivos[i].state;
	   k = tablaArchivos[i].parent_directory;
	   if(j!=DELETED){
		   while(k!=ROOT_INDEX){
			   printf("%s", guion);
			   k = tablaArchivos[k].parent_directory;
		   }
		   printf("%s\n", tablaArchivos[i].fname);
	   }
	}
}
/*
 * Retorna -1 si el archivo no existe o
 * el indice de su posicion en la tabla
 * de archivos
 */
int buscarArchivoEnFS(char* nombre, int padre){
	int retorno = archivoNoEncontrado;
	osada_file* tablaArchivos = obtenerTablaArchivos();
	int i= 1;
	osada_file_state j = REGULAR;
	for ( i = 0 ; (retorno == archivoNoEncontrado && i<2048); i++ ) {
	   j = tablaArchivos[i].state;
	   if(j!=DELETED){
		   if((strcmp((char*)tablaArchivos[i].fname, (char*)nombre)==0) && (tablaArchivos[i].parent_directory == padre)){
			   retorno = i;
		   }
	   }
	}
	return retorno;
}

//ejemplo de uso quieroElAnteUltimo:
//	"/Vermilion City/Pokemons/EduGroso"
// with true:	-> resultado = pokemons
// with false:	-> resultado = EduGroso.
//
int buscarArchivoPorPath(char* path, bool quieroElAnteUltimo){ //retorna el indice del archivo en la tabla de archivos
	int resultado = archivoNoEncontrado;
	if(strcmp("/",path)!=0){
		char** array = string_split(path, "/");
		int i = 0;
		int j=0;
		if (quieroElAnteUltimo)	j++;	//apunta al indice siguiente.
		resultado = ROOT_INDEX;
		int padre = ROOT_INDEX;
		while(array[j]!= NULL){
			resultado = buscarArchivoEnFS(array[i], padre);
			padre = resultado;
			i++;
			j++;
		}
		//resultado = padre;
	} else {
		resultado = rootPath;
	}
	return resultado;
}


/*
 * Retorna un puntero a un array con los
 * bloques que componen al archivo
 */
int* obtenerBloquesArchivo(int numeroBloqueInicial, int cantidadDeBloques){
	int* bloques = malloc(sizeof(int) * cantidadDeBloques);
	int* tablaAsignaciones = obtenerTablaAsignaciones();
	int i = numeroBloqueInicial;
	int j= 0;
	bloques[j] = i;
	while(tablaAsignaciones[i]!= finDeArchivo){
		j++;
		bloques[j] = tablaAsignaciones[i];
		i = tablaAsignaciones[i];
	}
	return bloques;
}

int calcularBytesExedentes(int cantidadDeBloques, int tamanioArchivo){
	int bytesEnBloques = cantidadDeBloques * OSADA_BLOCK_SIZE;
	int resultado;
	if(bytesEnBloques == tamanioArchivo){
		resultado = 0;
	} else {
		int unBloqueDeMenos = (cantidadDeBloques -1) * OSADA_BLOCK_SIZE;
		int exedente = tamanioArchivo - unBloqueDeMenos;
		resultado = exedente;
	}
	return resultado;
}

osada_block* obtenerArchivo(int* bloquesQueLoConforman, int cantidadDeBloques, int tamanioArchivo){
	osada_block* archivoConOffset = malloc(cantidadDeBloques * OSADA_BLOCK_SIZE * sizeof(unsigned char));
	osada_block* bloquesDeDatos = obtenerBloqueDatos();
	int k = 0;
	int i;
	int cantidadACopiar = OSADA_BLOCK_SIZE;
	int exedente = calcularBytesExedentes(cantidadDeBloques,tamanioArchivo);
	while( k < cantidadDeBloques ){
		i = bloquesQueLoConforman[k];
		if ((k == (cantidadDeBloques -1)) && (exedente>0)){
			int cantidadACopiar = exedente;
		}
		memcpy(archivoConOffset[k], bloquesDeDatos[i], cantidadACopiar * sizeof(unsigned char));
		k++;
	}
	return archivoConOffset;
}


osada_block* obtenerArchivoPorPath(char* path, size_t bytes, off_t offset, uint32_t * tamanioCopiarSockets){
	osada_file* tablaArchivos = obtenerTablaArchivos();
	int index = buscarArchivoPorPath(path, false);
	osada_block* resultado;
	if(index>0){
		uint32_t tamanio= tablaArchivos[index].file_size;

		//valido que el offset que me pidieron sea menor al archivo.
		if ( (tamanio>0) && (tamanio> offset) )
		{
			int cantidadDeBloques = calcularCantidadBloques( tamanio );
			int* bloquesArchivo = obtenerBloquesArchivo((int)tablaArchivos[index].first_block, cantidadDeBloques);
			osada_block* archivoCompleto;
			archivoCompleto = obtenerArchivo(bloquesArchivo,cantidadDeBloques, tamanio );

			//Con esto ajusto el archivo a la solicitud, osea al offset y longitud pedida.
			uint32_t bytesParaCopiar = 0;
			if ( (offset+bytes) > tamanio )
				bytesParaCopiar = tamanio - offset;
			else
				bytesParaCopiar = bytes;

			printf("-->Voy a copiar: size: %d, offset: %jd\n", bytesParaCopiar, (intmax_t) offset);
			//finalmente lo copio
			resultado = malloc(bytesParaCopiar * sizeof(unsigned char));
			memcpy( resultado, ((unsigned char*)archivoCompleto)+offset , bytesParaCopiar * sizeof(unsigned char));
			free(archivoCompleto);
			*tamanioCopiarSockets = bytesParaCopiar;
		}
	}
	return resultado;
}

char** leerDirectorio(char* path){
	int subdirectoriosMax[2048]; //Cantidad máxima de subdirectorios
	int padre;
	if(strcmp("/",path)==0){
		padre = ROOT_INDEX;
	} else {
		padre = buscarArchivoPorPath(path, false);
	}
	osada_file* tablaArchivos = obtenerTablaArchivos();
	osada_file_state j = REGULAR;
	int i,k;
	k = 0;
	for ( i = 0 ; i<2048 ; i++ ) {
	   j = tablaArchivos[i].state;
	   if(tablaArchivos[i].parent_directory==padre && (j !=DELETED)){
		   subdirectoriosMax[k]=i;
		   k++;
	   }
	}
	char** subdirectoriosNombres;
	subdirectoriosNombres = malloc(sizeof(char)*(k+1)*OSADA_FILENAME_LENGTH);
	for(i=0;i<k;i++){
		subdirectoriosNombres[i] = tablaArchivos[subdirectoriosMax[i]].fname;
	}
	subdirectoriosNombres[k] = NULL;
	//La ultima posicion tiene un null
	return subdirectoriosNombres;
}

long* obtenerAtributos(char* path){
	long* atributos = malloc(sizeof(long) * 2);
	int indiceDirectorio = buscarArchivoPorPath(path, false);
	if(indiceDirectorio>archivoNoEncontrado){
		osada_file* tablaArchivos = obtenerTablaArchivos();
		atributos[0] = tablaArchivos[indiceDirectorio].state;
		atributos[1] = tablaArchivos[indiceDirectorio].file_size;
	} else {
		atributos[0]= archivoNoEncontrado;
	}
	return atributos;
}

/*
 * Retorna la primera posicion libre
 * en la tabla de archivos, o -1 si
 * existieran 2048 elementos en la tabla
 * de archivos
 */
int obtenerEspacioLibreTablaArchivos(){
	int i = 1;
	osada_file_state j = REGULAR;
	osada_file* tablaArchivos = obtenerTablaArchivos();
	for ( i = 0 ; (j!=DELETED && i<2048); i++ ) {
	   j = tablaArchivos[i].state;
	}
	int k;
	if(i<2048){
		i--;
		k = i;
	} else {
		k = noHayEspacioLibreTablaArchivos;
	}
	return k;
}

/*
 * Retorna el elemento n del path,
 * con -1 es el ultimo, -2 el anteultimo...
 */
unsigned char* obtenerUltimoElemento(char* path){
	unsigned char** array = (unsigned char **) string_split(path, "/");
	int i = 0;
	unsigned char* ultimo;
	if(strcmp("/",path)!= 0){
		while(array[i]!=NULL){
			ultimo = array[i];
			i++;
		}
	} else {
		ultimo = (unsigned char*) "";
	}
	return ultimo;
}

/*
 * Retorna el path del padre de
 * un directorio
 */
char* obtenerPathPadre(char* path){
	char** array = string_split(path, "/");
	int i = 0;
	while(array[i]!=NULL){
		i++;
	}
	char* padre;
	if(i>1){
		int j=0;
		int hasta;
		//TODO: cambio aca.
		while(path[j]!='\0'){
			if(path[j]=='/'){
				hasta = j;
			}
			j++;
		}
		padre = string_substring(path, 0, hasta);
	} else {
		padre = "";
	}
	return padre;
}

/*
 * Retorna 0 si el directorio se creo
 * o -1 de lo contrario
 */
int crearDirectorio(char* path){
	int existeDirectorio = buscarArchivoPorPath(path, false);
	int resultado = archivoNoEncontrado;
	int espacioLibre = obtenerEspacioLibreTablaArchivos();
	if(existeDirectorio!=archivoNoEncontrado && espacioLibre!=noHayEspacioLibreTablaArchivos){
		resultado = archivoNoEncontrado;
	} else {
		osada_file* tablaArchivos = obtenerTablaArchivos();
		tablaArchivos[espacioLibre].file_size = 0;
		tablaArchivos[espacioLibre].first_block = -1;
		memcpy(tablaArchivos[espacioLibre].fname, obtenerUltimoElemento(path), OSADA_FILENAME_LENGTH * sizeof (unsigned char));
		tablaArchivos[espacioLibre].lastmod = (unsigned)time(NULL);
		int padre;
		char* pathPadre = obtenerPathPadre(path);
		int existePadre = buscarArchivoPorPath(path, true);
		if(existePadre>-1){
			padre = existePadre;
		} else {
			padre = ROOT_INDEX;
		}
		tablaArchivos[espacioLibre].parent_directory = padre;
		tablaArchivos[espacioLibre].state = DIRECTORY;
		resultado = 0;
	}
	return resultado;
}

int borrarDirectorio(char* path){
	int resultado;
	int existeDirectorio = buscarArchivoPorPath(path, false);
	osada_file* tablaArchivos = obtenerTablaArchivos();
	if(existeDirectorio>archivoNoEncontrado){
		if(tablaArchivos[existeDirectorio].state == 2){
			tablaArchivos[existeDirectorio].file_size = 0;
			tablaArchivos[existeDirectorio].first_block = -1;
			memcpy(tablaArchivos[existeDirectorio].fname, "", OSADA_FILENAME_LENGTH * sizeof (unsigned char));
			tablaArchivos[existeDirectorio].lastmod = (unsigned)time(NULL);
			tablaArchivos[existeDirectorio].parent_directory = -1;
			tablaArchivos[existeDirectorio].state = DELETED;
			resultado = 0;
		} else {
			resultado = elPathNoCorrespondeAUnDirectorio;
		}
	} else {
		resultado = archivoNoEncontrado;
	}
	return resultado;
}



int cambiarNombre(char* path, char* pathNuevo){
	int resultado;
	//pathNuevo es un path, Ej: /tmp/prueba
	//entonces limitamos el cambiar nombre a que solo lo llame como el ulitmo elemento ("prueba")
	//sin importar donde lo renombraron, sino validamos el path padre.
	unsigned char * nombreNuevo = obtenerUltimoElemento(pathNuevo);
	int longitudCopiar = string_length((char*)nombreNuevo);
	if (string_length((char*)nombreNuevo) < 1)
		return archivoNoEncontrado;	//Enrealidad el nuevo nombre es muy corto

	//limito el nuevo largo del string al tamaño de osada
	if (longitudCopiar > (OSADA_FILENAME_LENGTH -1))
	{
		//En caso de decidir truncar el nombre usar este codigo:
		//longitudCopiar = 16;
		//nombreNuevo[16] = '\0';

		return archivoNoEncontrado;	//Enrealidad el nuevo nombre es muy largo
	}

	int existeDirectorio = buscarArchivoPorPath(path, false);
	if(existeDirectorio>archivoNoEncontrado){
		osada_file* tablaArchivos = obtenerTablaArchivos();
		memcpy(tablaArchivos[existeDirectorio].fname, nombreNuevo, (longitudCopiar+1)* sizeof (unsigned char));
		resultado = 0;
	} else {
		resultado = archivoNoEncontrado;
	}
	return resultado;
}

/*
 * Retorna el largo del ultimo elemento del path
 */
int largoUltimoElementoPath (char* path){
	char* pathAlReves = string_reverse(path);
	char** array = string_split(pathAlReves, "/");
	int largo = string_length(array[0]);
	return largo;
}

char* obtenerPathSinArchivo(char* path){
	char* pathAlReves = string_reverse(path);
	int largo = largoUltimoElementoPath(path);
	char* pathSinArchivoAlReves = string_substring_from(pathAlReves,largo);
	char* pathSinArchivo = string_reverse(pathSinArchivoAlReves);
	return pathSinArchivo;
}

char* obtenerNombreDelArchivo (char* path){
	int largoNombre = largoUltimoElementoPath(path);
	char* pathAlReves = string_reverse(path);
	char* nombreArchivo = string_substring(pathAlReves, 0, largoNombre);
	return nombreArchivo;
}

/*
 * Retorna el indice del archivo si existe,
 * archivoNoEncontrado si no existe o
 * archivoInexistenteConDirCorrecto si el archivo no existe
 * pero el path pasado es correcto.
 * Ejemplo:
 * 		checkearPath("/home/utn/so/archivo.txt") y el directorio /home/utn/so no existe
 * 		retorna archivoNoEncontrado, si /home/utn/so existiera pero el archivo no, retorna
 * 		archivoInexistenteConDirCorrecto
 */
int checkearPath(char* path){
	int resultado;
	int existeDirectorio = buscarArchivoPorPath(path, false);
	if(existeDirectorio!=rootPath && existeDirectorio==archivoNoEncontrado){
		char* pathSinArchivo = obtenerPathSinArchivo(path);
		int existeDirectorioPadre = buscarArchivoPorPath(pathSinArchivo, false);
		if(existeDirectorioPadre > archivoNoEncontrado){
			resultado = archivoInexistenteConDirCorrecto;
		} else {
			resultado = archivoNoEncontrado;
		}
	} else {
		resultado = existeDirectorio;
	}
	return resultado;
}

int obtenerCantidadBloquesLibres(){
	t_bitarray* bitmap = obtenerBitmap();
	osada_header* header = obtenerHeader();
	int totalBlocks = header->data_blocks;
	int i = 0;
	int bloquesLibres = 0;
	while(i<totalBlocks){
		if(bitarray_test_bit(bitmap,i) == bloqueLibre){
			bloquesLibres++;
		}
		i++;
	}
	return bloquesLibres;
}

int obtenerPrimerBloqueLibre(){
	t_bitarray* bitmap = obtenerBitmap();
	int i = 0;
	int primerBloqueLibre = -1;
	while(primerBloqueLibre != -1){
		if(bitarray_test_bit(bitmap,i) == bloqueLibre){
			primerBloqueLibre = i;
		}
		i++;
	}
	return primerBloqueLibre;
}

void marcarBloques(int primerBloque, int totalBloques){
	t_bitarray* bitmap = obtenerBitmap();
	int* tablaAsignaciones = obtenerTablaAsignaciones();
	int actual = primerBloque;
	int siguiente;
	while(totalBloques>0){
		bitarray_set_bit(bitmap,actual);
		siguiente = obtenerPrimerBloqueLibre();
		tablaAsignaciones[actual] = siguiente;
		totalBloques--;
	}
	tablaAsignaciones[siguiente] = finDeArchivo;
}

int crearArchivo(char* path, long bytes){
	int resultado;
	int espacioLibreTablaArchivos = obtenerEspacioLibreTablaArchivos();
	if(espacioLibreTablaArchivos!=noHayEspacioLibreTablaArchivos){
		int totalBloquesNecesarios = calcularCantidadBloques(bytes);
		int bloquesLibres = obtenerCantidadBloquesLibres();
		if(bloquesLibres>=totalBloquesNecesarios){
			osada_file* tablaArchivos = obtenerTablaArchivos();
			int primerBloqueLibre = obtenerPrimerBloqueLibre();
			char* nombreArchivo = obtenerNombreDelArchivo(path);
			char* pathPadre = obtenerPathPadre(path);
			int indicePadre = buscarArchivoPorPath(pathPadre, false);
			tablaArchivos[espacioLibreTablaArchivos].file_size = bytes;
			tablaArchivos[espacioLibreTablaArchivos].first_block = primerBloqueLibre;
			memcpy(tablaArchivos[espacioLibreTablaArchivos].fname, nombreArchivo, OSADA_FILENAME_LENGTH * sizeof (unsigned char));
			tablaArchivos[espacioLibreTablaArchivos].lastmod = (unsigned)time(NULL);
			tablaArchivos[espacioLibreTablaArchivos].parent_directory = indicePadre;
			tablaArchivos[espacioLibreTablaArchivos].state = REGULAR;
			marcarBloques(primerBloqueLibre,totalBloquesNecesarios);
		} else {
			resultado = noHayBloquesLibres;
		}
	} else {
		resultado = noHayEspacioLibreTablaArchivos;
	}
	return resultado;
}

int obtenerUltimoBloqueActual(int primerBloque){
	int* tablaAsignaciones = obtenerTablaAsignaciones();
	int ultimoActual = primerBloque;
	while(tablaAsignaciones[ultimoActual] != finDeArchivo){
		ultimoActual = tablaAsignaciones[ultimoActual];
	}
	return ultimoActual;
}

void liberarBloquesBitmap(int primerBloque){
	t_bitarray* bitmap = obtenerBitmap();
	int* tablaAsignaciones = obtenerTablaAsignaciones();
	int indice = primerBloque;
	while(tablaAsignaciones[indice] != finDeArchivo){
		bitarray_clean_bit(bitmap,indice);
		indice = tablaAsignaciones[indice];
	}
	bitarray_clean_bit(bitmap,indice);
}

int obtenerPrimerBloqueALiberar(int primerBloque, int bloquesNecesarios){
	int* tablaAsignaciones = obtenerTablaAsignaciones();
	int indice = primerBloque;
	bloquesNecesarios--; //Empiezo con primerBloque, asi que es 1 menos al total
	while(bloquesNecesarios > 0){
		indice = tablaAsignaciones[indice];
		bloquesNecesarios--;
	}
	return indice;
}

int redimencionar(int indiceArchivo, long bytesNecesarios){
	int resultado;
	osada_file* tablaArchivos = obtenerTablaArchivos();
	long bytesActuales = tablaArchivos[indiceArchivo].file_size;
	if(bytesActuales == bytesNecesarios){
		resultado = operacionExitosa; //Nada que hacer
	} else if (bytesActuales < bytesNecesarios){ // Agrandar
		int bloquesActuales = calcularCantidadBloques(bytesActuales);
		int bloquesNecesarios = calcularCantidadBloques(bytesNecesarios);
		int bloquesTotalesNecesarios = bloquesNecesarios - bloquesActuales;
		int bloquesLibres = obtenerCantidadBloquesLibres();
		if(bloquesLibres >= bloquesTotalesNecesarios){
			int primerBloque = tablaArchivos[indiceArchivo].first_block;
			int ultimoBloqueActual = obtenerUltimoBloqueActual(primerBloque);
			marcarBloques(ultimoBloqueActual,bloquesTotalesNecesarios);
			tablaArchivos[indiceArchivo].file_size = bytesNecesarios;
			resultado = operacionExitosa;
		} else {
			resultado = noHayBloquesLibres;
		}
	} else if(bytesActuales > bytesNecesarios){ // Achicar
		int bloquesNecesarios = calcularCantidadBloques(bytesNecesarios);
		int primerBloque = tablaArchivos[indiceArchivo].first_block;
		int primerBloqueALiberar = obtenerPrimerBloqueALiberar(primerBloque, bloquesNecesarios);
		liberarBloquesBitmap(primerBloqueALiberar);
		tablaArchivos[indiceArchivo].file_size = bytesNecesarios;
		resultado = operacionExitosa;
	}
	return resultado;
}

int truncar(char* path, long bytes){
	int resultado;
	int existeDirectorio = checkearPath(path);
	if(existeDirectorio == archivoInexistenteConDirCorrecto){
		//el archivo no existe pero el path es correcto
		resultado = crearArchivo(path,bytes);
	} else if(existeDirectorio > archivoNoEncontrado) {
		//el archivo existe, se debe redimencionar
		resultado = redimencionar(existeDirectorio,bytes);
	} else {
		resultado = archivoNoEncontrado;
	}
	return resultado;
}

int borrarArchivo(char* path){
	int resultado;
	int existeDirectorio = checkearPath(path);
	if(existeDirectorio > archivoNoEncontrado){
		osada_file* tablaArchivos = obtenerTablaArchivos();
		if(tablaArchivos[existeDirectorio].state == REGULAR){
			int primerBloque = tablaArchivos[existeDirectorio].first_block;
			liberarBloquesBitmap(primerBloque);
			tablaArchivos[existeDirectorio].state = DELETED;
			resultado = operacionExitosa;
		} else {
			resultado = noEsUnArchivo;
		}
	} else {
		resultado = existeDirectorio;
	}
	return resultado;
}

int escribir(const char *path, const char *buffer, size_t tamanio,off_t offset){

}



/*
 * libGrafica.c
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#include "libGrafica.h"
#include <pthread.h>				//para cerrar gui
//#include "libPlanificador.h"		//para cerrar gui
//#include "conexiones.h"				//para cerrar gui

void freeForPokeNest (t_mapa * mapa);
int fcAuxiliarEstaDentroDelMargenDelMapa(int pos_x, int pos_y, int guiRows, int guiCols);
void actualizarEstadoEntrenador(int respuesta, int * newPos_x, int * newPos_y);

/*
 * @NAME: decimeComoSeLlamaEstaPokeNest
 * @DESC: Recibe la ubicacion actual de una pokenest.
 * 		Ej: /Mapas/prueba1/PokeNests/pikachu/metadata
 *
 * 		y regresa el nombre de la pokenest: pikachu.
 *
 * 	Nota: Hay que hacer free del char * que devuelve, (y llegado el caso de ubicacionActual tambien).
 */
char * decimeComoSeLlamaEstaPokeNest (char * ubicacionActual);



//------------------------------------------//





void dibujarMapa (t_mapa * mapa, pthread_t hiloPlanificador, pthread_t hiloConexiones, pthread_t hiloBloqueados,pthread_t hiloDeadlock, sem_t * semaforoGraficar)
{
	while ( 1 )
	{

		if (_SIGINT_flag == 1 || _SIGTERM_flag == 1)
		{
			log_warning(myArchivoDeLog,"**Voy a tratar la senial SIGINT.**" );

			//pthread_mutex_lock(&mutex_sock);
			pthread_cancel(hiloConexiones);

			//sem_wait(&mapa_libre);
			pthread_cancel(hiloPlanificador);

			mapa->metadata->batalla[0] = '0';
			//usleep(mapa->metadata->tiempoChequeadoDeadlock);
			sleep(1);
			pthread_cancel(hiloDeadlock);

			//sem_trywait(&entrenador_bloqueado);
			pthread_cancel(hiloBloqueados);





			finalizarGui(mapa);
			_SIGINT_flag = 0;	//reseteo el flag.
		}

		if ( !_mapaEnModoDebug && (sem_trywait(semaforoGraficar) == 0) )
			nivel_gui_dibujar(mapa->items, mapa->nombre);

		sleepInMiliSegundos(1);
	}
}


void borrarMapa (t_mapa * mapa)
{
	if (mapa == NULL)
		return;

	freeForMetadataMapa (mapa);

	/*
	if (mapa->directorioPokeDex != NULL);
	{
		free(mapa->directorioPokeDex);
		mapa->directorioPokeDex = NULL;
	}

	if (mapa->nombre != NULL);
	{
		free(mapa->nombre);
		mapa->nombre = NULL;
	}
	 */

	if (mapa->items != NULL);
	{
		if (list_size(mapa->items) >0)
		{
			list_clean_and_destroy_elements(mapa->items, (void *) free);
			mapa->items = NULL;
		}
		else
			list_destroy(mapa->items);
	}


	freeForPokeNest(mapa);
}


t_mapa * inicializarEstructurasDelMapa (char *nombreMapa, char *directorioPokeDex)
{
	t_mapa * nuevoMapa = malloc(sizeof(t_mapa));	//pedir malloc
	nuevoMapa->nombre = nombreMapa;
	nuevoMapa->directorioPokeDex = directorioPokeDex;
	nuevoMapa->items = NULL;
	nuevoMapa->metadata = NULL;

	t_list* itemsVisiblesEnMapa = list_create();
	nuevoMapa->items = itemsVisiblesEnMapa;

	t_list* pokemonesDisponibles = list_create();
	nuevoMapa->pokeNest = pokemonesDisponibles;


	leerMetadataDelMapa (nuevoMapa);
	leerRecursivamenteLasPokenest (nuevoMapa);


	return nuevoMapa;
}


void leerMetadataDelMapa (t_mapa * nuevoMapa)
{
    log_info(myArchivoDeLog,"Voy a leer la metadata del mapa.");
	t_config *metadataMapa;		//tiene info sobre el archivo config "metadata".
	metadataMapa = config_create_for_metadataMapa(nuevoMapa);
	t_metadataMapa * nuevaMetadataMapa = malloc(sizeof(t_metadataMapa));	//pedir malloc
	nuevoMapa->metadata = nuevaMetadataMapa;

	nuevoMapa->metadata->algoritmo=NULL;
	nuevoMapa->metadata->batalla=NULL;
	nuevoMapa->metadata->ip=NULL;
	nuevoMapa->metadata->puerto = NULL;

	char * temporal;
	temporal = _mapa_configLeerString(metadataMapa, __nombreEnConfig_Deadlock, nuevoMapa, (void *) finalizarGui);
	nuevaMetadataMapa->tiempoChequeadoDeadlock = atoll(temporal);
	free(temporal);

	temporal = _mapa_configLeerString(metadataMapa, __nombreEnConfig_Retardo, nuevoMapa, (void *) finalizarGui);
	nuevaMetadataMapa->retardo = atoll(temporal);
	free(temporal);

	nuevaMetadataMapa->batalla = _mapa_configLeerString(metadataMapa, __nombreEnConfig_Batalla, nuevoMapa, (void *) finalizarGui);
	nuevaMetadataMapa->algoritmo = _mapa_configLeerString(metadataMapa, __nombreEnConfig_Algoritmo, nuevoMapa, (void *) finalizarGui);
	nuevaMetadataMapa->quantum = _mapa_configLeerInt(metadataMapa, __nombreEnConfig_Quantum, nuevoMapa, (void *) finalizarGui);
	nuevaMetadataMapa->ip = _mapa_configLeerString(metadataMapa, __nombreEnConfig_IP, nuevoMapa, (void *) finalizarGui);
	nuevaMetadataMapa->puerto = _mapa_configLeerString(metadataMapa, __nombreEnConfig_Puerto, nuevoMapa, (void *) finalizarGui);


	//nuevoMapa->metadata = nuevaMetadataMapa;

	metadata_finalizar (metadataMapa);
	loguearEstructuraDelMapa(nuevoMapa);

	log_info(myArchivoDeLog,"Finaliza la lectura de metadata del mapa.");
}



void cargarPokeNests (t_config * configPokeNest, t_mapa * nuevoMapa)
{
	char metadataIdentificador;
	metadataIdentificador = _mapa_configLeerString(configPokeNest,__nombreEnConfigIdPokeNest , nuevoMapa, (void *) finalizarGui)[0];

	char * pos_pokenest = _mapa_configLeerString(configPokeNest,__nombreEnConfigPosicionPokeNest , nuevoMapa, (void *) finalizarGui);
	int pos_x = atoi ((string_split(pos_pokenest, __separadorEnConfigPosicionPokeNest))[0] );
	int pos_y = atoi ((string_split(pos_pokenest, __separadorEnConfigPosicionPokeNest))[1] );


	int cant_de_pokemones_en_pokenest = inicializarCantDePokemonesEnPokeNest(configPokeNest->path, nuevoMapa, metadataIdentificador);



	if ( estaDentroDelMargenDelMapa(pos_x, pos_y) && esValidoElEspaciadoDePokeNest(nuevoMapa->items, pos_x, pos_y) && esUnicoEsteIdentificador(nuevoMapa->items, metadataIdentificador))
	{
		CrearCaja(nuevoMapa->items, metadataIdentificador, pos_x, pos_y, cant_de_pokemones_en_pokenest);
		log_info(myArchivoDeLog,"Se creo una Pokenest: [ %c | x=%s | y=%s | cant=%s ]", metadataIdentificador, string_itoa(pos_x), string_itoa(pos_y), string_itoa(cant_de_pokemones_en_pokenest));
	}
	else
	{
		log_error(myArchivoDeLog, "errorSintacticoSemantico. Se quiso crear una Pokenest fuera del mapa o el identificador ya existia o no estaba bien espaciadas. Pokenest: [ %c | x=%s | y=%s | cant=%s ]", metadataIdentificador, string_itoa(pos_x), string_itoa(pos_y), string_itoa(cant_de_pokemones_en_pokenest));
		finalizarGui(nuevoMapa);
	}

}


int cargarEntrenador (t_list* items, char simbolo)
{
	if (estaDentroDelMargenDelMapa(__entrenadorPosInicialEnX,__entrenadorPosInicialEnY) ==1 && esUnicoEsteIdentificador(items, simbolo))
	{
		CrearPersonaje(items, simbolo, __entrenadorPosInicialEnX, __entrenadorPosInicialEnY);
		log_info(myArchivoDeLog,"Se conecto un entrenador: [ %c ]", simbolo);
		return 0;	//No se mando ninguno moco al moverse
	}
	else
	{
		log_error(myArchivoDeLog, "error se conecto un entrenador con un simbolo ya existente o se quiso cargar al entrenador en una posicion inicial fuera del area del mapa. Simbolo: [ %c ]", simbolo);
		return 1;	//Se mando algun moco
	}
}


int moverEntrenador (t_mapa * mapa, char simbolo, int respuesta)
{
	int newPos_x=0;
	int newPos_y=0;
	ITEM_NIVEL* myEntrenador = encontrameEsteIdEnLaLista(mapa, simbolo);
	actualizarEstadoEntrenador(respuesta, &newPos_x, &newPos_y);

	if ( (newPos_x !=0) || (newPos_y!= 0))
	{
		newPos_x = newPos_x + myEntrenador->posx;
		newPos_y = newPos_y + myEntrenador->posy;

		//free (myEntrenador);

		if (estaDentroDelMargenDelMapa(newPos_x,newPos_y) ==1 )
		{

			log_debug(myArchivoDeLog, "movi al entrenador");
			log_debug(myArchivoDeLog, "simbolo: %c, %s, %s", simbolo, string_itoa(newPos_x), string_itoa(newPos_y));
			MoverPersonaje(mapa->items, simbolo, newPos_x, newPos_y);
			return 0;	//No se mando ninguno moco al moverse
		}
	}

	log_error(myArchivoDeLog, "errorSintacticoSemantico el entrenador se movio fuera del mapa. O era otra la respuesta.");
	free (myEntrenador);
	return 1;	//Se mando algun moco
}


void actualizarEstadoEntrenador(int respuesta, int * newPos_x, int * newPos_y) {

	switch (respuesta) {
	case moverDerecha:
		*newPos_x = *newPos_x + 1;
		break;
	case moverArriba:
		*newPos_y = *newPos_y - 1;
		break;
	case moverIzquierda:
		*newPos_x = *newPos_x - 1;
		break;
	case moverAbajo:
		*newPos_y = *newPos_y + 1;
		break;
	}
}




int estaDentroDelMargenDelMapa(int pos_x, int pos_y)
{
	int rows, cols;
	if (_mapaEnModoDebug)
	{
		//En caso de debug, le hardcodeo el size de la terminal a 80x24 (default).
		rows = 24;
		cols = 80;
		return fcAuxiliarEstaDentroDelMargenDelMapa(pos_x, pos_y, rows, cols);
	}
	else
	{
		if (nivel_gui_get_area_nivel(&rows, &cols) == EXIT_SUCCESS)
			return fcAuxiliarEstaDentroDelMargenDelMapa(pos_x, pos_y, rows, cols);

		return 0;
	}
}


int fcAuxiliarEstaDentroDelMargenDelMapa(int pos_x, int pos_y, int guiRows, int guiCols)
{
	log_info(myArchivoDeLog,"%s,%s", string_itoa(guiRows), string_itoa(guiCols));
	if ( (pos_x > 0) && (pos_x < guiRows) && (pos_y > 0) && (pos_y < guiCols))
		return 1;

	return 0;
}


int estosObjetosEstanEspaciados(int pos_x1, int pos_y1, int pos_x2, int pos_y2, int espaciado_x, int espaciado_y)
{
	//hice |X1-X2|=d1 y |Y1-Y2|=d2
	//ahora reviso si: d1 > distanciaMinima (idem d2).
	if ( (distanciaEntreObjetos(pos_x1, pos_x2) > espaciado_x) && (distanciaEntreObjetos(pos_y1, pos_y2) > espaciado_y))
	{
		return 1;
	}

	return 0;
}


int esValidoElEspaciadoDePokeNest(t_list* items, int newPos_x, int newPos_y)
{
    //defino la condicion (que sea una pokeNest y que cumpla estar espaciada una de otra)
	bool _esUnaPokeNestNoEspaciada(ITEM_NIVEL* item)
	{
        if ( (item->item_type == RECURSO_ITEM_TYPE) && ( !(estosObjetosEstanEspaciados(newPos_x, newPos_y, item->posx, item->posy, __pokeNestSeparacionEnX, __pokeNestSeparacionEnY) ==1) ) )
        {
        	return 1;	//NO estan espaciados
        }
        return 0;
    }

	//devuelvo 0 si NO estan espaciadas.
	if (list_count_satisfying( items, (void *) _esUnaPokeNestNoEspaciada ) > 0)
		return 0;

	return 1;
}




t_config *config_create_for_metadataMapa(t_mapa * nuevoMapa)
{
	char * directorioMapa;
	directorioMapa = malloc ( (sizeof (char)) * PATH_MAX +1);

	sprintf(directorioMapa, "/%s/%s/%s/%s", nuevoMapa->directorioPokeDex, __ubicacionMapas, nuevoMapa->nombre, __ubicacionMetadataMapas);


	t_config * metadataMapa = _mapa_newConfigType(directorioMapa, nuevoMapa, (void *) finalizarGui);
	free (directorioMapa);


	return metadataMapa;
}




void leerRecursivamenteLasPokenest (t_mapa * nuevoMapa)
{
	log_info(myArchivoDeLog,"Voy a leer las PokeNest y cargarlas");

	char * directorioMapa;
	directorioMapa = malloc ( (sizeof (char)) * PATH_MAX +1);

	sprintf(directorioMapa, "/%s/%s/%s/%s", nuevoMapa->directorioPokeDex, __ubicacionMapas, nuevoMapa->nombre , __ubicacionDirPokenest);



    //esta funcion me compara si el archivo actual es un archivo llamado __nombreMetadataPokeNest. Si es asi, levanto la config de este archivo.
    //esto es debido a que recorre recursivamente subdirectorios..
    void _funcionOrdenSuperiorQueQuieroEjecutar (const char * d_name, const char * nombreDirectorio)
    {
    	//reviso que este directorio que contiene las pokenest, tenga un archivo de "metadata"
    	//El archivo actual esta en "nombreDirectorio/d_name"
    	if ( (strcmp (d_name, __nombreMetadataPokeNest )) == 0 )
    		   		levantarConfigPokeNest( (char *)nombreDirectorio, nuevoMapa );

    }

    //consulto si devolvio 0.
    if ( ( encontrarEnUnDirectorio (directorioMapa,  (void *) _funcionOrdenSuperiorQueQuieroEjecutar ) )==0 )
    {
    	log_error(myArchivoDeLog, "problemas en 'encontrarEnUnDirectorio' y la '_funcionOrdenSuperiorQueQuieroEjecutar'");
    	finalizarGui(nuevoMapa);
    }


    free (directorioMapa);
    log_info(myArchivoDeLog,"Finalizo la carga de PokeNest.");

}



void levantarConfigPokeNest (char * nombreDirectorio, t_mapa * nuevoMapa)
{
	char * directorioMapa;
	directorioMapa = malloc ( (sizeof (char)) * PATH_MAX +1);

	sprintf(directorioMapa, "/%s/%s", nombreDirectorio, __ubicacionMetadataPokeNest);

	t_config * metadataPokenest = _mapa_newConfigType(directorioMapa, nuevoMapa, (void *) finalizarGui);
	cargarPokeNests(metadataPokenest, nuevoMapa);


	metadata_finalizar (metadataPokenest);
	free (directorioMapa);
}


void loguearEstructuraDelMapa(t_mapa * nuevoMapa)
{
	char str[256];
	sprintf(str, "%lld", nuevoMapa->metadata->tiempoChequeadoDeadlock);

	log_info(myArchivoDeLog,"tiempoChequeadoDeadlock= %s", str );
	log_info(myArchivoDeLog,"ModoBatalla= %s", nuevoMapa->metadata->batalla );
	log_info(myArchivoDeLog,"Algoritmo= %s", nuevoMapa->metadata->algoritmo );
	log_info(myArchivoDeLog,"Quantum= %s", string_itoa(nuevoMapa->metadata->quantum) );

	sprintf(str, "%lld", nuevoMapa->metadata->retardo);
	log_info(myArchivoDeLog,"Retardo= %s", str );
	log_info(myArchivoDeLog,"IP= %s", nuevoMapa->metadata->ip );
	log_info(myArchivoDeLog,"Puerto= %s", nuevoMapa->metadata->puerto );
}


void finalizarGui (t_mapa * mapa)
{
	log_info(myArchivoDeLog, "voy a finalizar la gui");
	borrarMapa (mapa);
	if (!_mapaEnModoDebug)
		nivel_gui_terminar();
	log_destroy(myArchivoDeLog);
	exit(EXIT_FAILURE);
}


bool esUnicoEsteIdentificador (t_list* items, char idDelItem)
{
	//defino la condicion (que no exista el item)
	bool _yaExistiaEsteId(ITEM_NIVEL* item)
	{
		if (item->id == idDelItem)	//sabemos que idDelItem es un char.
		{
			return 1;	//Ese id ya existe
	 	}
		return 0;
	}

	//devuelvo 0-false si NO es unico.
	if (list_count_satisfying( items, (void *) _yaExistiaEsteId ) > 0)
		return 0;

	//devuelvo 1-true xq es unico.
	return 1;
}




void freeForMetadataMapa (t_mapa * unMapa)
{
	//borro todos los strings primero
	if (unMapa->metadata != NULL)
	{
		if (unMapa->metadata->algoritmo != NULL)
			free(unMapa->metadata->algoritmo);
		if (unMapa->metadata->batalla != NULL)
			free(unMapa->metadata->batalla);
		if (unMapa->metadata->ip != NULL)
			free(unMapa->metadata->ip);
		if (unMapa->metadata->puerto != NULL)
			free(unMapa->metadata->puerto);

		free(unMapa->metadata);
		unMapa->metadata = NULL;
		log_info(myArchivoDeLog,"Libere de memoria la estructura unMapa->metadata." );
	}
}

void freeForPokeNest (t_mapa * mapa)
{
	if (mapa->pokeNest != NULL);
	{
		if (list_size(mapa->pokeNest) <1)
		{
			list_destroy(mapa->pokeNest);
			mapa->pokeNest=NULL;
			return;
		}

		int i=0, j=0;
		for (i=0; i < list_size(mapa->pokeNest); i++)
		{
			t_pokeNest * temporal = list_get(mapa->pokeNest, i);
			for (j=0; j < list_size(temporal->pokemones); j++)
			{
				t_pokemonEnPokeNest * temporal2 = list_get(temporal->pokemones, j);
				free(temporal2->pokemonNNNdat);
				temporal2->pokemonNNNdat = NULL;
			}
			list_clean_and_destroy_elements(temporal->pokemones, (void *) &free);
			temporal->pokemones = NULL;
		}
		list_clean_and_destroy_elements(mapa->pokeNest, (void *) &free);
		mapa->pokeNest = NULL;
	}
}

void accionDelMapaAnteSIGUSR2 (t_mapa * unMapa)
{
	//TODO: implementar semaforos. Si el planificador tenia en ejecucion, deberia esperar que termine, y recien ahi releer...
	freeForMetadataMapa (unMapa);
	leerMetadataDelMapa (unMapa);
}


int dondeQuedaEstaPokeNest (t_mapa * unMapa, char * idPokeNest, int * pos_x, int * pos_y)
{
	ITEM_NIVEL * resultado = encontrameEsteIdEnLaLista(unMapa, *idPokeNest);

	if (resultado != NULL)
	{
		*pos_x = resultado->posx;
		*pos_y = resultado->posy;
		//puts ("Encontre una pokenest;");
		return 1;
	}
	return 0;

}

int inicializarCantDePokemonesEnPokeNest (char * nombreDirectorio, t_mapa * nuevoMapa, char identificador)
{
	log_info(myArchivoDeLog, "Voy a contar la cantidad de pokemones en una pokenest");
	//log_debug(myArchivoDeLog,"%s", nombreDirectorio);
	char * nombrePokeNest = decimeComoSeLlamaEstaPokeNest(nombreDirectorio);

	if (nombrePokeNest != NULL)
	{
		int aux = ejecutarLogicaContarPokemons (nombrePokeNest, string_substring(nombreDirectorio,0, (string_length(nombreDirectorio) - sizeof(__ubicacionMetadataMapas))+1 ), nuevoMapa, identificador);

		free(nombrePokeNest);
		return aux;
	}

	return 0;
}

char * decimeComoSeLlamaEstaPokeNest (char * ubicacionActual)
{
	/*
	 * Como este metodo recibe un directorio del tipo /Mapas/prueba1/PokeNests/pikachu/metadata
	 * Le borro el /metadata (y chequeo que dps de pikachu no quede ninguna otra '/'), luego
	 * Lo que primero hago es conocer el nombre de la pokenest (en este caso pikachu)
	 *
	 * Para eso 1º voy a dar vuelta el string:
	 * 	"uhcakip/..."
	 *
	 * 	Luego, trunco el string con el token / y me quedo solo con la primera parte
	 * 	Y lo vuelvo a dar vuelta al string, resultando
	 * 	"pikachu"
	 *
	 * 	En una segunda instancia, hago la logica para recorrer los pokemones de la pokenest.
	 */


	char * nombrePokeNest;
	char * nombrePokeNest2;	//variable temporal
	nombrePokeNest = string_duplicate(ubicacionActual);
	if (nombrePokeNest != NULL)
	{
		//este es el path de *1
		nombrePokeNest2 = string_substring(nombrePokeNest,0, (string_length(nombrePokeNest) - sizeof(__ubicacionMetadataMapas)) );
		free(nombrePokeNest);	//string_substring hace malloc internamente.
		nombrePokeNest = nombrePokeNest2;

		//Aca voy a validar que el substring se haya hecho bien, y no hayan quedado alguna contrabarra en medio.
		int i =0;
		while ( ((*nombrePokeNest)*i) != '/')
		{
			nombrePokeNest2 = string_substring(nombrePokeNest,1, string_length(nombrePokeNest));
			free(nombrePokeNest);	//string_substring hace malloc internamente.
			nombrePokeNest = nombrePokeNest2;
			i++;
		}


		nombrePokeNest = string_reverse(nombrePokeNest);
		nombrePokeNest = (string_n_split(nombrePokeNest, 2, "/"))[0];
		nombrePokeNest = string_reverse(nombrePokeNest);

		//log_debug(myArchivoDeLog,"%s", nombrePokeNest);
		//le paso el path de *1
		return nombrePokeNest;
	}
	return NULL;	//algun quilombo.
}


int ejecutarLogicaContarPokemons (char * nombrePokeNest, char * nombreDirectorio, t_mapa * nuevoMapa, char identificador)
{
	/* *************************************** */
	/* Logica para conocer cuantos pokemon hay */

	t_pokeNest * pokemonesEnEstaPokeNest = malloc (sizeof(t_pokeNest));
	pokemonesEnEstaPokeNest->identificador = identificador;
	pokemonesEnEstaPokeNest->pokemones = list_create();
	list_add(nuevoMapa->pokeNest, pokemonesEnEstaPokeNest);

	char * pokemonNNNdat;
	pokemonNNNdat = malloc ( (sizeof (char)) * PATH_MAX +1);


	//esta funcion me compara si el archivo actual es un archivo llamado pokemonNNNdat. Si es asi, sumo +1.
	void _funcionOrdenSuperiorQueQuieroEjecutar (const char * d_name, const char * nombreDirectorio)
	{
		//El archivo actual esta en "nombreDirectorio/d_name"
		if ( (strcmp (d_name, pokemonNNNdat )) == 0 )
		{
			log_debug(myArchivoDeLog,"%s", d_name);
			t_pokemonEnPokeNest * pokemon = malloc (sizeof(t_pokemonEnPokeNest));
			pokemon->pokemonNNNdat = string_from_format("%s/%s", nombreDirectorio, d_name) ;
			pokemon->capturadoPorEntrenador = '\0';				//lo inicializo como vacio.
			list_add(pokemonesEnEstaPokeNest->pokemones, pokemon);
		}
	}

	int i = __valorInicialPokemonesPokenest;
	while (i <= __valorFinalPokemonesPokenest)
	{
		sprintf(pokemonNNNdat, __ubicacionPokemonesPokeNest,nombrePokeNest, i);
		//consulto si devolvio 0.
		if ( ( encontrarEnUnDirectorio (nombreDirectorio,  (void *) _funcionOrdenSuperiorQueQuieroEjecutar ) )==0 )
		{
			log_error(myArchivoDeLog, "problemas en 'encontrarEnUnDirectorio' y la '_funcionOrdenSuperiorQueQuieroEjecutar'");
			finalizarGui(nuevoMapa);
		}
		i++;
	}
	free(pokemonNNNdat);
	return list_size(pokemonesEnEstaPokeNest->pokemones);
}


int distanciaEntrenadorPokenest (char simboloEntrenador, t_mapa * self, char pokeNest)
{
	if (pokeNest == '\0')
	{
		log_error(myArchivoDeLog, "me pidieron una distancia con una pokenest invalida [P=%c | E= %c]", pokeNest, simboloEntrenador);
		return -1;
	}



	ITEM_NIVEL* itemPokeNest = encontrameEsteIdEnLaLista (self, pokeNest);
	if (itemPokeNest == NULL)
	{
		log_error(myArchivoDeLog, "me pidieron una distancia con una pokenest invalida [P=%c | E= %c]", pokeNest, simboloEntrenador);
		return -1;
	}



	ITEM_NIVEL* myEntrenador = encontrameEsteIdEnLaLista(self, simboloEntrenador);
	if (myEntrenador != NULL)
	{
		int aux = 0;
		aux = aux + distanciaEntreObjetos(itemPokeNest->posx, myEntrenador->posx);
		aux = aux + distanciaEntreObjetos(itemPokeNest->posy, myEntrenador->posy);
		return aux;
	}

	//devuelvo -1  xq fallo.
	return -1;
}



int distanciaEntreObjetos (int pos_x1, int pos_x2)
{
	int dif_x = (pos_x1 - pos_x2);

	if (dif_x < 0 )
		dif_x = (pos_x2 - pos_x1);

	//hice |X1-X2|=d1
	return dif_x;
}

ITEM_NIVEL* encontrameEsteIdEnLaLista (t_mapa * self, char id)
{

	bool encontrameEstaPokeNest (ITEM_NIVEL* item)
	{
		if (item->id == id)	//sabemos que idDelItem es un char.
		{
			return 1;	//Encontre el id
		}
		return 0;
	}

	return list_find(self->items, (void*) encontrameEstaPokeNest);
}

void borrarEntrenadorDelMapa(t_mapa * unMapa, char simboloEntrenador)
{
	BorrarItem(unMapa->items, simboloEntrenador);
	//TODO: liberar memoria??

	int j=0;
	int cant =0;
	int i= 0;
	for (i=0; i< unMapa->pokeNest->elements_count; i++)
	{
		t_pokeNest * pokeNest = list_get(unMapa->pokeNest, i);
		for (j=0; j< pokeNest->pokemones->elements_count; j++)
		{
			t_pokemonEnPokeNest * pokemon = list_get(pokeNest->pokemones, j);

			if ( pokemon->capturadoPorEntrenador == simboloEntrenador)
			{
				cant++;
				pokemon->capturadoPorEntrenador = '\0';
			}
		}
		ITEM_NIVEL* pokenestActual = encontrameEsteIdEnLaLista(unMapa,pokeNest->identificador);
		pokenestActual->quantity = pokenestActual->quantity + cant;
		cant =0;
	}



}


/*
 * libGrafica.c
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#include "libGrafica.h"


void dibujarMapa (t_mapa * mapa)
{

	//TODO: esto en cualquier momento se va...
	//entrenador de prueba por el momento
	//cargarEntrenador(mapa->items, '#');
	//cargarEntrenador(mapa->items, '$');
	//cargarEntrenador(mapa->items, '@');

	//moverEntrenador(mapa->items, '#', 5,5);


	//log_info(myArchivoDeLog, "voy a tratar de dibujar el mapa ");

	//nivel_gui_dibujar(mapa->items, mapa->nombre);
	//TODO: se deberia redibujar el nivel ante cada cambio...

	while ( 1 )
	{
		//TODO: esto no deberia ir aca...??
		//Nota: es un ejemplo de uso!! darle bola
		funcionesQueQuieroEjecutarSegunLaSenial(mapa, (void *) finalizarGui, (void* ) accionDelMapaAnteSIGUSR2 );
		nivel_gui_dibujar(mapa->items, mapa->nombre);

/*
		int key = getch();

		switch( key )
		{
			case 'Q':
			case 'q':
				finalizarGui(mapa);
				break;

		}
*/
	}


}


void borrarMapa (t_mapa * mapa)
{
	if (mapa == NULL)
		return;

	if (mapa->metadata != NULL)
		freeForMetadataMapa (mapa);

	//TODO: recorrer listas y borrar.


	//free(mapa->metadata);

	/*
	BorrarItem(items, '#');
	BorrarItem(items, '@');

	BorrarItem(items, '1');
	BorrarItem(items, '2');

	BorrarItem(items, 'H');
	BorrarItem(items, 'M');
	BorrarItem(items, 'F');
	*/
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

	nuevaMetadataMapa->tiempoChequeadoDeadlock = _mapa_configLeerInt(metadataMapa, __nombreEnConfig_Deadlock, nuevoMapa, (void *) finalizarGui);
	nuevaMetadataMapa->batalla = _mapa_configLeerString(metadataMapa, __nombreEnConfig_Batalla, nuevoMapa, (void *) finalizarGui);
	nuevaMetadataMapa->algoritmo = _mapa_configLeerString(metadataMapa, __nombreEnConfig_Algoritmo, nuevoMapa, (void *) finalizarGui);
	nuevaMetadataMapa->quantum = _mapa_configLeerInt(metadataMapa, __nombreEnConfig_Quantum, nuevoMapa, (void *) finalizarGui);
	nuevaMetadataMapa->retardo = _mapa_configLeerInt(metadataMapa, __nombreEnConfig_Retardo, nuevoMapa, (void *) finalizarGui);
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


void cargarEntrenador (t_list* items, char simbolo)
{
	if (estaDentroDelMargenDelMapa(__entrenadorPosInicialEnX,__entrenadorPosInicialEnY) ==1 && esUnicoEsteIdentificador(items, simbolo))
	{
		CrearPersonaje(items, simbolo, __entrenadorPosInicialEnX, __entrenadorPosInicialEnY);
		log_info(myArchivoDeLog,"Se conecto un entrenador: [ %c ]", simbolo);
		return;
	}
	else
	{
		log_error(myArchivoDeLog, "error se conecto un entrenador con un simbolo ya existente o se quiso cargar al entrenador en una posicion inicial fuera del area del mapa. Simbolo: [ %c ]", simbolo);
		//TODO: deberia finalizar la gui? Enrealidad deberia finalizar al entrenador....
		finalizarGui(NULL);
	}
}


void moverEntrenador (t_list* items, char simbolo, int newPos_x, int newPos_y)
{
	if (estaDentroDelMargenDelMapa(newPos_x,newPos_y) ==1 )
	{
		log_debug(myArchivoDeLog, "movi al entrenador");
		MoverPersonaje(items, simbolo, newPos_x, newPos_y);
		return;
	}
	else
	{
		log_error(myArchivoDeLog, "errorSintacticoSemantico el entrenador se movio fuera del mapa.");
		//TODO: deberia finalizar la gui? Enrealidad deberia finalizar al entrenador....
		finalizarGui(NULL);
	}
}



int estaDentroDelMargenDelMapa(int pos_x, int pos_y)
{
	int rows, cols;
	if (nivel_gui_get_area_nivel(&rows, &cols) == EXIT_SUCCESS)
	{
		if (pos_x > 0 && pos_x < rows && pos_y > 0 && pos_y < cols)
		{
			return 1;
		}
	}
	return 0;

}


int estosObjetosEstanEspaciados(int pos_x1, int pos_y1, int pos_x2, int pos_y2, int espaciado_x, int espaciado_y)
{
	int dif_x = (pos_x1 - pos_x2);
	int dif_y = (pos_y1 - pos_y2);

	if (dif_x < 0 )
		dif_x = (pos_x2 - pos_x1);

	if (dif_y < 0 )
			dif_y = (pos_y2 - pos_y1);

	//hice |X1-X2|=d1 y |Y1-Y2|=d2
	//ahora reviso si: d1 > distanciaMinima (idem d2).
	if (dif_x > espaciado_x && dif_y > espaciado_y)
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
	log_info(myArchivoDeLog,"tiempoChequeadoDeadlock= %s", string_itoa(nuevoMapa->metadata->tiempoChequeadoDeadlock) );
	log_info(myArchivoDeLog,"ModoBatalla= %s", nuevoMapa->metadata->batalla );
	log_info(myArchivoDeLog,"Algoritmo= %s", nuevoMapa->metadata->algoritmo );
	log_info(myArchivoDeLog,"Quantum= %s", string_itoa(nuevoMapa->metadata->quantum) );
	log_info(myArchivoDeLog,"Retardo= %s", string_itoa(nuevoMapa->metadata->retardo) );
	log_info(myArchivoDeLog,"IP= %s", nuevoMapa->metadata->ip );
	log_info(myArchivoDeLog,"Puerto= %s", nuevoMapa->metadata->puerto );
}


void finalizarGui (t_mapa * mapa)
{
	log_info(myArchivoDeLog, "voy a finalizar la gui");
	borrarMapa (mapa);
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
	//free(unMapa->metadata->algoritmo);
	//free(unMapa->metadata->batalla);
	//free(unMapa->metadata->ip);
	//free(unMapa->metadata->puerto);

	free(unMapa->metadata);
	unMapa->metadata = NULL;
	log_info(myArchivoDeLog,"Libere de memoria la estructura unMapa->metadata." );
}


void accionDelMapaAnteSIGUSR2 (t_mapa * unMapa)
{
	//TODO: implementar semaforos. Si el planificador tenia en ejecucion, deberia esperar que termine, y recien ahi releer...
	freeForMetadataMapa (unMapa);
	leerMetadataDelMapa (unMapa);
}


int dondeQuedaEstaPokeNest (t_mapa * unMapa, char * idPokeNest, int * pos_x, int * pos_y)
{
	ITEM_NIVEL * resultado;

	bool _coincideElIdconEsteElemento(ITEM_NIVEL* item)
	{
		if (item->id == idPokeNest[0])	//sabemos que idPokeNest es un char.
		{
			return 1;	//Este es el ID que busco
		 }
		return 0;
	}

	resultado = list_find(unMapa->items, (void *) _coincideElIdconEsteElemento);
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
	/*
	 * Como este metodo recibe un directorio del tipo /Mapas/prueba1/PokeNests/pikachu/metadata
	 * Le borro el /metadata, luego
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

	log_info(myArchivoDeLog, "Voy a contar la cantidad de pokemones en una pokenest");
	//log_debug(myArchivoDeLog,"%s", nombreDirectorio);
	char * nombrePokeNest = malloc ( (sizeof (char)) * PATH_MAX +1);
	nombrePokeNest = string_duplicate(nombreDirectorio);

	if (nombrePokeNest != NULL)
	{
		//este es el path de *1
		nombrePokeNest = string_substring(nombrePokeNest,0, (string_length(nombrePokeNest) - sizeof(__ubicacionMetadataMapas))+1 );		//el +1 es xq hay 2 contrabarras.
		nombrePokeNest = string_reverse(nombrePokeNest);

		nombrePokeNest = (string_n_split(nombrePokeNest, 2, "/"))[0];
		nombrePokeNest = string_reverse(nombrePokeNest);

		//log_debug(myArchivoDeLog,"%s", nombrePokeNest);

		//le paso el path de *1
		int aux = ejecutarLogicaContarPokemons (nombrePokeNest, string_substring(nombreDirectorio,0, (string_length(nombreDirectorio) - sizeof(__ubicacionMetadataMapas))+1 ), nuevoMapa, identificador);

		free(nombrePokeNest);
		return aux;
	}

	return 0;
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
			t_pokemon * pokemon = malloc (sizeof(t_pokemon));
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

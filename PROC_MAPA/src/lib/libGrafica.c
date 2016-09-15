/*
 * libGrafica.c
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#include "libGrafica.h"


void dibujarMapa (t_mapa * mapa)
{


	//entrenador de prueba por el momento
	cargarEntrenador(mapa->items, '#');
	cargarEntrenador(mapa->items, '$');
	cargarEntrenador(mapa->items, '@');

	moverEntrenador(mapa->items, '#', 5,5);


	log_info(myArchivoDeLog, "voy a tratar de dibujar el mapa ");

	nivel_gui_dibujar(mapa->items, mapa->nombre);

	while ( 1 ) {
			int key = getch();

			switch( key ) {

				case 'Q':
				case 'q':
					finalizarGui(mapa);
				break;

			}
	}


}


void borrarMapa (t_mapa * mapa)
{
	if (mapa == NULL)
		return;


	//TODO: recorrer listas y borrar.

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

	t_list* itemsVisiblesEnMapa = list_create();
	nuevoMapa->items = itemsVisiblesEnMapa;


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

	nuevaMetadataMapa->tiempoChequeadoDeadlock = _mapa_configLeerInt(metadataMapa, __nombreEnConfig_Deadlock);
	nuevaMetadataMapa->batalla = _mapa_configLeerString(metadataMapa, __nombreEnConfig_Batalla);
	nuevaMetadataMapa->algoritmo = _mapa_configLeerString(metadataMapa, __nombreEnConfig_Algoritmo);
	nuevaMetadataMapa->quantum = _mapa_configLeerInt(metadataMapa, __nombreEnConfig_Quantum);
	nuevaMetadataMapa->retardo = _mapa_configLeerInt(metadataMapa, __nombreEnConfig_Retardo);
	nuevaMetadataMapa->ip = _mapa_configLeerString(metadataMapa, __nombreEnConfig_IP);
	nuevaMetadataMapa->puerto = _mapa_configLeerString(metadataMapa, __nombreEnConfig_Puerto);


	nuevoMapa->metadata = nuevaMetadataMapa;

	metadata_finalizar (metadataMapa);
	loguearEstructuraDelMapa(nuevoMapa);

	log_info(myArchivoDeLog,"Finaliza la lectura de metadata del mapa.");
}



void cargarPokeNests (t_config * configPokeNest, t_mapa * nuevoMapa)
{
	char metadataIdentificador;
	metadataIdentificador = _mapa_configLeerString(configPokeNest,__nombreEnConfigIdPokeNest )[0];

	char * pos_pokenest = _mapa_configLeerString(configPokeNest,__nombreEnConfigPosicionPokeNest );
	int pos_x = atoi ((string_split(pos_pokenest, __separadorEnConfigPosicionPokeNest))[0] );
	int pos_y = atoi ((string_split(pos_pokenest, __separadorEnConfigPosicionPokeNest))[1] );

	//TODO: levantar dinamico este valor.
	int cant_de_pokemones_en_pokenest = 4;	// -> Hay que contar cuantos archivos hay exeptuando el de metadata.



	//TODO: validar si la pokeNest tiene pokemon de una unica especie..)
	//TODO: consultar si es unico el identificador de pokenest x mapa..
	if ( estaDentroDelMargenDelMapa(pos_x, pos_y) && esValidoElEspaciadoDePokeNest(nuevoMapa->items, pos_x, pos_y) )
	{
		CrearCaja(nuevoMapa->items, metadataIdentificador, pos_x, pos_y, cant_de_pokemones_en_pokenest);
		log_info(myArchivoDeLog,"Se creo una Pokenest: [ %c | x=%s | y=%s | cant=%s ]", metadataIdentificador, string_itoa(pos_x), string_itoa(pos_y), string_itoa(cant_de_pokemones_en_pokenest));
	}
	else
	{
		//TODO: errorSintacticoSemantico fuera del mapa
		log_error(myArchivoDeLog, "errorSintacticoSemantico fuera del mapa o pokenest no espaciadas");
		finalizarGui(nuevoMapa);
	}

}


void cargarEntrenador (t_list* items, char simbolo)
{
	//TODO: Consultar si el simbolo de 1 entrenador es unico en un mapa (entiendo que si)

	if (estaDentroDelMargenDelMapa(__entrenadorPosInicialEnX,__entrenadorPosInicialEnY) ==1 )
	{
		CrearPersonaje(items, simbolo, __entrenadorPosInicialEnX, __entrenadorPosInicialEnY);
		log_info(myArchivoDeLog,"Se conecto un entrenador: [ %c ]", simbolo);
		return;
	}
	else
	{
		//TODO: errorSintacticoSemantico fuera del mapa
		finalizarGui(NULL);
	}
}


void moverEntrenador (t_list* items, char simbolo, int newPos_x, int newPos_y)
{
	//TODO: Consultar si un entrenador se puede mover afuera del mapa (entiendo que no)
	//		deberia abortar mapa o entrenador??


	//TODO: valido que se mueva de a 1 paso? (1 paso y ademas no en diagonal)
	//		deberia abortar mapa o entrenador??

	if (estaDentroDelMargenDelMapa(newPos_x,newPos_y) ==1 )
	{
		MoverPersonaje(items, simbolo, newPos_x, newPos_y);
		return;
	}
	else
	{
		//TODO: errorSintacticoSemantico fuera del mapa
		log_error(myArchivoDeLog, "errorSintacticoSemantico entrenador fuera del mapa");
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


	t_config * metadataMapa = _mapa_newConfigType(directorioMapa);
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
    	finalizarGui(nuevoMapa);


    free (directorioMapa);
    log_info(myArchivoDeLog,"Finalizo la carga de PokeNest.");

}



void levantarConfigPokeNest (char * nombreDirectorio, t_mapa * nuevoMapa)
{
	char * directorioMapa;
	directorioMapa = malloc ( (sizeof (char)) * PATH_MAX +1);

	sprintf(directorioMapa, "/%s/%s", nombreDirectorio, __ubicacionMetadataPokeNest);


	t_config * metadataPokenest = _mapa_newConfigType(directorioMapa);
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




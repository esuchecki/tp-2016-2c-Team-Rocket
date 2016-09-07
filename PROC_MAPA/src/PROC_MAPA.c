/*
 ============================================================================
 Name        : PROC_MAPA.c
 Author      : TEAM ROCKET
 Version     :
 Copyright   : TEAM ROCKET
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//----------- Sector Include ---------------//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//agreagado de librerias utn.so
#include <nivel.h>
#include <tad_items.h>		//contiene una lista con los items a dibujar en el mapa
#include <curses.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>

//agregado de librerias para leer directorios
#include <sys/types.h>
#include <dirent.h>		/* "readdir" etc. are defined here. */
#include <limits.h>		/* limits.h defines "PATH_MAX". */



//------------------------------------------//
/* ********************************************	*/
//----------- Sector Estructuras -------------//

typedef struct
{
	uint16_t tiempoChequeadoDeadlock;
	char * batalla;
	char * algoritmo;
	uint16_t quantum;
	uint16_t retardo;
	char * ip;
	char * puerto;
} t_metadataMapa ;


typedef struct
{
	uint16_t nivel;
	char capturadoPorEntrenador;	//'0' para ninguno, simbolo del entrenador para identificar quien lo tiene
} t_pokemon;

/*
typedef struct
{
	char identificador;
	char tipo[20];
	uint16_t pos_x;
	uint16_t pos_y;
	t_pokemon pokemones;	//a futuro esto deberia ser un array o una lista enrealidad....
} t_pokeNest;
*/

typedef struct
{
     char * nombre;
     char * directorioPokeDex;
     t_metadataMapa * metadata;
     t_list* items;
     //t_pokeNest pokeNest;		//a futuro esto deberia ser un array o una lista enrealidad...
} t_mapa ;

t_log* log;

//------------------------------------------//


//------------------------------------------//
/* ********************************************	*/
//----------- Sector Funciones -------------//


/*
 * @NAME: dibujarMapa
 * @DESC: dibuja el mapa en pantalla en base a la estructura t_mapa
 */
void dibujarMapa (t_mapa * mapa);

/*
 * @NAME: borrarMapa
 * @DESC: borra el mapa, recibe la estructura t_mapa
 */
void borrarMapa (t_mapa * mapa);

/*
 * @NAME: cargarPokeNests
 * @DESC: Dado un archivo de configuracion con la metadata, lee sus parametros
 */
void cargarPokeNests (t_config * configPokeNest, t_mapa * nuevoMapa);

/*
 * @NAME: estaDentroDelMargenDelMapa
 * @DESC: Le pasas la posicion de un objeto y te valida que este dentro de los margenes del area de juego.
 * @RET.: Devuelve 0 si NO esta dentro de los margenes, otro valor (1) si esta dentro de los margenes.
 */
int estaDentroDelMargenDelMapa(int pos_x, int pos_y);

/*
 * @NAME: estosObjetosEstanEspaciados
 * @DESC: Verifica en los ejes x e y si: |x1-x2| > espaciado_x
 * @RET.: Devuelve 0 si NO estan "pegados", otro valor (1) si estan correctamente espaciados.
 */
int estosObjetosEstanEspaciados(int pos_x1, int pos_y1, int pos_x2, int pos_y2, int espaciado_x, int espaciado_y);

/*
 * @NAME: validarEspaciadoDePokeNest
 * @DESC: Verifica que entre una nueva pokeNest y las existentes cumpla la distancia minima de separacion.
 * @RET.: Devuelve 0 si NO cumple la distancia, otro valor (1) si estan correctamente espaciados.
 */
int esValidoElEspaciadoDePokeNest(t_list* items, int newPos_x, int newPos_y);

/*
 * @NAME: cargarEntrenador
 * @DESC: Crea un nuevo entrenador.
 */
void cargarEntrenador (t_list* items, char simbolo);

/*
 * @NAME: moverEntrenador
 * @DESC: mueve un Entrenador a una nueva posicion del mapa.
 */
void moverEntrenador (t_list* items, char simbolo, int newPos_x, int newPos_y);

/*
 * @NAME: metadata_inicializar
 * @DESC: lee toda la metadata del mapa y la inicializa
 */
t_mapa * inicializarEstructurasDelMapa (char nombreMapa[], char directorioPokeDex[]);

/*
 * @NAME: metadata_finalizar
 * @DESC: borra toda la metadata del mapa
 */
void metadata_finalizar (t_config *metadataMapa);

/*
 * @NAME: leerMetadataDelMapa
 * @DESC: inicializa todas las estructuras del mapa
 */
void leerMetadataDelMapa (t_mapa * nuevoMapa);

/*
 * @NAME: config_create_for_metadataMapa
 * @DESC: inicializa SOLO la metadata del mapa (IP, Puerto, etc..) ( /Mapas/[name]/metadata )
 */
t_config *config_create_for_metadataMapa(t_mapa * nuevoMapa);

/*
 * @NAME: configLeerInt
 * @DESC: Lee un valor de tipo Int en el archivo de config
 */
uint16_t configLeerInt (t_config * archivoConfig, char nombreDeLaPropiedad[50]);

/*
 * @NAME: configLeerString
 * @DESC: Lee un valor de tipo String en el archivo de config
 */
char * configLeerString (t_config * archivoConfig, char nombreDeLaPropiedad[50]);

/*
 * @NAME: leerRecursivamenteLasPokenest
 * @DESC: Funcion de llamado para comenzar a leer las pokenest
 */
void leerRecursivamenteLasPokenest (t_mapa * nuevoMapa);

/*
 * @NAME: buscamePokeNestEnEsteDirectorio
 * @DESC: Lee el directorio pasado por parametro e intenta buscar si tiene PokeNests
 */
static void buscamePokeNestEnEsteDirectorio (const char * nombreDirectorio, t_mapa * nuevoMapa);


/*
 * @NAME: levantarConfigPokeNest
 * @DESC: Recibe un directorio (que supuestamente contiene la metadata de una pokenest) y agrega la pokenest a los items del mapa
 *
 * Se baso en estas webs:
 * 	//http://www.lemoda.net/c/recursive-directory/
 *	//https://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html
 *
 */
void levantarConfigPokeNest (char * nombreDirectorio, t_mapa * nuevoMapa);

//------------------------------------------//
/* ********************************************	*/
//----------- Sector Constantes -------------//


#define __pokeNestSeparacionEnX (2)		// el enunciado dice que 2 pokenest deben estar separadas en el eje x, unas posiciones
#define __pokeNestSeparacionEnY (2)		// idem para el eje y
#define __entrenadorPosInicialEnX (1) 	// que posicion de la gui inicia el jugador en el eje x
#define __entrenadorPosInicialEnY (1)	// idem para el eje y

#define __ubicacionMapas "/Mapas/"
#define __ubicacionMetadataMapas "/metadata"
#define __ubicacionDirPokenest "/PokeNests/"
#define __ubicacionMetadataPokeNest "/metadata"


#define __nombreEnConfig_Deadlock "TiempoChequeoDeadlock"
#define __nombreEnConfig_Batalla "Batalla"
#define __nombreEnConfig_Algoritmo "algoritmo"
#define __nombreEnConfig_Quantum "quantum"
#define __nombreEnConfig_Retardo "retardo"
#define __nombreEnConfig_IP "IP"
#define __nombreEnConfig_Puerto "Puerto"


#define __nombreEnConfigIdPokeNest "Identificador"
#define __nombreEnConfigPosicionPokeNest "Posicion"


//------------------------------------------//








int main( int argc, char *argv[] )
{
	puts("PROCESO MAPA"); /* prints PROCESO MAPA */

	//arg[0] -> nombre del proceso ejecutable
	//arg[1] -> nombre del mapa
	//arg[2] -> donde esta ubicada la pokedex
	if ( argc !=3  ) /* debe recibir 3 argumentos */
	{
		printf("\tUso incorrecto..\n");
		printf("\tuso: %s nombreMapa dirPokeDex\n", argv[0]);
	  	printf("\tejemplo: %s %s %s\n", argv[0], "prueba1", "/home/utnso/git/tp-2016-2c-Team-Rocket/PROC_MAPA/test_files");
		exit(EXIT_FAILURE);
	}


	/*levanto el archivo para loggear*/
	//TODO: revisar que pasa si no existe el archivo de log y/o el directorio
	char* file ="/home/utnso/Escritorio/logMapa_teamRocket";
	char* pg_name = "Mapa";

	//TODO: revisar que pasa si no esta creado el archivo :S
	log = log_create(file, pg_name, false, LOG_LEVEL_INFO);
	if (log != NULL)
	{
		puts("se creo OK el arch de log");
	}
	else
	{
		puts("NO se pudo crear el archivo de log");
		exit(EXIT_FAILURE);
	}


	#warning("Consultar el nombre del proceso")
	//TODO: consultar el nombre de proceso, creo que es "mapa"
	#warning("Consultar si el nombre del mapa puede contener espacios")
	//TODO: consultar si el nombre de un mapa puede contener espacios....? En enunciado dice "Ciudad Paleta"

	char * nombreMapa = argv[1];
	char * directorioPokeDex = argv[2];



	if ( directorioPokeDex == NULL || (strlen (directorioPokeDex) < 1 ) )
	{
		//TODO: errorSintacticoSemantico nombre del directorio incorrecto
		log_error(log, "errorSintacticoSemantico nombre del directorio incorrecto");
		exit(EXIT_FAILURE);
	}
	if ( nombreMapa == NULL || (strlen (nombreMapa) < 1 ) )
	{
		//TODO: errorSintacticoSemantico nombre del directorio incorrecto
		log_error(log, "errorSintacticoSemantico nombre del directorio incorrecto");
		exit(EXIT_FAILURE);
	}







	//TODO: no inicializar 2 procesos mapa con el mismo nombre en el sistema...



	t_mapa * mapa;
	nivel_gui_inicializar();

	//ignoro este warning, se inicializa mas adelante.
	#pragma GCC diagnostic ignored "-Wuninitialized"
	#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
	mapa = inicializarEstructurasDelMapa (nombreMapa, directorioPokeDex);



	//TODO: loguear estos valores (creo que es pedido del enunciado)
	//printf("%d\n", mapa->metadata->tiempoChequeadoDeadlock);
	//printf("%s\n", mapa->metadata->batalla);
	//printf("%s\n", mapa->metadata->algoritmo);
	//printf("%d\n", mapa->metadata->quantum);
	//printf("%d\n", mapa->metadata->retardo);
	//printf("%s\n", mapa->metadata->ip);
	//printf("%s\n", mapa->metadata->puerto);




	dibujarMapa (mapa);
	borrarMapa (mapa);
	nivel_gui_terminar();






	return EXIT_SUCCESS;
}


void dibujarMapa (t_mapa * mapa)
{


	//entrenador de prueba por el momento
	cargarEntrenador(mapa->items, '#');
	cargarEntrenador(mapa->items, '$');
	cargarEntrenador(mapa->items, '@');

	moverEntrenador(mapa->items, '#', 5,5);


	log_info(log, "voy a tratar de dibujar el mapa ");

	nivel_gui_dibujar(mapa->items, mapa->nombre);

	while ( 1 ) {
			int key = getch();

			switch( key ) {

				case 'Q':
				case 'q':
					nivel_gui_terminar();
					exit(0);
				break;

			}
	}


}


void borrarMapa (t_mapa * mapa)
{
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


	//TODO: chequear la longitud del string, por ejemplo si es de 10 y le ingreso 15 tengo problemas...

	leerMetadataDelMapa (nuevoMapa);

	leerRecursivamenteLasPokenest (nuevoMapa);


	return nuevoMapa;
}


void leerMetadataDelMapa (t_mapa * nuevoMapa)
{
	t_config *metadataMapa;		//tiene info sobre el archivo config "metadata".
	metadataMapa = config_create_for_metadataMapa(nuevoMapa);
	t_metadataMapa * nuevaMetadataMapa = malloc(sizeof(t_metadataMapa));	//pedir malloc

	nuevaMetadataMapa->tiempoChequeadoDeadlock = configLeerInt(metadataMapa, __nombreEnConfig_Deadlock);
	nuevaMetadataMapa->batalla = configLeerString(metadataMapa, __nombreEnConfig_Batalla);
	nuevaMetadataMapa->algoritmo = configLeerString(metadataMapa, __nombreEnConfig_Algoritmo);
	nuevaMetadataMapa->quantum = configLeerInt(metadataMapa, __nombreEnConfig_Quantum);
	nuevaMetadataMapa->retardo = configLeerInt(metadataMapa, __nombreEnConfig_Retardo);
	nuevaMetadataMapa->ip = configLeerString(metadataMapa, __nombreEnConfig_IP);
	nuevaMetadataMapa->puerto = configLeerString(metadataMapa, __nombreEnConfig_Puerto);


	nuevoMapa->metadata = nuevaMetadataMapa;

	metadata_finalizar (metadataMapa);
}


void metadata_finalizar (t_config *metadataMapa)
{
	//TODO: borrar todo...
	log_info(log, "borro metadata Mapa");
	config_destroy (metadataMapa);
}


void cargarPokeNests (t_config * configPokeNest, t_mapa * nuevoMapa)
{
	//TODO: levantar datos de un .config:
		/*		/Mapas/[nombre]/PokeNests/XXXXXX/				-> Las XXXX es un Subdirectorio con el nombre de la pokenest, ej: pikachu.
		 * 									..../metadata		-> archivo con contenido (sobre como graficar pokenest en pantalla)
		 * 									..../pokemon.dat 	-> archivo con lo unico importante el lvl del pokemon (hay un .dat x cada pokemon)
		 */

	char metadataIdentificador;
	//char infoLeida[50];
	//strcpy (infoLeida, configLeerString(configPokeNest,__nombreEnConfigIdPokeNest ) );
	//metadataIdentificador = *(infoLeida+0);
	//log_info(log, infoLeida);
	metadataIdentificador = 'P';
	//metadataIdentificador = infoLeida[0];

	//char * posicionPokeNest;
	int pos_x = 4;
	int pos_y = 6;

	//posicionPokeNest = configLeerString(configPokeNest,__nombreEnConfigPosicionPokeNest );
	//TODO: parsearPosPokeNest

	//TODO: levantar dinamico este valor.
	int cant_de_pokemones_en_pokenest = 4;	// -> Hay que contar cuantos archivos hay exeptuando el de metadata.



	//TODO: validar si la pokeNest tiene pokemon de una unica especie..)
	//TODO: consultar si es unico el identificador de pokenest x mapa..
	if ( estaDentroDelMargenDelMapa(pos_x, pos_y) && esValidoElEspaciadoDePokeNest(nuevoMapa->items, pos_x, pos_y) )
	{
		CrearCaja(nuevoMapa->items, metadataIdentificador, pos_x, pos_y, cant_de_pokemones_en_pokenest);
	}
	else
	{
		//TODO: errorSintacticoSemantico fuera del mapa
		log_error(log, "errorSintacticoSemantico fuera del mapa");
		exit(EXIT_FAILURE);
	}

}


void cargarEntrenador (t_list* items, char simbolo)
{
	//TODO: Consultar si el simbolo de 1 entrenador es unico en un mapa (entiendo que si)

	if (estaDentroDelMargenDelMapa(__entrenadorPosInicialEnX,__entrenadorPosInicialEnY) ==1 )
	{
		CrearPersonaje(items, simbolo, __entrenadorPosInicialEnX, __entrenadorPosInicialEnY);
		return;
	}
	else
	{
		//TODO: errorSintacticoSemantico fuera del mapa
		exit(EXIT_FAILURE);
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
		exit(EXIT_FAILURE);
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
	//TODO: chequear la longitud del string, por ejemplo si es de 10 y le ingreso 15 tengo problemas...
	char directorioMapa[250];
	strcpy (directorioMapa, nuevoMapa->directorioPokeDex);


	strcat (directorioMapa, __ubicacionMapas );
	strcat (directorioMapa, nuevoMapa->nombre);
	strcat (directorioMapa, __ubicacionMetadataMapas );


	t_config * metadataMapa = config_create(directorioMapa);

	if (metadataMapa == NULL || config_keys_amount(metadataMapa) < 0 )
	{
		//TODO: errorSintacticoSemantico no se pudo levantar el archivo config
		exit(EXIT_FAILURE);
	}

	return metadataMapa;
}


uint16_t configLeerInt (t_config * archivoConfig, char nombreDeLaPropiedad[50])
{
	//leo ints
	if (config_has_property(archivoConfig, nombreDeLaPropiedad))
	{
		return config_get_int_value(archivoConfig, nombreDeLaPropiedad);
	}
	else
	{
		//TODO: errorSintacticoSemantico no se pudo levantar el archivo config
		exit(EXIT_FAILURE);
	}
}


char * configLeerString (t_config * archivoConfig, char nombreDeLaPropiedad[50])
{
	//leo strings
	if (config_has_property(archivoConfig, nombreDeLaPropiedad))
	{
		return config_get_string_value(archivoConfig, nombreDeLaPropiedad);
	}
	else
	{
		//TODO: errorSintacticoSemantico no se pudo levantar el archivo config
		exit(EXIT_FAILURE);
	}
}

void leerRecursivamenteLasPokenest (t_mapa * nuevoMapa)
{


	//voy a recorrer la carpeta de las pokenest y revisar cuantas hay!

    //TODO: chequear la longitud del string, por ejemplo si es de 10 y le ingreso 15 tengo problemas...
    char directorioMapa[250];
    strcpy (directorioMapa, nuevoMapa->directorioPokeDex);


    strcat (directorioMapa, "Mapas/" );
    strcat (directorioMapa, nuevoMapa->nombre);
    strcat (directorioMapa, __ubicacionDirPokenest );


    buscamePokeNestEnEsteDirectorio (directorioMapa, nuevoMapa);


}

static void buscamePokeNestEnEsteDirectorio (const char * nombreDirectorio, t_mapa * nuevoMapa)
{


	DIR * d;
    d = opendir (nombreDirectorio);
	//Reviso si lo pudo abrir
    if (d == NULL)
	{
		//TODO: loguear error no se pudo abrir el directorio.
    	puts (nombreDirectorio);
		exit (EXIT_FAILURE);
	}

	while (1)
	{
		struct dirent * entry;
		const char * d_name;

		//leemos el contenido del directorio d.
	   	entry = readdir (d);
	   	if (entry == NULL)
	   	{
	   		//No hay mas entradas
	       	break;
		}

	   	d_name = entry->d_name;
	   	//solo listo los directorios (tipo=DT_DIR)
        if (entry->d_type & DT_DIR)
        {
        	//reviso que no sean los directorios "." y ".."

        	if ( ( strcmp (d_name, "..") != 0 ) && (strcmp (d_name, ".") != 0) )
        	{
        		int path_length;
	           	char path[PATH_MAX];

	           	path_length = snprintf (path, PATH_MAX, "%s/%s", nombreDirectorio, d_name);

	           	//printf ("%s\n", path);	cargarPokenest
	           	levantarConfigPokeNest(path, nuevoMapa);

	            if (path_length >= PATH_MAX)
	            {
	               	fprintf (stderr, "Path length has got too long.\n");
	                    exit (EXIT_FAILURE);
	            }

	            //recorremos recursivamente archivos dentro de este directorio
	            buscamePokeNestEnEsteDirectorio (path, nuevoMapa);
        	}
		}
	}
	//cerramos el directorio
	if (closedir (d))
	{
		//fprintf (stderr, "Could not close '%s': %s\n",
		//         dir_name, strerror (errno));
		fprintf (stderr, "Error al cerrar un directorio.\n");
		exit (EXIT_FAILURE);
	}
}

void levantarConfigPokeNest (char * nombreDirectorio, t_mapa * nuevoMapa)
{
	//TODO: chequear la longitud del string, por ejemplo si es de 10 y le ingreso 15 tengo problemas...
	char directorioMapa[250];
	strcpy (directorioMapa, nombreDirectorio);
	strcat (directorioMapa, __ubicacionMetadataPokeNest );

	t_config * metadataPokenest = config_create(nombreDirectorio);

	if (metadataPokenest == NULL || config_keys_amount(metadataPokenest) < 0 )
	{
		//TODO: errorSintacticoSemantico no se pudo levantar el archivo config
		exit(EXIT_FAILURE);
	}

	cargarPokeNests(metadataPokenest, nuevoMapa);

	metadata_finalizar (metadataPokenest);
}

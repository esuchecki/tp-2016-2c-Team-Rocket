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

//agreagado de librerias utn.so
#include <nivel.h>
#include <tad_items.h>		//contiene una lista con los items a dibujar en el mapa
#include <curses.h>
#include <commons/collections/list.h>
#include <commons/config.h>


//------------------------------------------//
/* ********************************************	*/
//----------- Sector Funciones -------------//


/*
 * @NAME: dibujarMapa
 * @DESC: dibuja el mapa, recibe un nombre.
 */
void dibujarMapa ();

/*
 * @NAME: borrarMapa
 * @DESC: borra el mapa, recibe la lista de items.
 */
void borrarMapa (t_list* items);

/*
 * @NAME: cargarPokeNests
 * @DESC: carga las PokeNests de este mapa.
 */
void cargarPokeNests (t_list* items);

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


//------------------------------------------//
/* ********************************************	*/
//----------- Sector Constantes -------------//


#define __pokeNestSeparacionEnX (2)
#define __pokeNestSeparacionEnY (2)
#define __entrenadorPosInicialEnX (1)
#define __entrenadorPosInicialEnY (1)


//------------------------------------------//





int main(void) {
	//TODO: recibir 2 parametros (nombre y ruta del pokedex)

	puts("PROCESO MAPA"); /* prints PROCESO MAPA */
	//TODO: consultar el nombre de proceso, creo que es "mapa"


	//char nombreMapa[10] = "Prueba";
	//strcopy (miNombre, "Prueba");

	t_list* items = list_create();

	nivel_gui_inicializar();
	dibujarMapa (items, "NOMBRE_MAPA");
	borrarMapa (items);
	nivel_gui_terminar();




	return EXIT_SUCCESS;
}


void dibujarMapa (t_list* items, char nombreMapa[])
{


	//entrenador de prueba por el momento
	cargarEntrenador(items, '#');
	cargarEntrenador(items, '$');
	cargarEntrenador(items, '@');

	moverEntrenador(items, '#', 5,5);



	cargarPokeNests (items);



	nivel_gui_dibujar(items, nombreMapa);

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

void borrarMapa (t_list* items)
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

void cargarPokeNests (t_list* items)
{
	//TODO: levantar datos de un .config:
		/*		/Mapas/[nombre]/PokeNests/XXXXXX/				-> Las XXXX es un Subdirectorio con el nombre de la pokenest, ej: pikachu.
		 * 									..../metadata		-> archivo con contenido (sobre como graficar pokenest en pantalla)
		 * 									..../pokemon.dat 	-> archivo con lo unico importante el lvl del pokemon (hay un .dat x cada pokemon)
		 */

	//TODO: pensar en usar un struct. Ver si Struct solo para pokenest o en gral.

	//TODO: levantar dinamico con .config estos datos.
		/*		reemplazar 'H' por metadata -> Identificador.
		 * 		reemplazar
		 */
	char metadataIdentificador = 'P';	//pikachu
	int pos_x = 10;
	int pos_y = 10;
	int cant_de_pokemones_en_pokenest = 4;	// -> Hay que contar cuantos archivos hay exeptuando el de metadata.

	CrearCaja(items, 'A', 13, 13, cant_de_pokemones_en_pokenest);

	//TODO: validar si la pokeNest tiene pokemon de una unica especie..)
	//TODO: consultar si es unico el identificador de pokenest x mapa..
	if ( estaDentroDelMargenDelMapa(pos_x, pos_y) && esValidoElEspaciadoDePokeNest(items, pos_x, pos_y) )
	{
		CrearCaja(items, metadataIdentificador, pos_x, pos_y, cant_de_pokemones_en_pokenest);
	}
	else
	{
		//TODO: errorSintacticoSemantico fuera del mapa
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

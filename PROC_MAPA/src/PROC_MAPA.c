/*
 ============================================================================
 Name        : PROC_MAPA.c
 Author      : TEAM ROCKET
 Version     :
 Copyright   : TEAM ROCKET
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//agreagar libreria de nivel

#include <nivel.h>
#include <tad_items.h>
#include <stdlib.h>
#include <curses.h>
#include <commons/collections/list.h>


/*
 * @NAME: dibujarMapa
 * @DESC: dibuja el mapa, recibe un nombre.
 */
void dibujarMapa ();


int main(void) {
	//TODO: recibir 2 parametros (nombre y ruta del pokedex)

	puts("PROCESO MAPA"); /* prints PROCESO MAPA */
	//TODO: consultar el nombre de proceso, creo que es "mapa"


	//char nombreMapa[10] = "Prueba";
	//strcopy (miNombre, "Prueba");

	dibujarMapa ();




	return EXIT_SUCCESS;
}


void dibujarMapa ()
{
	t_list* items = list_create();
	int rows, cols;

	nivel_gui_inicializar();
	nivel_gui_get_area_nivel(&rows, &cols);

	CrearPersonaje(items, '@', 2, 2);
	CrearPersonaje(items, '#', 5, 5);

	//CrearEnemigo(items, '1', ex1, ey1);
	//CrearEnemigo(items, '2', ex2, ey2);

	CrearCaja(items, 'H', 26, 10, 5);
	CrearCaja(items, 'M', 8, 15, 3);
	CrearCaja(items, 'F', 19, 9, 2);

	nivel_gui_dibujar(items, "HOLA");

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


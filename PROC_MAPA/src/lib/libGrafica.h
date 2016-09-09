/*
 * libGrafica.h
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#ifndef LIBGRAFICA_H_
#define LIBGRAFICA_H_




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
#include <commons/string.h>
#include <commons/log.h>


//agregados librerias propias
#include "libConfig.h"


//------------------------------------------//









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

t_log* myArchivoDeLog;

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
 * @NAME: leerRecursivamenteLasPokenest
 * @DESC: Funcion de llamado para comenzar a leer las pokenest
 */
void leerRecursivamenteLasPokenest (t_mapa * nuevoMapa);

/*
 * @NAME: levantarConfigPokeNest
 * @DESC: Recibe un directorio (que supuestamente contiene la metadata de una pokenest) y agrega la pokenest a los items del mapa

 */
void levantarConfigPokeNest (char * nombreDirectorio, t_mapa * nuevoMapa);

/*
 * @NAME: loguearEstructuraDelMapa
 * @DESC: Sube al log la info de lo que cargo el mapa (ip/puerto/algoritmo/cant de pokenest/etc..)
 */
void loguearEstructuraDelMapa(t_mapa * nuevoMapa);

/*
 * @NAME: finalizarGui
 * @DESC: Sale de la pantalla gui + exit (0).
 * ***Usar esta funcion para finalizar ante algun error..***
 */
void finalizarGui (t_mapa * mapa);
//------------------------------------------//












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


#define __nombreMetadataPokeNest "metadata"
#define __nombreEnConfigIdPokeNest "Identificador"
#define __nombreEnConfigPosicionPokeNest "Posicion"
#define __separadorEnConfigPosicionPokeNest ";"


//------------------------------------------//









#endif /* LIBGRAFICA_H_ */

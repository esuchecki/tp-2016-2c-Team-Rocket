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
#include <unistd.h>



//agreagado de librerias utn.so
#include <nivel.h>

#include <curses.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/log.h>


//agregados librerias propias
#include "mapaConfig.h"
#include "seniales.h"
#include "estructurasMapa.h"
#include "so/constantes.h"


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

/*
 * @NAME: esUnicoEsteIdentificador
 * @DESC: Devuelve true si el identificador, ej: '#', ya existe en la lista de items.
 */
bool esUnicoEsteIdentificador (t_list* items, char idDelItem);



/*
 * @NAME: tratarLaSenialRecibida
 * @DESC: Libera de memoria la estructura unMapa->metadata.
 */
void freeForMetadataMapa (t_mapa * unMapa);

/*
 * @NAME: accionDelMapaAnteSIGUSR2
 * @DESC: Cuando recibe la senial mencionada, ejecuta esta fc.
 */
void accionDelMapaAnteSIGUSR2 (t_mapa * unMapa);

/*
 * @NAME: dondeQuedaEstaPokeNest
 * @DESC: El int que devuelte es 0 en caso de error, 1 en caso de todo ok. La pos la devuelve en pos_x y en pos_y.
 */
int dondeQuedaEstaPokeNest (t_mapa * unMapa, char * idPokeNest, int * pos_x, int * pos_y);

/*
 * @NAME: inicializarCantDePokemonesEnPokeNest
 * @DESC: **fc de uso interno ** carga en el mapa, los pokemones de una pokenest y los inicializa asignados a '\0'.
 * Devuelve la cantidad total de recursos que tiene la pokenest.
 */
int inicializarCantDePokemonesEnPokeNest (char * nombreDirectorio, t_mapa * nuevoMapa, char identificador);

/*
 * @NAME: ejecutarLogicaContarPokemons
 * @DESC: **fc de uso interno ** ejecuta la logica de inicializarCantDePokemonesEnPokeNest;
 * Devuelve la cantidad total de recursos que tiene la pokenest.
 */
int ejecutarLogicaContarPokemons (char * nombrePokeNest, char * nombreDirectorio, t_mapa * nuevoMapa, char identificador);

/*
 * @NAME: distanciaEntrenadorPokenest
 * @DESC: Funcion utilizada por el planificador para conocer la distancia de un entrenador a una pokenest (SRDF).
 */
int distanciaEntrenadorPokenest (char simboloEntrenador, t_mapa * self, char pokeNest);

/*
 * @NAME: distanciaEntreObjetos
 * @DESC: fc auxiliar para calcular el modulo de la diferencia entre x1 y x2 -->> | x1 - x2 |
 */
int distanciaEntreObjetos (int pos_x1, int pos_x2);

/*
 * @NAME: encontrameEsteIdEnLaLista
 * @DESC: fc auxiliar. Es utilizada como closure para encontrar un ID en una lista. Devuelve un item.
 */
ITEM_NIVEL* encontrameEsteIdEnLaLista (t_mapa * self, char id);

/*
 * @NAME: borrarEntrenadorDelMapa
 * @DESC: fc utilizada por el planificador para borrar un entrenador del mapa.
 */
void borrarEntrenadorDelMapa(t_mapa * unMapa, char simboloEntrenador);
//------------------------------------------//










#endif /* LIBGRAFICA_H_ */

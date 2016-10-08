/*
 * batallaPkmn.h
 *
 *  Created on: 6/10/2016
 *      Author: utnso
 */

#ifndef LIB_BATALLAPKMN_H_
#define LIB_BATALLAPKMN_H_


#include <stdio.h>
#include <stdbool.h>
#include <pkmn/factory.h>	//si se borra el test, borrar este.
#include <pkmn/battle.h>
#include "estructurasMapa.h"
#include <commons/collections/list.h>


/*
 * @NAME: batallarDosPkmn
 * @DESC: Resuelve un conflicto entre 2 pokemons.
 *      -> Si en deadlock solo hay 2 entrenadores llamar a esta fc.
 *      ->  -> Tenes que llamar a free por tu cuenta.
 *
 *
 * @RETURN: devuelve el pokemon perdedor.
 */
t_pokemon * batallarDosPkmn(t_pokemon * pkmn1, t_pokemon * pkmn2);

/*
 * @NAME: batallarListaDePkmn
 * @DESC: Recibe una lista con t_pokemon cargados.
 *      -> Si en deadlock hay + de 2 entrenadores, llamar a esta fc.
 *      ->  -> Tenes que llamar a free por tu cuenta.
 *
 *
 * @RETURN: devuelve el pokemon perdedor.
 *
 * Nota: Esta fc se puede adaptar para recibir una lista de otro tipo (p, ej, que contenga al entrenador).
 */
t_pokemon * batallarListaDePkmn(t_list * listaPkmn);


/*
 * @NAME: testListaPkmn
 * @DESC: Una funcion que hace un test de este modulo.
 */
void testListaPkmn (); //si se borra el test, borrar <pkmn/factory.h>.



#endif /* LIB_BATALLAPKMN_H_ */

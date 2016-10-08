/*
 * batallaPkmn.c
 *
 *  Created on: 6/10/2016
 *      Author: utnso
 */


#include "batallaPkmn.h"


void loguearPkmn (t_pokemon * pkmn);



t_pokemon * batallarDosPkmn(t_pokemon * pkmn1, t_pokemon * pkmn2)
{
	log_info(myArchivoDeLog, "========Batalla!========");

	//Si el nombre del Pokémon no existe en los primeros 151 o está mal escrito
	//Retornará el puntero NULL (0x0)
	if (pkmn1 == NULL || pkmn2 == NULL)
	{
		log_error(myArchivoDeLog, "--> Me mandaron a batallar pokemones 'MissingNo'. ");
		return NULL;
	}


	loguearPkmn(pkmn1);
	log_info(myArchivoDeLog, "\t\t\t\tvs");
	loguearPkmn(pkmn2);

	//La batalla propiamente dicha
	t_pokemon * loser = pkmn_battle(pkmn1, pkmn2);

	log_info(myArchivoDeLog, "El Perdedor es: %s", loser->species);
	log_info(myArchivoDeLog, "************ Fin de Batalla! ************");


	//Como el puntero loser apunta a alguno de los otros 2, no se lo libera
	return loser;
}


t_pokemon * batallarListaDePkmn(t_list * listaPkmn)
{
	log_info(myArchivoDeLog, "Me dieron una lista de pokemones a batallar");
	if (listaPkmn->elements_count < 2)
	{
		log_error(myArchivoDeLog, "--> La lista esta vacia o no hay un minimo de 2 pokemones ");
		return NULL;
	}


	int flag = 0;
	void _funcionOrdenSuperiorQueQuieroEjecutar (t_pokemon * unPkmn)
	{
		if (unPkmn == NULL)
			flag =1;
	}
	list_iterate(listaPkmn, (void *) _funcionOrdenSuperiorQueQuieroEjecutar);



	//Si el nombre del Pokémon no existe en los primeros 151 o está mal escrito
	//Retornará el puntero NULL (0x0)
	if (flag != 0)
	{
		log_error(myArchivoDeLog, "--> Me mandaron a batallar pokemones 'MissingNo'. ");
		return NULL;
	}



	/* Voy a iterar la lista de pokemones en busca del perdedor */
	t_pokemon * loser = NULL;
	t_pokemon * temporal = NULL;


	int i=0;
	for (i=0; i< (listaPkmn->elements_count) ; i++)
	{
		if (i==0)
		{
			//era el primero.
			loser = list_get(listaPkmn,i);
			continue;
		}
		temporal = list_get(listaPkmn,i);
		loser = batallarDosPkmn (loser, temporal);
	}

	log_info(myArchivoDeLog, "Finaliza la batalla de listas de pokemones. El pkmn perdedor fue:");
	loguearPkmn(loser);

	return loser;
}


void loguearPkmn (t_pokemon * pkmn)
{
	log_info(myArchivoDeLog, "Pokemon: %s[%s/%s] Nivel: %d",
			pkmn->species,
			pkmn_type_to_string(pkmn->type),
			pkmn_type_to_string(pkmn->second_type),
			pkmn->level);
}

void testListaPkmn ()
{
	//Creamos una instancia de la Factory
	//La misma sirve para crear pokémons con solo el nombre y el nivel
	t_pkmn_factory* pokemon_factory = create_pkmn_factory();

	//Creamos un "Pikachu" de nivel 30.
	//Nótese que empieza con letra mayúscula y no debe tener errores de nombre
	t_pokemon * Pikachu = create_pokemon(pokemon_factory, "Pikachu", 40);
	t_pokemon * Rhyhorn = create_pokemon(pokemon_factory, "Rhyhorn", 21);
	t_pokemon * Rattata = create_pokemon(pokemon_factory, "Rattata", 80);
	t_pokemon * Onix = create_pokemon(pokemon_factory, "Onix", 20);
	t_pokemon * Seadra = create_pokemon(pokemon_factory, "Seadra", 20);
	t_pokemon * Magmar = create_pokemon(pokemon_factory, "Magmar", 25);
	t_pokemon * Dragonite = create_pokemon(pokemon_factory, "Dragonite", 60);
	t_pokemon * Nidoran = create_pokemon(pokemon_factory, "Nidoran♀", 2);


	t_list * listaDePrueba = list_create();

	list_add(listaDePrueba, Pikachu );
	list_add(listaDePrueba, Rhyhorn );
	list_add(listaDePrueba, Rattata );
	list_add(listaDePrueba, Onix );
	list_add(listaDePrueba, Seadra );
	list_add(listaDePrueba, Magmar );
	list_add(listaDePrueba, Dragonite );
	list_add(listaDePrueba, Nidoran );




	batallarListaDePkmn(listaDePrueba);


	//Liberemos los recursos
	//free(Pikachu);
	//free(Rhyhorn);
	//Hay que destruir la instancia de la Factory
	destroy_pkmn_factory(pokemon_factory);
	list_clean_and_destroy_elements(listaDePrueba, (void*) &free);
	return;
}


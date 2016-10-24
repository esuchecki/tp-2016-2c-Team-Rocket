/*
 * batallaPkmn.c
 *
 *  Created on: 6/10/2016
 *      Author: utnso
 */


#include "batallaPkmn.h"


/*
 * @NAME: batallarDosEntrenadores
 * @DESC: Resuelve un conflicto entre 2 entrenadores, batallando sus pkmn.
 *      -> Si en deadlock solo hay 2 entrenadores llamar a esta fc.
 *      ->  -> Tenes que llamar a free por tu cuenta.
 *
 *
 * @RETURN: devuelve el pokemon perdedor.
 */
t_entrenador * batallarDosEntrenadores(t_entrenador * entrenador1, t_entrenador * entrenador2);

/*
 * @NAME: loguearPkmn
 * @DESC: Muestra por log el pkmn del entrenador a batallar.
 */
void loguearPkmn (t_entrenador * unEntrenador);



t_entrenador * batallarDosEntrenadores(t_entrenador * entrenador1, t_entrenador * entrenador2)
{
	log_info(myArchivoDeLog, "========Batalla!========");

	//Si el nombre del Pokémon no existe en los primeros 151 o está mal escrito
	//Retornará el puntero NULL (0x0)
	if (entrenador1 == NULL || entrenador2 == NULL || entrenador1->mejorPokemon== NULL || entrenador2->mejorPokemon ==NULL)
	{
		log_error(myArchivoDeLog, "--> Me mandaron a batallar entrenadores NULL o pokemones 'MissingNo'. ");
		return NULL;
	}


	loguearPkmn(entrenador1);
	log_info(myArchivoDeLog, "\t\t\t\tvs");
	loguearPkmn(entrenador2);

	//La batalla propiamente dicha
	t_pokemon * loser = pkmn_battle(entrenador1->mejorPokemon, entrenador2->mejorPokemon);

	log_info(myArchivoDeLog, "El Perdedor es: %s", loser->species);
	log_info(myArchivoDeLog, "************ Fin de Batalla! ************");


	//Como el puntero loser apunta a alguno de los otros 2, no se lo libera
	if (entrenador1->mejorPokemon == loser)
		return entrenador1;
	else
		return entrenador2;
}


t_entrenador * batallarListaDePkmn(t_list * listaEntrenadores)
{
	log_info(myArchivoDeLog, "Me dieron una lista de entreandores a batallar con sus pkmn.");
	if (listaEntrenadores->elements_count < 2)
	{
		log_error(myArchivoDeLog, "--> La lista esta vacia o no hay un minimo de 2 entrenadores ");
		return NULL;
	}


	int flag = 0;
	void _funcionOrdenSuperiorQueQuieroEjecutar (t_entrenador * unEntrenador)
	{
		if (unEntrenador== NULL || unEntrenador->mejorPokemon == NULL)
			flag =1;
	}
	list_iterate(listaEntrenadores, (void *) _funcionOrdenSuperiorQueQuieroEjecutar);



	//Si el nombre del Pokémon no existe en los primeros 151 o está mal escrito
	//Retornará el puntero NULL (0x0)
	if (flag != 0)
	{
		log_error(myArchivoDeLog, "--> Me mandaron a batallar entrenadores NULL o pokemones 'MissingNo'. ");
		return NULL;
	}



	/* Voy a iterar la lista de pokemones en busca del perdedor */
	t_entrenador * loser = NULL;
	t_entrenador * temporal = NULL;


	int i=0;
	for (i=0; i< (listaEntrenadores->elements_count) ; i++)
	{
		if (i==0)
		{
			//era el primero.
			loser = list_get(listaEntrenadores,i);
			continue;
		}
		temporal = list_get(listaEntrenadores,i);
		loser = batallarDosEntrenadores (loser, temporal);
	}

	log_info(myArchivoDeLog, "Finaliza la batalla de listas de pokemones. El pkmn perdedor fue:");
	loguearPkmn(loser);

	return loser;
}


void loguearPkmn (t_entrenador * entrenador)
{
	log_info(myArchivoDeLog, "Pokemon: %s[%s/%s] Nivel: %d \t\t| Entrenador:%c",
			entrenador->mejorPokemon->species,
			pkmn_type_to_string(entrenador->mejorPokemon->type),
			pkmn_type_to_string(entrenador->mejorPokemon->second_type),
			entrenador->mejorPokemon->level,
			entrenador->simbolo);
}

void testListaPkmn ()
{
	//Creamos una instancia de la Factory
	//La misma sirve para crear pokémons con solo el nombre y el nivel
	t_pkmn_factory* pokemon_factory = create_pkmn_factory();

	//Creamos un "Pikachu" de nivel 30.
	//Nótese que empieza con letra mayúscula y no debe tener errores de nombre
	t_pokemon * Pikachu = create_pokemon(pokemon_factory, "Pikachu", 40);
		t_entrenador * entrenador1 = malloc(sizeof(t_entrenador));
		entrenador1->simbolo = 'A';
		entrenador1->mejorPokemon = Pikachu;
	t_pokemon * Rhyhorn = create_pokemon(pokemon_factory, "Rhyhorn", 21);
		t_entrenador * entrenador2 = malloc(sizeof(t_entrenador));
		entrenador2->simbolo = 'B';
		entrenador2->mejorPokemon = Rhyhorn;
	t_pokemon * Rattata = create_pokemon(pokemon_factory, "Rattata", 80);
		t_entrenador * entrenador3 = malloc(sizeof(t_entrenador));
		entrenador3->simbolo = 'C';
		entrenador3->mejorPokemon = Rattata;
	t_pokemon * Onix = create_pokemon(pokemon_factory, "Onix", 20);
		t_entrenador * entrenador4 = malloc(sizeof(t_entrenador));
		entrenador4->simbolo = 'D';
		entrenador4->mejorPokemon = Onix;
	t_pokemon * Seadra = create_pokemon(pokemon_factory, "Seadra", 20);
		t_entrenador * entrenador5 = malloc(sizeof(t_entrenador));
		entrenador5->simbolo = 'E';
		entrenador5->mejorPokemon = Seadra;
	t_pokemon * Magmar = create_pokemon(pokemon_factory, "Magmar", 25);
		t_entrenador * entrenador6 = malloc(sizeof(t_entrenador));
		entrenador6->simbolo = 'F';
		entrenador6->mejorPokemon = Magmar;
	t_pokemon * Dragonite = create_pokemon(pokemon_factory, "Dragonite", 60);
		t_entrenador * entrenador7 = malloc(sizeof(t_entrenador));
		entrenador7->simbolo = 'G';
		entrenador7->mejorPokemon = Dragonite;
	t_pokemon * Nidoran = create_pokemon(pokemon_factory, "Nidoran♀", 2);
		t_entrenador * entrenador8 = malloc(sizeof(t_entrenador));
		entrenador8->simbolo = 'H';
		entrenador8->mejorPokemon = Nidoran;


	t_list * listaDePrueba = list_create();

	list_add(listaDePrueba, entrenador1 );
	list_add(listaDePrueba, entrenador2 );
	list_add(listaDePrueba, entrenador3 );
	list_add(listaDePrueba, entrenador4 );
	list_add(listaDePrueba, entrenador5 );
	list_add(listaDePrueba, entrenador6 );
	list_add(listaDePrueba, entrenador7 );
	list_add(listaDePrueba, entrenador8 );




	batallarListaDePkmn(listaDePrueba);


	//Liberemos los recursos
	free(Pikachu);
	free(Rhyhorn);
	free(Rattata);
	free(Onix);
	free(Seadra);
	free(Magmar);
	free(Dragonite);
	free(Nidoran);
	//Hay que destruir la instancia de la Factory
	destroy_pkmn_factory(pokemon_factory);
	list_clean_and_destroy_elements(listaDePrueba, (void*) &free);
	return;
}


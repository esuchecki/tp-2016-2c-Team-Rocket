/*
 * batallaPkmn.c
 *
 *  Created on: 6/10/2016
 *      Author: utnso
 */

#include "batallaPkmn.h"



t_list * crearListaDePkmnEnDirDeBill (t_entrenadorFisico * unEntrenador);
t_pokemon* crearPokemon (t_entrenadorFisico * unEntrenador, char * pokemonName, int * nivelPkmn);
char * ubicacionDirDeBill(t_entrenadorFisico * unEntrenador);
t_pokemon* cualEsElMejorPkmnDeEstaLista(t_list * lista_pokemon, t_entrenadorFisico * nuevoEntrenador);
t_config * config_create_for_pkmnEnDirDeBill(t_entrenadorFisico * nuevoEntrenador, char * pokemon);





t_pokemon * elegirMejorPokemon (t_entrenadorFisico * unEntrenador)
{
	log_info(myArchivoDeLog, "Voy a buscar mi mejor pokemon en el directorio de bill");
	t_list * lista_pokemon = crearListaDePkmnEnDirDeBill(unEntrenador);
	//t_pokemon * pokemonFactorya;
	t_pokemon * pokemonFactorya = cualEsElMejorPkmnDeEstaLista(lista_pokemon, unEntrenador);


	if (pokemonFactorya == NULL)
	{
		log_error(myArchivoDeLog, "El Pokémon MissingNo no existe! El puntero de retorno de la factory fue: %p", pokemonFactorya);
		finalizarEntrenador(unEntrenador);
	}

	return pokemonFactorya;
}



t_list * crearListaDePkmnEnDirDeBill (t_entrenadorFisico * unEntrenador)
{
	t_list * lista_pokemon = list_create();

	//esta funcion me compara si el archivo actual es un archivo llamado pokemonNNNdat. Si es asi, sumo +1.
	void _funcionOrdenSuperiorQueQuieroEjecutar (const char * d_name,const char * nombreDirectorio)
	{

		//Pregunto si el archivo actual termina en "*.dat"
		if ( (string_ends_with(d_name,".dat")) != 0 )
		{
			log_info(myArchivoDeLog,"Encontre en Dir De Bill un: %s", d_name);
			char * pokemon = string_duplicate(d_name);
			list_add(lista_pokemon, pokemon);
		}

	}


	if ( ( encontrarEnUnDirectorio ( ubicacionDirDeBill(unEntrenador),  (void *) _funcionOrdenSuperiorQueQuieroEjecutar ) )==0 )
	{
		log_error(myArchivoDeLog, "problemas en 'encontrarEnUnDirectorio' y la '_funcionOrdenSuperiorQueQuieroEjecutar'");
		finalizarEntrenador(unEntrenador);
	}

	return lista_pokemon;
}



t_pokemon* crearPokemon (t_entrenadorFisico * unEntrenador, char * pokemonName, int * nivelPkmn)
{
	//Creamos una instancia de la Factory
	//La misma sirve para crear pokémons con solo el nombre y el nivel
	t_pkmn_factory* pokemon_factory = create_pkmn_factory();


	//El -7 es la cantidad de caracteres que no son del nombre del pokemon "*NNN.dat" = 7
	pokemonName = string_substring(pokemonName, 0, (string_length(pokemonName) - 7));


	//Hago que el nombre sea primer letra mayuscula y lo demas minuscula (como pide la libreria)
	string_capitalized(pokemonName);


	t_pokemon * nuevoPokemon = create_pokemon(pokemon_factory, pokemonName, *nivelPkmn);
	if (nuevoPokemon == NULL)
	{
		log_error(myArchivoDeLog, "Mi mejor pkmn es un puntero a NULL (MissingNo). ");
		finalizarEntrenador(unEntrenador);
	}
	log_info(myArchivoDeLog, "Mi mejor pkmn es: %s[%s/%s] Nivel: %d", nuevoPokemon->species, pkmn_type_to_string(nuevoPokemon->type), pkmn_type_to_string(nuevoPokemon->second_type), nuevoPokemon->level );
	//free(pokemonName);
	free(nivelPkmn);
	//Hay que destruir la instancia de la Factory
	destroy_pkmn_factory(pokemon_factory);

	return nuevoPokemon;
}


char * ubicacionDirDeBill(t_entrenadorFisico * unEntrenador)
{
	char * unDirectorioNuevo;
	asprintf(&unDirectorioNuevo, "/%s/%s/%s/%s/", unEntrenador->directorioPokeDex, __ubicacionEntrenadores, unEntrenador->nombre, __ubicacionDirDeBill );

	log_debug(myArchivoDeLog, "%s", unDirectorioNuevo);
	log_debug(myArchivoDeLog, "%s", string_itoa(string_length(unDirectorioNuevo)));

	return unDirectorioNuevo;
}


t_pokemon* cualEsElMejorPkmnDeEstaLista(t_list * lista_pokemon, t_entrenadorFisico * nuevoEntrenador)
{
	char * nombreMejorPkmn;
	nombreMejorPkmn = NULL;
	int * nivelMejorPkmn = malloc (sizeof(int));
	*nivelMejorPkmn = -1;

	void _funcionOrdenSuperiorQueQuieroEjecutar (char * pokemon)
	{
		t_config * temporal = config_create_for_pkmnEnDirDeBill(nuevoEntrenador, pokemon);
		int nivelPkmn= _entrenador_configLeerInt(temporal, __nombreEnConfigNivelPkmn, nuevoEntrenador);
		metadata_finalizar(temporal);

		if (nivelPkmn > *nivelMejorPkmn)
		{
			*nivelMejorPkmn = nivelPkmn;
			//free(nombreMejorPkmn);
			nombreMejorPkmn = string_duplicate(pokemon);
		}
	}


	list_iterate(lista_pokemon, (void *) _funcionOrdenSuperiorQueQuieroEjecutar);
	if ( (nombreMejorPkmn != NULL) && (*nivelMejorPkmn != -1) )
	{
		return crearPokemon (nuevoEntrenador, nombreMejorPkmn, nivelMejorPkmn);
	}

	return NULL;
}


t_config * config_create_for_pkmnEnDirDeBill(t_entrenadorFisico * nuevoEntrenador, char * pokemon)
{
	char * directorioPokemon;
	asprintf(&directorioPokemon, "%s/%s", ubicacionDirDeBill(nuevoEntrenador), pokemon);

	//log_debug(myArchivoDeLog, "fdsafsa: %s", directorioPokemon);
	t_config * metadataEntrenador = _entrenador_newConfigType(directorioPokemon, nuevoEntrenador);
	free (directorioPokemon);


	return metadataEntrenador;
}




/*
 * batallaPkmn.c
 *
 *  Created on: 6/10/2016
 *      Author: utnso
 */


#include "batallaPkmn.h"


/*
int main(int argc, char **argv) {

  //Creamos una instancia de la Factory
  //La misma sirve para crear pokémons con solo el nombre y el nivel
  t_pkmn_factory* pokemon_factory = create_pkmn_factory();

  //Creamos un "Pikachu" de nivel 30.
  //Nótese que empieza con letra mayúscula y no debe tener errores de nombre
  t_pokemon * pikachu = create_pokemon(pokemon_factory, "Pikachu", 30);
  t_pokemon * rhyhorn = create_pokemon(pokemon_factory, "Rhyhorn", 6);

  //Si el nombre del Pokémon no existe en los primeros 151 o está mal escrito
  //Retornará el puntero NULL (0x0)
  t_pokemon * missigno = create_pokemon(pokemon_factory, "MissingNo", 128);
  printf("El Pokémon MissingNo no existe! El puntero de retorno de la factory fue: %p\n", missigno);

  printf("========Batalla!========\n");
  printf("Primer Pokemon: %s[%s/%s] Nivel: %d\n",
    pikachu->species, pkmn_type_to_string(pikachu->type),
    pkmn_type_to_string(pikachu->second_type), pikachu->level);
  printf("Segundo Pokemon: %s[%s/%s] Nivel: %d\n",
    rhyhorn->species, pkmn_type_to_string(rhyhorn->type),
    pkmn_type_to_string(rhyhorn->second_type), rhyhorn->level); //Función que sirve para ver el Tipo de Enum como un String

  //La batalla propiamente dicha
  t_pokemon * loser = pkmn_battle(pikachu, rhyhorn);

  printf("El Perdedor es: %s\n", loser->species);

  //Liberemos los recursos
  //Como el puntero loser apunta a alguno de los otros 2, no se lo libera
  free(pikachu);
  free(rhyhorn);
  //Hay que destruir la instancia de la Factory
  destroy_pkmn_factory(pokemon_factory);
  return 0;
}


void elegirMejorPokemon ()
{

}

const void crearPokemon (t_pkmn_factory* pokemon_factory, char * pokemonName)
{

}
*/

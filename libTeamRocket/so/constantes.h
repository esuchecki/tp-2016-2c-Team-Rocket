/*
 * constantes.h
 *
 *  Created on: 23/9/2016
 *      Author: utnso
 */

#ifndef SO_CONSTANTES_H_
#define SO_CONSTANTES_H_



//-------------------------------------//
/* ************************************	*/
//-------------- MAPA  ----------------//

#define __ubicacionArchivoDeLogMapa "./logMapa_%s"		//%s = nombre del entrenador

#define __pokeNestSeparacionEnX (2)		// el enunciado dice que 2 pokenest deben estar separadas en el eje x, unas posiciones
#define __pokeNestSeparacionEnY (2)		// idem para el eje y
#define __entrenadorPosInicialEnX (1) 	// que posicion de la gui inicia el jugador en el eje x
#define __entrenadorPosInicialEnY (1)	// idem para el eje y

#define __ubicacionMapas "/Mapas/"
#define __ubicacionMetadataMapas "/metadata"
#define __ubicacionDirPokenest "/PokeNests/"
#define __ubicacionMetadataPokeNest "/metadata"

#define __ubicacionPokemonesPokeNest "%s%03d.dat"	//  1º %s NombrePokeNest | 2º %03d = NNN (Actual) - Muestra 3 enteros con 0s.
#define __valorInicialPokemonesPokenest (1)
#define __valorFinalPokemonesPokenest (999)


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


#define __nombreEnConfigNivelPkmn "Nivel"

//-------------------------------------//





//-------------------------------------//
/* ************************************	*/
//----------- ENTRENADOR  -------------//

#define __ubicacionArchivoDeLogEntrenador "./logEntrenador_%s"	//%s = nombre del entrenador

#define __ubicacionEntrenadores "/Entrenadores/"
#define __ubicacionMetadataEntrenadores "/metadata"
#define __ubicacionDirDeBill "/Dir de Bill/"

#define __nombreEnConfig_Vidas "vidas"
#define __nombreEnConfig_Reintentos "reintentos"
#define __nombreEnConfig_Simbolo "simbolo"
#define __nombreEnConfig_HojaViajes "hojaDeViaje"
#define __nombreEnConfig_ObjMapa "obj[%s]"	//%s corresponde al nombre del mapa.

//-------------------------------------//




#endif /* SO_CONSTANTES_H_ */

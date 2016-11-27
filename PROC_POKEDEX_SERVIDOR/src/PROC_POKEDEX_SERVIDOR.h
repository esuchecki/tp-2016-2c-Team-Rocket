/*
 * PROC_POKEDEX_SERVIDOR.h
 *
 *  Created on: 26/10/2016
 *      Author: utnso
 */

#ifndef PROC_POKEDEX_SERVIDOR_H_
#define PROC_POKEDEX_SERVIDOR_H_

#include <commons/log.h>




/**
 * @NAME: InicializarLogServidor
 * @DESC: Esto inicializa el log del servidor que se guarda en una variable global para que lo pueda usar desde las conexiones
 * y fuse, el parametro consolaOn se setea true o false dependiendo si se quiere o no que tambien se vea por consola aparte del archivo
 */
void inicializarLogServidor ( char *argv[],bool consolaOn );


#endif /* PROC_POKEDEX_SERVIDOR_H_ */

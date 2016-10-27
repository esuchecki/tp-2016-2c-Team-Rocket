/*
 * PROC_POKEDEX_CLIENTE.h
 *
 *  Created on: 27/10/2016
 *      Author: utnso
 */

#ifndef PROC_POKEDEX_CLIENTE_H_
#define PROC_POKEDEX_CLIENTE_H_

#include <commons/log.h>

t_log * logCliente;

#define pathConfigCli "/home/utnso/git/tp-2016-2c-Team-Rocket/PROC_POKEDEX_CLIENTE/config_cliente.txt"

/**
 * @NAME: InicializarLogCliente
 * @DESC: Esto inicializa el log del cliente que se guarda en una variable global para que lo pueda usar desde las conexiones
 * y fuse, el parametro consolaOn se setea true o false dependiendo si se quiere o no que tambien se vea por consola aparte del archivo
 */
void inicializarLogCliente ( char *argv[],bool consolaOn );


#endif /* PROC_POKEDEX_CLIENTE_H_ */

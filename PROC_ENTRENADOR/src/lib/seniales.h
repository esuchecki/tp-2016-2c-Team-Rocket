/*
 * libSeniales.h
 *
 *  Created on: 17/9/2016
 *      Author: utnso
 */

#ifndef LIB_SENIALES_H_
#define LIB_SENIALES_H_





#include <signal.h>

//agreagado de librerias utn.so
#include <commons/log.h>

//incluyo mi variable externa para el log!
#include "estructurasEntrenador.h"




/*
 * @NAME: inicializarSenialesMapa
 * @DESC: Inicia una rutina para revisar si el programa recibio alguna senial.. En caso de error abortivo, cierra correctamente la gui con la fc y el parametro unEntrenador.
 *
 * NOTA: si llamas a este metodo, tu proxima linea deberia ser funcionesQueQuieroEjecutarSegunLaSenial!!!
 */
void inicializarSenialesEntrenador (t_entrenadorFisico * unEntrenador, void (*fc) (t_entrenadorFisico *));

/*
 * @NAME: funcionesQueQuieroEjecutarSegunLaSenial
 * @DESC: Inicia una rutina para revisar si el programa recibio alguna senial.. En caso de error abortivo, cierra correctamente la gui con la fc y el parametro unEntrenador.
 *
 * NOTA: si llamas a este metodo, en tu linea anterior deberias haber llamado a  revisarRecepcionDeSeniales!!!
 */
void funcionesQueQuieroEjecutarSegunLaSenial (t_entrenadorFisico * unEntrenador, void (*fcAbortiva) (t_entrenadorFisico *), void (*fcSIGUSR1) (t_entrenadorFisico *), void (*fcSIGTERM) (t_entrenadorFisico *) );

/*
 * @NAME: tratarLaSenialRecibida
 * @DESC: uso exclusivo de revisarRecepcionDeSeniales
 */
void tratarLaSenialRecibida (int senial);




//flags para saber que señal fue recibida.
short int _SIGINT_flag;
short int _SIGUSR1_flag;
short int _SIGTERM_flag;






#endif /* LIB_SENIALES_H_ */

/*
 * libSeniales.c
 *
 *  Created on: 17/9/2016
 *      Author: utnso
 */
#include "seniales.h"
#include "abortarEntrenador.h"

void inicializarSenialesEntrenador (t_entrenadorFisico * unEntrenador)
{
	_SIGINT_flag = 0;	//reseteo el flag.
	_SIGTERM_flag = 0;	//reseteo el flag.
	_SIGUSR1_flag = 0;	//reseteo el flag.

	if (signal( SIGUSR1, tratarLaSenialRecibida ) == SIG_ERR)
	{
		log_error(myArchivoDeLog, "error al setear la senial SIGUSR1");
		finalizarEntrenador(unEntrenador);
	}
	if (signal( SIGTERM, tratarLaSenialRecibida)==SIG_ERR)
	{
		log_error(myArchivoDeLog, "error al setear la senial SIGTERM");
		finalizarEntrenador(unEntrenador);
	}
//	if (signal(SIGINT, tratarLaSenialRecibida)==SIG_ERR)
//	{
//		log_error(myArchivoDeLog, "error al setear la senial SIGINT");
//		finalizarEntrenador(unEntrenador);
//	}
}


void tratarLaSenialRecibida (int senial)
{
	switch (senial)
	{
		case SIGINT:
			log_warning(myArchivoDeLog,"Recibi la senial SIGINT." );
			_SIGINT_flag = 1;
			break;

		case SIGUSR1:
			log_warning(myArchivoDeLog,"Recibi la senial SIGUSR1." );
			_SIGUSR1_flag ++;
			break;

		case SIGTERM:
			log_warning(myArchivoDeLog,"Recibi la senial SIGTERM." );
			_SIGTERM_flag ++;
			break;
	}

}

void funcionesQueQuieroEjecutarSegunLaSenial (t_entrenadorFisico * unEntrenador, void (*fcSIGUSR1) (t_entrenadorFisico *), void (*fcSIGTERM) (t_entrenadorFisico *, bool) )
{
	//TODO: revisar si hay que hacer alguna otra cosa para 'encolar' señales o enrealidad bloquearlas (si no quiero que me interrumpan algo)


	//Primero cierro correctamente al entrenador
	if (_SIGINT_flag == 1)
	{
		log_warning(myArchivoDeLog,"**Voy a tratar la senial SIGINT.**" );
		finalizarEntrenador(unEntrenador);
		_SIGINT_flag = 0;	//reseteo el flag.
	}

	//Resto vidas
	if (_SIGTERM_flag > 0)
	{
		log_warning(myArchivoDeLog,"**Voy a tratar la senial SIGTERM.**" );
		fcSIGTERM(unEntrenador, false);
		_SIGTERM_flag = 0;	//reseteo el flag.
	}


	//sumar vidas al entrenador
	if (_SIGUSR1_flag > 0)
	{
		log_warning(myArchivoDeLog,"**Voy a tratar la senial SIGUSR1.**" );
		fcSIGUSR1(unEntrenador);
		_SIGUSR1_flag = 0;	//reseteo el flag.
	}

}

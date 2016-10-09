/*
 * libSeniales.c
 *
 *  Created on: 17/9/2016
 *      Author: utnso
 */
#include "seniales.h"

void inicializarSenialesEntrenador (t_entrenadorFisico * unEntrenador, void (*fc) (t_entrenadorFisico *))
{
	_SIGINT_flag = 0;	//reseteo el flag.
	_SIGTERM_flag = 0;	//reseteo el flag.
	_SIGUSR1_flag = 0;	//reseteo el flag.

	if (signal( SIGUSR1, tratarLaSenialRecibida ) == SIG_ERR)
	{
		log_error(myArchivoDeLog, "error en la senial SIGUSR1");
		fc(unEntrenador);
	}
	if (signal( SIGTERM, tratarLaSenialRecibida)==SIG_ERR)
	{
		log_error(myArchivoDeLog, "error en la senial SIGTERM");
		fc(unEntrenador);
	}
	if (signal(SIGINT, tratarLaSenialRecibida)==SIG_ERR)
	{
		log_error(myArchivoDeLog, "error en la senial SIGINT");
		fc(unEntrenador);
	}
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
			_SIGUSR1_flag = 1;
			break;

		case SIGTERM:
			log_warning(myArchivoDeLog,"Recibi la senial SIGTERM." );
			_SIGTERM_flag = 1;
			break;
	}

}

void funcionesQueQuieroEjecutarSegunLaSenial (t_entrenadorFisico * unEntrenador, void (*fcAbortiva) (t_entrenadorFisico *), void (*fcSIGUSR1) (t_entrenadorFisico *), void (*fcSIGTERM) (t_entrenadorFisico *) )
{
	//TODO: revisar si hay que hacer alguna otra cosa para 'encolar' señales o enrealidad bloquearlas (si no quiero que me interrumpan algo)


	//Primero cierro correctamente al entrenador
	if (_SIGINT_flag == 1)
	{
		log_warning(myArchivoDeLog,"**Voy a tratar la senial SIGINT.**" );
		fcAbortiva(unEntrenador);
		_SIGINT_flag = 0;	//reseteo el flag.
	}

	//Resto vidas
	if (_SIGTERM_flag == 1)
	{
		log_warning(myArchivoDeLog,"**Voy a tratar la senial SIGTERM.**" );
		fcSIGTERM(unEntrenador);
		_SIGTERM_flag = 0;	//reseteo el flag.
	}


	//sumar vidas al entrenador
	if (_SIGUSR1_flag == 1)
	{
		log_warning(myArchivoDeLog,"**Voy a tratar la senial SIGUSR1.**" );
		fcSIGUSR1(unEntrenador);
		_SIGUSR1_flag = 0;	//reseteo el flag.
	}

}

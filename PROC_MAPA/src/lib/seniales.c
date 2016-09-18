/*
 * libSeniales.c
 *
 *  Created on: 17/9/2016
 *      Author: utnso
 */
#include "seniales.h"

void revisarRecepcionDeSeniales (t_mapa * unMapa, void (*fc) (t_mapa *))
{
	//TODO: Que hilo deberia revisar las señales? Gui? Planificador? Etc..

	_SIGINT_flag = 0;	//reseteo el flag.
	_SIGTERM_flag = 0;	//reseteo el flag.
	_SIGUSR2_flag = 0;	//reseteo el flag.

	if (signal( SIGUSR2, tratarLaSenialRecibida ) == SIG_ERR)
	{
		log_error(myArchivoDeLog, "error en la senial SIGUSR2");
		fc(unMapa);
	}
	if (signal( SIGTERM, tratarLaSenialRecibida)==SIG_ERR)
	{
		log_error(myArchivoDeLog, "error en la senial SIGTERM");
		fc(unMapa);
	}
	if (signal(SIGINT, tratarLaSenialRecibida)==SIG_ERR)
	{
		log_error(myArchivoDeLog, "error en la senial SIGINT");
		fc(unMapa);
	}
}


void tratarLaSenialRecibida (int senial)
{
	switch (senial)
	{
		case SIGINT:
			log_info(myArchivoDeLog,"Recibi la senial SIGINT." );
			_SIGINT_flag = 1;
			break;

		case SIGUSR2:
			log_info(myArchivoDeLog,"Recibi la senial SIGUSR2." );
			_SIGUSR2_flag = 1;
			break;

		case SIGTERM:
			log_info(myArchivoDeLog,"Recibi la senial SIGTERM." );
			_SIGTERM_flag = 1;
			break;
	}

}

void funcionesQueQuieroEjecutarSegunLaSenial (t_mapa * unMapa, void (*fcAbortiva) (t_mapa *), void (*fcSIGUSR2) (t_mapa *) )
{
	//TODO: revisar si hay que hacer alguna otra cosa para 'encolar' señales o enrealidad bloquearlas (si no quiero que me interrumpan algo)


	//Primero cierro correctamente la gui
	if (_SIGINT_flag == 1)
	{
		fcAbortiva(unMapa);
		_SIGINT_flag = 0;	//reseteo el flag.
	}
	if (_SIGTERM_flag == 1)
	{
		fcAbortiva(unMapa);
		_SIGTERM_flag = 0;	//reseteo el flag.
	}


	//Segundo hago releer la metadata del mapa, segun enunciado con señal SIGUSR2
	if (_SIGUSR2_flag == 1)
	{
		fcSIGUSR2(unMapa);
		_SIGUSR2_flag = 0;	//reseteo el flag.
	}

}

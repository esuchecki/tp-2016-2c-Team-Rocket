/*
 * movimiento.c
 *
 *  Created on: 8/10/2016
 *      Author: utnso
 */


#include "movimiento.h"

void jugarEnElMapa(t_entrenadorFisico * unEntrenador, t_data * info, int socketConexion);
void inicializarEstadoEntrenador(t_entrenadorFisico * unEntrenador);
int queHago(t_estadoEntrenador* estado);
void enviarMensajeMovimiento(t_entrenadorFisico * unEntrenador, int socketConection);
void actualizarEstado(t_entrenadorFisico * unEntrenador, int socketConection);
void inicializarSocketEntrenador(t_entrenadorFisico * nuevoEntrenador, char * mapaIP, char * mapaPuerto);
bool conozcoLaPosicionDeLaPokeNest(t_entrenadorFisico * unEntrenador);
void pedirPorSocketLaPosicionDeLaPokeNestProxima (t_entrenadorFisico * unEntrenador, int socketConexion);
void jugarEnElMapa(t_entrenadorFisico * unEntrenador, t_data * info, int socketConexion);
void soyMaestroPokemon(t_entrenadorFisico * unEntrenador);
void enviarMensajeCapturarPkmn(t_entrenadorFisico * unEntrenador, int socketConection);
void logicaDeCapturarUnPkmn (t_entrenadorFisico * unEntrenador, t_data * info);
void logicaDeGuardarLaPosDeUnaPokenest (t_entrenadorFisico * unEntrenador, t_data * info);

void accionDelEntrenadorAnteSIGUSR1(t_entrenadorFisico * unEntrenador);
void accionDelEntrenadorAnteSIGTERM(t_entrenadorFisico * unEntrenador);



void inicializarEstadoEntrenador(t_entrenadorFisico * unEntrenador) {
	if (unEntrenador->moverseEnMapa == NULL)
	{
		//inicializamos al Estado del entrenador.
		t_estadoEntrenador * estadoEntrenador = malloc(sizeof(t_estadoEntrenador));	//pedir mmem
		estadoEntrenador->e_posX = __entrenadorPosInicialEnX;
		estadoEntrenador->e_posY = __entrenadorPosInicialEnY;
		estadoEntrenador->ultimoMov = 'Y'; //solo para que empieza hacia un costado...

		estadoEntrenador->indexObjetivoPkmn=0;
		estadoEntrenador->indexMapaActual=0;
		estadoEntrenador->p_posX = -1;
		estadoEntrenador->p_posY = -1;
		estadoEntrenador->respuesta = -1;

		unEntrenador->moverseEnMapa = estadoEntrenador;
	}
	else
	{
		t_estadoEntrenador * estadoEntrenador =unEntrenador->moverseEnMapa;
		estadoEntrenador->indexObjetivoPkmn ++;
		estadoEntrenador->p_posX = -1;
		estadoEntrenador->p_posY = -1;
		estadoEntrenador->respuesta = -1;
	}
}


void iniciarAventura(t_entrenadorFisico * unEntrenador)
{
	log_info(myArchivoDeLog, "Voy a iniciar mi aventura!");
	if (unEntrenador->metadata->hojaDeViaje->elements_count < 1)
	{
		log_error(myArchivoDeLog, "En la hoja de viaje no habia al menos 1 mapa al que conectarse");
		finalizarEntrenador(unEntrenador);
		return;
	}

	inicializarEstadoEntrenador(unEntrenador);
	//int indexMapaActual = 0;
	//Me conecto al primer mapa y sigo asi conectandome recursivamente..
	while (unEntrenador->metadata->hojaDeViaje->elements_count > unEntrenador->moverseEnMapa->indexMapaActual)
	{
		t_mapa * mapaActual = list_get(unEntrenador->metadata->hojaDeViaje, unEntrenador->moverseEnMapa->indexMapaActual);
		log_info(myArchivoDeLog, "Me voy a conectar al mapa: %s", mapaActual->nombreMapa);

		char * mapaActual_Ip = NULL;
		char * mapaActual_Puerto = NULL;
		cualEsLaIpDeEsteMapa(unEntrenador, mapaActual->nombreMapa, &mapaActual_Ip, &mapaActual_Puerto);

		if ((mapaActual_Ip != NULL) && (mapaActual_Puerto != NULL))
		{
			//Bueno, vamos a jugar en el primer mapa.
			inicializarSocketEntrenador(unEntrenador, mapaActual_Ip, mapaActual_Puerto);
		}
		else
		{
			log_error(myArchivoDeLog, "Me devolvieron una ip y puerto nulo para este mapa." );
			finalizarEntrenador(unEntrenador);
			return;
		}


		free(mapaActual_Ip);
		free(mapaActual_Puerto);
		//free(mapaActual);
		unEntrenador->moverseEnMapa->indexMapaActual ++;
		log_info(myArchivoDeLog, "Termine mis tareas en el mapa actual.");
	}

	//En teoria aca se convirtio en maestro pokemon..
	soyMaestroPokemon(unEntrenador);

}


void soyMaestroPokemon(t_entrenadorFisico * unEntrenador)
{
	//TODO: completar con lo demas...
	puts ("Soy maestro pokemon");
	mostrarTiempoTotalAventura(&unEntrenador->misEstadisticas);
}


int queHago(t_estadoEntrenador* estado) {

	//TODO: ver en cada movimiento que no se vaya fuera del mapa...
	// en teoria no va a pasar porque los pokemones van a estar en el mapa y se va a mover hasta alli.
	// pero quedaria joya chequearlo
	int respuesta = -1;
	int distanciaX = estado->p_posX - estado->e_posX;
	int distanciaY = estado->p_posY - estado->e_posY;
	if (estado->ultimoMov == 'X') {
		if (distanciaY < 0) {
			respuesta = 2;
		} else if (distanciaY > 0) {
			respuesta = 4;
		} else if (distanciaX < 0) {
			respuesta = 1;
		} else if (distanciaX > 0) {
			respuesta = 3;
		} else {
			respuesta = 0;
		}
	} else if (estado->ultimoMov == 'Y') {
		if (distanciaX < 0) {
			respuesta = 3;
		} else if (distanciaX > 0) {
			respuesta = 1;
		} else if (distanciaY < 0) {
			respuesta = 2;
		} else if (distanciaY > 0) {
			respuesta = 4;
		} else {
			respuesta = 0;
		}
	} else {
		//TODO:en este caso estoy en el primer movimiento... tomo este por defecto, ver cual tomar
		respuesta = 1;
	}

	return respuesta;
}


void enviarMensajeMovimiento(t_entrenadorFisico * unEntrenador, int socketConection){
	//puts("aca mandamos al mapa en base a lo que respondio!");
	t_data * paquete = pedirPaquete(movimientoEntrenador, sizeof(enum actividad), &unEntrenador->moverseEnMapa->respuesta);
	common_send(socketConection, paquete);
	free(paquete);
}

void enviarMensajeCapturarPkmn(t_entrenadorFisico * unEntrenador, int socketConection){
	t_mapa * mapaActual = list_get(unEntrenador->metadata->hojaDeViaje, unEntrenador->moverseEnMapa->indexMapaActual);
	char identificadorPokeNest = *(mapaActual->objetivosDelMapa[unEntrenador->moverseEnMapa->indexObjetivoPkmn]);


	t_data * paquete = pedirPaquete(capturarPokemon, sizeof(char), &identificadorPokeNest);
	common_send(socketConection, paquete);
	free(paquete);
}

void actualizarEstado(t_entrenadorFisico * unEntrenador, int socketConection) {

	switch (unEntrenador->moverseEnMapa->respuesta) {
	case destino:
		enviarMensajeCapturarPkmn(unEntrenador, socketConection);
		return;
		break;
	case moverDerecha:
		unEntrenador->moverseEnMapa->e_posX++;
		unEntrenador->moverseEnMapa->ultimoMov = 'X';
		puts("muevoDerecha");
		break;
	case moverArriba:
		unEntrenador->moverseEnMapa->e_posY--;
		unEntrenador->moverseEnMapa->ultimoMov = 'Y';
		puts("muevoArriba");
		break;
	case moverIzquierda:
		unEntrenador->moverseEnMapa->e_posX--;
		unEntrenador->moverseEnMapa->ultimoMov = 'X';
		puts("muevoIzquierda");
		break;
	case moverAbajo:
		unEntrenador->moverseEnMapa->e_posY++;
		unEntrenador->moverseEnMapa->ultimoMov = 'Y';
		puts("muevoAbajo");
		break;
	case noActividad:
		log_error(myArchivoDeLog, "Error, estoy en switch case noActividad.");
		finalizarEntrenador(unEntrenador);
		return;
		break;
	}
	enviarMensajeMovimiento(unEntrenador, socketConection);
}



void inicializarSocketEntrenador(t_entrenadorFisico * nuevoEntrenador, char * mapaIP, char * mapaPuerto) {
	log_info(myArchivoDeLog, "Inicializando la conexion por socket");
	log_info(myArchivoDeLog, "IP = %s, Puerto= %s.", mapaIP, mapaPuerto);

	int socketConexion = connect_to(mapaIP, mapaPuerto);
	if (socketConexion == -1)
	{
		log_error(myArchivoDeLog, "No se pudo inicializar la conexion por socket");
		finalizarEntrenador(nuevoEntrenador);
	}
	else
	{
		printf("socket: %d\n", socketConexion);

		log_info(myArchivoDeLog, "Me conecte por socket, mi socket es: %d",
				socketConexion);


		t_data * info = pedirPaquete(99, sizeof(char),
				&nuevoEntrenador->metadata->simbolo);
		common_send(socketConexion, info);
		log_info(myArchivoDeLog, "Envie paquete handshake");

		jugarEnElMapa (nuevoEntrenador, info, socketConexion);

	}
	close(socketConexion);
}

void accionDelEntrenadorAnteSIGUSR1(t_entrenadorFisico * unEntrenador) {
	//TODO: ¿Tengo que validar que la metadata este inicializada?
	unEntrenador->metadata->vidas++;	//sumo una vida
}

void accionDelEntrenadorAnteSIGTERM(t_entrenadorFisico * unEntrenador) {
	//TODO: ¿Tengo que validar que la metadata este inicializada?
	if (unEntrenador->metadata->vidas > 0)		//Minima cantidad de vidas es 0.
		unEntrenador->metadata->vidas--;	//resto una vida

	//TODO: Validar si me quede sin vidas me muero =(
}


bool conozcoLaPosicionDeLaPokeNest(t_entrenadorFisico * unEntrenador)
{
	if ( (unEntrenador->moverseEnMapa->p_posX > 0) && (unEntrenador->moverseEnMapa->p_posY > 0))
		return true;
	else
		return false;
}


void pedirPorSocketLaPosicionDeLaPokeNestProxima (t_entrenadorFisico * unEntrenador, int socketConexion)
{
	t_mapa * mapaActual = list_get(unEntrenador->metadata->hojaDeViaje, unEntrenador->moverseEnMapa->indexMapaActual);
	char identificadorPokeNest = *(mapaActual->objetivosDelMapa[unEntrenador->moverseEnMapa->indexObjetivoPkmn]);


	t_data * paquete = pedirPaquete(peticionPokenest, sizeof(char), &identificadorPokeNest);
	common_send(socketConexion, paquete);
	free(paquete);
}


void jugarEnElMapa(t_entrenadorFisico * unEntrenador, t_data * info, int socketConexion)
{
	t_mapa * mapaActual = list_get(unEntrenador->metadata->hojaDeViaje, unEntrenador->moverseEnMapa->indexMapaActual);
	while (1)
	{
		//TODO: abrir un hilo aparte para esto??
		funcionesQueQuieroEjecutarSegunLaSenial(unEntrenador,(void *) &finalizarEntrenador, (void*) &accionDelEntrenadorAnteSIGUSR1, (void*) &accionDelEntrenadorAnteSIGTERM);

		//Primero chequea que no le haya llegado ninguna señal...

		info = leer_paquete(socketConexion);
		switch (info->header)
		{
			case otorgarTurno:
				;

				/*
				 * Este 'otorgarTurno' es el corazon del Entrenador. Por defecto
				 * si el entrenador no conoce a donde se tiene que dirigir, pide
				 * la posicion de la proxima pokenest. Si la conoce, se fija que
				 * para donde moverse o avisa que quiere capturar un pokemon.
				 *
				 */
				puts("me otorgaron turno");
				if ( !conozcoLaPosicionDeLaPokeNest(unEntrenador) )
				{
					pedirPorSocketLaPosicionDeLaPokeNestProxima(unEntrenador, socketConexion);
				}
				else
				{
					//tengo la pokenest.. tengo que moverme
					unEntrenador->moverseEnMapa->respuesta=queHago(unEntrenador->moverseEnMapa);
					actualizarEstado(unEntrenador, socketConexion);
				}

				break;

			case ubicacionPokenest:
				logicaDeGuardarLaPosDeUnaPokenest(unEntrenador, info);

				break;
			case capturastePokemon:
				puts("Capture un pokemon");
				logicaDeCapturarUnPkmn(unEntrenador, info);

				break;
			case dameMejorPokemon:
				//TODO: enviar pokemon mas fuerte. realizar esta funcion

				//En caso de deadlock llamar a esta fc
				;
				t_pokemon * elMejor = malloc(sizeof(t_pokemon));
				elMejor = elegirMejorPokemon(unEntrenador,(void*) finalizarEntrenador);

				t_data * paquete = pedirPaquete(mejorPokemon,sizeof(t_pokemon),elMejor);

				common_send(socketConexion,paquete);

				break;
			case ganasteBatalla:
				//TODO: hacer lo que tenga que hacer
				break;
			case perdisteBatalla:
				//TODO: hacer lo que tenga que hacer
				break;
			case ubicacionMedallaMapa:
				//TODO: hacer lo que tenga que hacer
				break;
		}
		free(info->data);
		free(info);

		//Si llego al ultimo objetivo del mapa, sale!
		//TODO: ver que pasa con la medalla, de momento no informa que gano el mapa!!
		if ( (mapaActual->objetivosDelMapa[unEntrenador->moverseEnMapa->indexObjetivoPkmn]) == NULL)
		{
			//reseteo los objetivos del mapa, mantengo el flag de indexMapaActual
			int aux = unEntrenador->moverseEnMapa->indexMapaActual;
			free(unEntrenador->moverseEnMapa);
			unEntrenador->moverseEnMapa =NULL;
			inicializarEstadoEntrenador(unEntrenador);
			unEntrenador->moverseEnMapa->indexMapaActual = aux;

			return;
			break;
		}

	}
}

void logicaDeCapturarUnPkmn (t_entrenadorFisico * unEntrenador, t_data * info)
{
	log_info(myArchivoDeLog, "Me informaron que capture un pokemon. Lo voy a copiar a mi Dir de Bill");
	char * ubicacionPokemon = malloc ((sizeof (char)) * PATH_MAX +1);
	memcpy(ubicacionPokemon, info->data, (sizeof (char)) * PATH_MAX +1);


	char * directorioDeBill;
	directorioDeBill = malloc((sizeof(char)) * PATH_MAX + 1);

	sprintf(directorioDeBill, "/%s/%s/%s/%s/", unEntrenador->directorioPokeDex,__ubicacionEntrenadores, unEntrenador->nombre, __ubicacionDirDeBill);

	if ( copyFiles(ubicacionPokemon, directorioDeBill) )	//Lo copio. Si hubo algun error lo handleo
	{
		log_debug(myArchivoDeLog, "%s", ubicacionPokemon);
		log_debug(myArchivoDeLog, "%s", directorioDeBill);
		free(directorioDeBill);
		free(ubicacionPokemon);
		log_error(myArchivoDeLog,"Quise copiarme un pokemon y lo hice mal.");
		finalizarEntrenador(unEntrenador);
	}
	free(ubicacionPokemon);
	free(directorioDeBill);
	inicializarEstadoEntrenador(unEntrenador);	//Con esta linea avanza al proximo objetivo pokemon. Chequear que pasa si termino los objetivos del mapa.

}

void logicaDeGuardarLaPosDeUnaPokenest (t_entrenadorFisico * unEntrenador, t_data * info)
{
	//en info->data estara la posicion de la pokenest
	;
	int coordenadasEnX = 0;
	int coordenadasEnY = 0;
	memcpy(&coordenadasEnX, info->data, sizeof(int));
	memcpy(&coordenadasEnY, info->data + sizeof(int), sizeof(int));

	unEntrenador->moverseEnMapa->p_posX=coordenadasEnX;
	unEntrenador->moverseEnMapa->p_posY=coordenadasEnY;
	printf("Las coordenadas de la pokenest son: %d , %d\n",
			coordenadasEnX, coordenadasEnY);

}

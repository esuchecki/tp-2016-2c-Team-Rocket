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
	while (unEntrenador->metadata->hojaDeViaje->elements_count >= unEntrenador->moverseEnMapa->indexMapaActual)
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
		free(mapaActual);
		unEntrenador->moverseEnMapa->indexMapaActual ++;
	}

	//En teoria aca se convirtio en maestro pokemon..
	soyMaestroPokemon(unEntrenador);

}


void soyMaestroPokemon(t_entrenadorFisico * unEntrenador)
{
	//TODO: completar con lo demas...
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
	puts("aca mandamos al mapa en base a lo que respondio!");
	t_data * paquete = pedirPaquete(movimientoEntrenador, sizeof(enum actividad), &unEntrenador->moverseEnMapa->respuesta);
	common_send(socketConection, paquete);
	free(paquete);
}


void actualizarEstado(t_entrenadorFisico * unEntrenador, int socketConection) {

	switch (unEntrenador->moverseEnMapa->respuesta) {
	case destino:
		//TODO: falta implementar la func... enviar mensaje para capturar poke
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

/*
void enviarMsjFantasmaParaMoverse(int socketConection, t_data * info) {
	puts("entro");
	//char * mensaje = info->data;
	int * mensaje = malloc(sizeof(int));
	//strcpy(mensaje, "6");
	//printf("%s\n", mensaje);
	*mensaje = 6;	//solo movete 1 pos.
	t_data * paquete = pedirPaquete(movimientoEntrenador, sizeof(int), mensaje);
	common_send(socketConection, paquete);
	free(mensaje);
	free(paquete);
}
*/

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
	while (1)
	{
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
				//en info->data estara la posicion de la pokenest
				;
				int coordenadasEnX = 0;
				int coordenadasEnY = 0;
				memcpy(&coordenadasEnX, info->data, sizeof(int));
				memcpy(&coordenadasEnY, info->data + sizeof(int), sizeof(int));

				//MAXIS: inicializo la posicion del poknest
				unEntrenador->moverseEnMapa->p_posX=coordenadasEnX;
				unEntrenador->moverseEnMapa->p_posY=coordenadasEnY;
				printf("Las coordenadas de la pokenest son: %d , %d\n",
						coordenadasEnX, coordenadasEnY);

				//enviarMsjFantasmaParaMoverse(socketConexion, info);
				//sleep(5);

				break;
			case capturastePokemon:
				//TODO: hacer lo que tengo que hacer
				inicializarEstadoEntrenador(unEntrenador);	//Con esta linea avanza al proximo objetivo pokemon. Chequear que pasa si termino los objetivos del mapa.
				break;
			case dameMejorPokemon:
				//TODO: enviar pokemon mas fuerte. realizar esta funcion

				//En caso de deadlock llamar a esta fc
				//elegirMejorPokemon(miEntrenador,(void*) finalizarEntrenador);

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


	}
}

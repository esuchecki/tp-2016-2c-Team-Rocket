#include "movimiento.h"



enum tipoDeEstadoEntrenador {
	inicializar_estado = 150,
	siguienteObjetivoPkmn = 151,
	avanzarDeMapa = 152,
	setearmeEnReiniciarMapaActual = 153,
	setearmeEnReiniciarHojaDeViaje = 154,
};


void jugarEnElMapa(t_entrenadorFisico * unEntrenador, t_data * info,
		int socketConexion);
void setearEstadoDelEntrenador(t_entrenadorFisico * unEntrenador, enum tipoDeEstadoEntrenador unFlag);
int queHago(t_estadoEntrenador* estado);
void enviarMensajeMovimiento(t_entrenadorFisico * unEntrenador,
		int socketConection);
void actualizarEstado(t_entrenadorFisico * unEntrenador, int socketConection,
		time_t tiempo);
void inicializarSocketEntrenador(t_entrenadorFisico * nuevoEntrenador,
		char * mapaIP, char * mapaPuerto);
bool conozcoLaPosicionDeLaPokeNest(t_entrenadorFisico * unEntrenador);
void pedirPorSocketLaPosicionDeLaPokeNestProxima(
		t_entrenadorFisico * unEntrenador, int socketConexion);
void jugarEnElMapa(t_entrenadorFisico * unEntrenador, t_data * info,
		int socketConexion);
void soyMaestroPokemon(t_entrenadorFisico * unEntrenador);
void enviarMensajeCapturarPkmn(t_entrenadorFisico * unEntrenador,
		int socketConection);
void logicaDeCapturarUnPkmn(t_entrenadorFisico * unEntrenador, t_data * info);
void logicaDeGuardarLaPosDeUnaPokenest(t_entrenadorFisico * unEntrenador,
		t_data * info);
void recibirRespuesta(int socketConexion, t_entrenadorFisico * unEntrenador,
		time_t tiempo);
void copiarseMedallasDelMapaActual(t_entrenadorFisico * unEntrenador, char * nombreMapa);
void fcAuxiliarInicializarEstado (t_entrenadorFisico * unEntrenador);
bool estoyEnEstadoDeReiniciarMapaActual (t_entrenadorFisico * unEntrenador);
bool estoyEnEstadoDeReiniciarHojaDeViaje (t_entrenadorFisico * unEntrenador);


void accionDelEntrenadorAnteSIGUSR1(t_entrenadorFisico * unEntrenador);
void accionDelEntrenadorAnteSIGTERM(t_entrenadorFisico * unEntrenador, bool fueVicitimaDeDeadlock);




//----------------------------------------------------//




void setearEstadoDelEntrenador(t_entrenadorFisico * unEntrenador, enum tipoDeEstadoEntrenador unFlag) {
	switch (unFlag)
	{
	case inicializar_estado:

		if ( estoyEnEstadoDeReiniciarHojaDeViaje(unEntrenador) ) {
			//inicializamos al Estado del entrenador.
			t_estadoEntrenador * estadoEntrenador = malloc(sizeof(t_estadoEntrenador));

			estadoEntrenador->indexMapaActual = 0;
			unEntrenador->moverseEnMapa = estadoEntrenador;
			fcAuxiliarInicializarEstado(unEntrenador);
			break;
		}

		//si fui victima de deadlock
		else if ( estoyEnEstadoDeReiniciarMapaActual(unEntrenador) )
		{
			unEntrenador->moverseEnMapa->indexMapaActual++;
			unEntrenador->moverseEnMapa->indexMapaActual = -unEntrenador->moverseEnMapa->indexMapaActual;
			fcAuxiliarInicializarEstado(unEntrenador);
			break;
		}

		//TODO: error?
		break;
	case siguienteObjetivoPkmn:

		//avanzo al proximo objetivo pkmn.
		unEntrenador->moverseEnMapa->indexObjetivoPkmn++;
		unEntrenador->moverseEnMapa->p_posX = -1;
		unEntrenador->moverseEnMapa->p_posY = -1;
		unEntrenador->moverseEnMapa->respuesta = -1;

		break;
	case avanzarDeMapa:
		;
		//reseteo los objetivos del mapa, mantengo el flag de indexMapaActual
		int aux = unEntrenador->moverseEnMapa->indexMapaActual;
		free(unEntrenador->moverseEnMapa);
		unEntrenador->moverseEnMapa = NULL;
		setearEstadoDelEntrenador(unEntrenador, inicializar_estado);
		unEntrenador->moverseEnMapa->indexMapaActual = aux;
		break;
	case setearmeEnReiniciarMapaActual:

		unEntrenador->moverseEnMapa->indexMapaActual =- unEntrenador->moverseEnMapa->indexMapaActual;
		unEntrenador->moverseEnMapa->indexMapaActual--;
		break;
	case setearmeEnReiniciarHojaDeViaje:

		free(unEntrenador->moverseEnMapa);
		unEntrenador->moverseEnMapa = NULL;
		break;
	default:
		log_error(myArchivoDeLog, "llegue a default en setarEstadoEntrenador.");
		finalizarEntrenador(unEntrenador);
		break;
	}

}


void fcAuxiliarInicializarEstado (t_entrenadorFisico * unEntrenador)
{
	unEntrenador->moverseEnMapa->e_posX = __entrenadorPosInicialEnX;
	unEntrenador->moverseEnMapa->e_posY = __entrenadorPosInicialEnY;
	unEntrenador->moverseEnMapa->ultimoMov = 'Y'; //solo para que empieza hacia un costado...

	unEntrenador->moverseEnMapa->indexObjetivoPkmn = 0;
	unEntrenador->moverseEnMapa->p_posX = -1;
	unEntrenador->moverseEnMapa->p_posY = -1;
	unEntrenador->moverseEnMapa->respuesta = -1;
}

bool estoyEnEstadoDeReiniciarMapaActual (t_entrenadorFisico * unEntrenador)
{
	if (unEntrenador->moverseEnMapa != NULL)
		return (unEntrenador->moverseEnMapa->indexMapaActual < 0);
	return false;
}

bool estoyEnEstadoDeReiniciarHojaDeViaje (t_entrenadorFisico * unEntrenador)
{
	return (unEntrenador->moverseEnMapa == NULL);
}

void iniciarAventura(t_entrenadorFisico * unEntrenador) {
	log_info(myArchivoDeLog, "Voy a iniciar mi aventura!");
	if (unEntrenador->metadata->hojaDeViaje->elements_count < 1) {
		log_error(myArchivoDeLog,	"En la hoja de viaje no habia al menos 1 mapa al que conectarse");
		finalizarEntrenador(unEntrenador);
		return;
	}
	//TODO: esta referencia anull no tendria que estar.
	unEntrenador->moverseEnMapa = NULL;

	setearEstadoDelEntrenador(unEntrenador, inicializar_estado);
	//int indexMapaActual = 0;
	//Me conecto al primer mapa y sigo asi conectandome recursivamente..
	while ( list_size(unEntrenador->metadata->hojaDeViaje)	> unEntrenador->moverseEnMapa->indexMapaActual) {
		t_mapa * mapaActual = list_get(unEntrenador->metadata->hojaDeViaje,	unEntrenador->moverseEnMapa->indexMapaActual);
		log_info(myArchivoDeLog, "Me voy a conectar al mapa: %s",	mapaActual->nombreMapa);
		printf("Me voy a conectar al mapa: %s\n",	mapaActual->nombreMapa);

		char * mapaActual_Ip = NULL;
		char * mapaActual_Puerto = NULL;
		cualEsLaIpDeEsteMapa(unEntrenador, mapaActual->nombreMapa, &mapaActual_Ip, &mapaActual_Puerto);
		if ((mapaActual_Ip != NULL) && (mapaActual_Puerto != NULL)) {
			//Bueno, vamos a jugar en el primer mapa.
			inicializarSocketEntrenador(unEntrenador, mapaActual_Ip, mapaActual_Puerto);
		} else {
			log_error(myArchivoDeLog, "Me devolvieron una ip y puerto nulo para este mapa.");
			finalizarEntrenador(unEntrenador);
			return;
		}

		free(mapaActual_Ip);
		free(mapaActual_Puerto);
		if (estoyEnEstadoDeReiniciarMapaActual(unEntrenador) || estoyEnEstadoDeReiniciarHojaDeViaje(unEntrenador) )
		{
			setearEstadoDelEntrenador(unEntrenador, inicializar_estado);
		}
		else
		{
			//free(mapaActual);
			copiarseMedallasDelMapaActual(unEntrenador, mapaActual->nombreMapa);	//Antes me copio la medalla
			unEntrenador->moverseEnMapa->indexMapaActual++;
			log_info(myArchivoDeLog, "Termine mis tareas en el mapa actual.");
		}
	}
	//En teoria aca se convirtio en maestro pokemon..
	soyMaestroPokemon(unEntrenador);

}

void soyMaestroPokemon(t_entrenadorFisico * unEntrenador) {
	//TODO: completar con lo demas...
	puts("Soy maestro pokemon");
	mostrarEstadisticas(&unEntrenador->misEstadisticas);
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

void enviarMensajeMovimiento(t_entrenadorFisico * unEntrenador,
		int socketConection) {
	//puts("aca mandamos al mapa en base a lo que respondio!");
	t_data * paquete = pedirPaquete(movimientoEntrenador,
			sizeof(enum actividad), &unEntrenador->moverseEnMapa->respuesta);
	common_send(socketConection, paquete);
	free(paquete);
}

void enviarMensajeCapturarPkmn(t_entrenadorFisico * unEntrenador,
		int socketConection) {
	puts ("solicito capturar el pkmn");
	t_mapa * mapaActual = list_get(unEntrenador->metadata->hojaDeViaje,
			unEntrenador->moverseEnMapa->indexMapaActual);
	char identificadorPokeNest =
			*(mapaActual->objetivosDelMapa[unEntrenador->moverseEnMapa->indexObjetivoPkmn]);

	t_data * paquete = pedirPaquete(capturarPokemon, sizeof(char),
			&identificadorPokeNest);
	common_send(socketConection, paquete);
	free(paquete);
}

void actualizarEstado(t_entrenadorFisico * unEntrenador, int socketConection,
		time_t tiempo) {
	switch (unEntrenador->moverseEnMapa->respuesta) {
	case destino:
		enviarMensajeCapturarPkmn(unEntrenador, socketConection);
		recibirRespuesta(socketConection, unEntrenador, tiempo);
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
	recibirRespuesta(socketConection, unEntrenador, tiempo);
}

void inicializarSocketEntrenador(t_entrenadorFisico * nuevoEntrenador,
		char * mapaIP, char * mapaPuerto) {
	log_info(myArchivoDeLog, "Inicializando la conexion por socket");
	log_info(myArchivoDeLog, "IP = %s, Puerto= %s.", mapaIP, mapaPuerto);

	int socketConexion = connect_to(mapaIP, mapaPuerto);
	if (socketConexion == -1) {
		log_error(myArchivoDeLog,
				"No se pudo inicializar la conexion por socket");
		finalizarEntrenador(nuevoEntrenador);
	} else {
		printf("socket: %d\n", socketConexion);

		log_info(myArchivoDeLog, "Me conecte por socket, mi socket es: %d",
				socketConexion);

		t_data * info = pedirPaquete(99, sizeof(char),
				&nuevoEntrenador->metadata->simbolo);
		common_send(socketConexion, info);
		info = leer_paquete(socketConexion);
		if(info->header == 50){
			log_info(myArchivoDeLog,"Intercambio de mensajes Handshake realizado");
			log_info(myArchivoDeLog,"Conexion Exitosa");
			jugarEnElMapa(nuevoEntrenador, info, socketConexion);
		}else{
			log_info(myArchivoDeLog,"No se pudo conectar, cierre forzoso");
			finalizarEntrenador(nuevoEntrenador);
			//exit(EXIT_FAILURE);
		}

	}
	close(socketConexion);
	puts("Me desconecto del mapa actual.");
}

void accionDelEntrenadorAnteSIGUSR1(t_entrenadorFisico * unEntrenador) {
	//TODO: ¿Tengo que validar que la metadata este inicializada?
	unEntrenador->metadata->vidas = unEntrenador->metadata->vidas + _SIGUSR1_flag;	//sumo vidas
}

void actualizarTiempoBloqueado(t_entrenadorFisico * unEntrenador,
		time_t tInicio) {
	//double tiempo = difftime(time(NULL), tInicio);
	unEntrenador->misEstadisticas.bloqEnPokeNest_time =
			unEntrenador->misEstadisticas.bloqEnPokeNest_time
					+ difftime(time(NULL), tInicio);
	;
	//printf("bloqueo poke: %i \n",
	//		unEntrenador->misEstadisticas.bloqEnPokeNest_time);

}
void reintentar(t_entrenadorFisico * unEntrenador) {
	char reintentar;
	puts ("No le quedan mas vidas disponibles..");
	printf("Desea reintentar?S/N \n");
	scanf("%s", &reintentar);
	if (reintentar == 'S' || reintentar == 's')
	{
		unEntrenador->misEstadisticas.cant_reintentos ++;
		unEntrenador->metadata->vidas = 1;	//le dejo una vida! :D
		borrarDirectorioDeBill(unEntrenador);
		//TODO: borrar medallas.
		setearEstadoDelEntrenador(unEntrenador, setearmeEnReiniciarHojaDeViaje);
	}
	else	//Si no quiere reintentar lo finalizo.
		finalizarEntrenador(unEntrenador);
}

void accionDelEntrenadorAnteSIGTERM(t_entrenadorFisico * unEntrenador, bool fuiVictimaDeDeadlock) {
	//TODO: ¿Tengo que validar que la metadata este inicializada?


	if (fuiVictimaDeDeadlock)
	{
		puts ("Me descontaron una vida por deadlock");
		if (unEntrenador->metadata->vidas > 0)//Minima cantidad de vidas es 0.
			unEntrenador->metadata->vidas --;
	}
	else
	{
		if (unEntrenador->metadata->vidas > _SIGTERM_flag)		//Minima cantidad de vidas es 0.
			unEntrenador->metadata->vidas = unEntrenador->metadata->vidas -_SIGTERM_flag;	//resto vidas
		else
			unEntrenador->metadata->vidas =0;

		puts ("Le quitaron vidas con SIGTERM.");
	}

	printf ("Me quedan %d vidas.\n", unEntrenador->metadata->vidas);

	//Si no tengo vidas, me muero!.
	if (unEntrenador->metadata->vidas == 0) {
		unEntrenador->misEstadisticas.cant_muertes ++;
		reintentar(unEntrenador);
	} else if ( (unEntrenador->metadata->vidas > 0) && fuiVictimaDeDeadlock) {
		unEntrenador->misEstadisticas.cant_muertes++;
		setearEstadoDelEntrenador(unEntrenador, setearmeEnReiniciarMapaActual);
		borrarDirectorioDeBill(unEntrenador);
	}
}

bool conozcoLaPosicionDeLaPokeNest(t_entrenadorFisico * unEntrenador) {
	return ((unEntrenador->moverseEnMapa->p_posX > 0) && (unEntrenador->moverseEnMapa->p_posY > 0));
}

void pedirPorSocketLaPosicionDeLaPokeNestProxima(
		t_entrenadorFisico * unEntrenador, int socketConexion) {
	t_mapa * mapaActual = list_get(unEntrenador->metadata->hojaDeViaje,
			unEntrenador->moverseEnMapa->indexMapaActual);
	char identificadorPokeNest =
			*(mapaActual->objetivosDelMapa[unEntrenador->moverseEnMapa->indexObjetivoPkmn]);

	t_data * paquete = pedirPaquete(peticionPokenest, sizeof(char),
			&identificadorPokeNest);
	common_send(socketConexion, paquete);
	free(paquete);
}


void jugarEnElMapa(t_entrenadorFisico * unEntrenador, t_data * info,
		int socketConexion) {
	t_mapa * mapaActual = list_get(unEntrenador->metadata->hojaDeViaje,	unEntrenador->moverseEnMapa->indexMapaActual);
	time_t tiempoAuxPN;

	while (1) {
		//TODO: abrir un hilo aparte para esto??
		//Primero chequea que no le haya llegado ninguna señal...
		funcionesQueQuieroEjecutarSegunLaSenial(unEntrenador, (void*) &accionDelEntrenadorAnteSIGUSR1, (void*) &accionDelEntrenadorAnteSIGTERM);

		if ( estoyEnEstadoDeReiniciarHojaDeViaje(unEntrenador) )
		{	return;		break;		}


		if (!conozcoLaPosicionDeLaPokeNest(unEntrenador)) {
			pedirPorSocketLaPosicionDeLaPokeNestProxima(unEntrenador,
					socketConexion);
			recibirRespuesta(socketConexion, unEntrenador, tiempoAuxPN);
		} else {
			//tengo la pokenest.. tengo que moverme
			unEntrenador->moverseEnMapa->respuesta = queHago(unEntrenador->moverseEnMapa);
			if (unEntrenador->moverseEnMapa->respuesta == 0) {
				tiempoAuxPN = time(NULL); //Lo inicializo aca, por si justo arranca en una posicion pokeNest
			}
			actualizarEstado(unEntrenador, socketConexion, tiempoAuxPN);
		}


		if ( estoyEnEstadoDeReiniciarMapaActual(unEntrenador) )
		{	actualizarTiempoBloqueado(unEntrenador, tiempoAuxPN);
			return;		break;
		}

		//Si llego al ultimo objetivo del mapa, sale!
		if ((mapaActual->objetivosDelMapa[unEntrenador->moverseEnMapa->indexObjetivoPkmn])	== NULL) {
			setearEstadoDelEntrenador(unEntrenador, avanzarDeMapa);
			return;		break;
		}

	}
}

void logicaDeCapturarUnPkmn(t_entrenadorFisico * unEntrenador, t_data * info) {
	log_info(myArchivoDeLog,
			"Me informaron que capture un pokemon. Lo voy a copiar a mi Dir de Bill");
	char * ubicacionPokemon = malloc((sizeof(char)) * PATH_MAX + 1);
	memcpy(ubicacionPokemon, info->data, (sizeof(char)) * PATH_MAX + 1);

	char * directorioDeBill;
	directorioDeBill = malloc((sizeof(char)) * PATH_MAX + 1);

	sprintf(directorioDeBill, "/%s/%s/%s/%s/", unEntrenador->directorioPokeDex,
	__ubicacionEntrenadores, unEntrenador->nombre,
	__ubicacionDirDeBill);

	if (copyFiles(ubicacionPokemon, directorioDeBill))//Lo copio. Si hubo algun error lo handleo
	{
		log_debug(myArchivoDeLog, "%s", ubicacionPokemon);
		log_debug(myArchivoDeLog, "%s", directorioDeBill);
		free(directorioDeBill);
		free(ubicacionPokemon);
		log_error(myArchivoDeLog, "Quise copiarme un pokemon y lo hice mal.");
		finalizarEntrenador(unEntrenador);
	}
	free(ubicacionPokemon);
	free(directorioDeBill);
	setearEstadoDelEntrenador(unEntrenador, siguienteObjetivoPkmn);

}

void logicaDeGuardarLaPosDeUnaPokenest(t_entrenadorFisico * unEntrenador,
		t_data * info) {
	//en info->data estara la posicion de la pokenest
	;
	int coordenadasEnX = 0;
	int coordenadasEnY = 0;
	memcpy(&coordenadasEnX, info->data, sizeof(int));
	memcpy(&coordenadasEnY, info->data + sizeof(int), sizeof(int));

	unEntrenador->moverseEnMapa->p_posX = coordenadasEnX;
	unEntrenador->moverseEnMapa->p_posY = coordenadasEnY;
	printf("Las coordenadas de la pokenest son: %d , %d\n", coordenadasEnX,
			coordenadasEnY);

}

void recibirRespuesta(int socketConexion, t_entrenadorFisico * unEntrenador,
		time_t tiempoAuxPN) {
	t_data *info = leer_paquete(socketConexion);
	switch (info->header) {
	case otorgarTurno:
		;
		puts("Me Movi Bien");
		break;

	case ubicacionPokenest:
		logicaDeGuardarLaPosDeUnaPokenest(unEntrenador, info);
		puts("Guarde la pokenest");
		break;
	case capturastePokemon:
		puts("Capture un pokemon");
		logicaDeCapturarUnPkmn(unEntrenador, info);
		actualizarTiempoBloqueado(unEntrenador, tiempoAuxPN);
		break;
	case dameMejorPokemon:

		puts("Me pidieron mi mejor pokemon!.");
		unEntrenador->misEstadisticas.cant_deadlocks ++;

		//En caso de deadlock llamar a esta fc
		;
		t_pokemon * elMejor = malloc(sizeof(t_pokemon));
		elMejor = elegirMejorPokemon(unEntrenador);

		char * auxSpecies = malloc(50*sizeof(char));
		strcpy(auxSpecies, elMejor->species);

		void * buffer = malloc(sizeof(t_pokemon)+50*sizeof(char));
		memcpy(buffer, elMejor, sizeof(t_pokemon));
		memcpy(buffer + sizeof(t_pokemon), auxSpecies, 50*sizeof(char));

		t_data * paquete = pedirPaquete(mejorPokemon, sizeof(t_pokemon)+50*sizeof(char), buffer);

		common_send(socketConexion, paquete);

		recibirRespuesta(socketConexion, unEntrenador, tiempoAuxPN);	//vuelve a chequear a ver que le responden!

		break;
	case ganasteBatalla:
		puts("Gane batalla por Deadlock");
		break; 	//este mensaje quedo desestimado.
	case perdisteBatalla:
		//TODO: hacer lo que tenga que hacer
			//loguear, etc...
		accionDelEntrenadorAnteSIGTERM(unEntrenador, true);	//con esto ejecuta la logica!.
		break;
	case ubicacionMedallaMapa: break; //este mensaje quedo desestimado.
	}
	free(info->data);
	free(info);

}



void copiarseMedallasDelMapaActual(t_entrenadorFisico * unEntrenador, char * nombreMapa) {
	log_info(myArchivoDeLog,
			"Me informaron que finalice el mapa. Me voy a copiar la medalla");
	char * ubicacionMedallaEnMapa = malloc((sizeof(char)) * PATH_MAX + 1);
	char * directorioDeMedallasDelEntrenador = malloc((sizeof(char)) * PATH_MAX + 1);

	char * formato = string_from_format("%s%s", "/%s/%s/%s/", __ubicacionMedallaDelMapa);
	sprintf(ubicacionMedallaEnMapa, formato , unEntrenador->directorioPokeDex,	__ubicacionMapas, nombreMapa, nombreMapa);
	sprintf(directorioDeMedallasDelEntrenador, "/%s/%s/%s/%s/", unEntrenador->directorioPokeDex,	__ubicacionEntrenadores, unEntrenador->nombre,	__ubicacionDirDeMedallas);

	if (copyFiles(ubicacionMedallaEnMapa, directorioDeMedallasDelEntrenador))//Lo copio. Si hubo algun error lo handleo
			{
		log_debug(myArchivoDeLog, "%s", ubicacionMedallaEnMapa);
		log_debug(myArchivoDeLog, "%s", directorioDeMedallasDelEntrenador);
		free(directorioDeMedallasDelEntrenador);
		free(ubicacionMedallaEnMapa);
		free(formato);
		log_error(myArchivoDeLog, "Quise copiarme la medalla y lo hice mal.");
		finalizarEntrenador(unEntrenador);
	}
	free(ubicacionMedallaEnMapa);
	free(directorioDeMedallasDelEntrenador);
	free(formato);
	}
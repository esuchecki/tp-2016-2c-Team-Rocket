/*
 * tiempos.c
 *
 *  Created on: 12/11/2016
 *      Author: utnso
 */

#include <errno.h>
#include "tiempos.h"
#include <time.h>


void convertirMilisegundosToTimeSpec(void *ts, unsigned long ms, bool tiempoAbsoluto)
{
	struct timespec *ts2 = ts;
	ts2->tv_sec =0;
	ts2->tv_nsec=0;

	if (tiempoAbsoluto)
		clock_gettime(CLOCK_REALTIME, ts2);


    ts2->tv_sec =+ (ms / 1000);
    ts2->tv_nsec =+ ((ms % 1000) * 1000000);
}

void convertirSegundosToTimeSpec(void *ts, uint32_t ms)
{
	struct timespec *ts2 = ts;
	ts2->tv_sec =0;
	ts2->tv_nsec=0;

    ts2->tv_sec =+ ms;
    ts2->tv_nsec =+0;
}

void sleepInMiliSegundos(unsigned long ms)
{
	struct timespec retardo;
	convertirMilisegundosToTimeSpec(&retardo, ms, false);

	do
	{
		if ( clock_nanosleep(CLOCK_MONOTONIC,0, &retardo, &retardo ) == 0 || errno != EINTR )
			break;
	} while ( 1 );
	return;
}

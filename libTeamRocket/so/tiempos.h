/*
 * tiempos.h
 *
 *  Created on: 12/11/2016
 *      Author: utnso
 */

#ifndef SO_TIEMPOS_H_
#define SO_TIEMPOS_H_

#include <stdbool.h>
#include <stdint.h>	//para el uint32_t



/*
 * @NAME: sleepInMiliSegundos
 * @DESC: hace el sleep en milisegundos.
 */
void sleepInMiliSegundos(unsigned long ms);


/*
 * @NAME: convertirMilisegundosToTimeSpec
 * @DESC: fc utilizada para el nanosleep y el semaforo con espera.
 */
void convertirMilisegundosToTimeSpec(void *timespec, unsigned long ms, bool tiempoAbsoluto);

/*
 * @NAME: convertirSegundosToTimeSpec
 * @DESC: fc utilizada por el OSADA_FS
 */
void convertirSegundosToTimeSpec(void *ts, uint32_t ms);


void sleepInMiliSegundosRevisarSenial(unsigned long ms, short int * _SIGUSR2_flag);


#endif /* SO_TIEMPOS_H_ */

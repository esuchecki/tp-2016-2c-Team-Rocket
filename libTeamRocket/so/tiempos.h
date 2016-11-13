/*
 * tiempos.h
 *
 *  Created on: 12/11/2016
 *      Author: utnso
 */

#ifndef SO_TIEMPOS_H_
#define SO_TIEMPOS_H_

#include <stdbool.h>



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

#endif /* SO_TIEMPOS_H_ */

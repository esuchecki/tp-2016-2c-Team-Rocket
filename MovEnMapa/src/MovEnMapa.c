/*
 ============================================================================
 Name        : MovEnMapa.c
 Author      : maxis
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

/**
 *
 * ultimoMov->'x' o 'y'
 * respuesta:
 * 			- 0 -> esta en destino
 * 			- 1 -> movete derecha
 * 			- 2 -> movete arriba
 * 			- 3 -> movete izquierda
 * 			- 4 -> movete abajo
 * 			- -1-> hay algun error... ver mas adelante
 */

int queHago(int e_posX, int e_posY, int p_posX,int p_posY, char ultimoMov){
	int respuesta = -1;
		int distanciaX= p_posX-e_posX;
		int distanciaY= p_posY-e_posY;
		if(ultimoMov=='X'){
			if(distanciaY<0){
				respuesta = 2;
			}else if(distanciaY>0){
				respuesta = 4;
			}else if (distanciaX<0){
				respuesta = 1;
			}else if (distanciaX>0){
					respuesta = 3;
				}else{
					respuesta = 0;
				}
			}else if (ultimoMov=='Y'){
				if(distanciaX<0){
					respuesta = 1;
				}else if(distanciaX>0){
					respuesta = 3;
				}else if (distanciaY<0){
					respuesta = 2;
				}else if (distanciaY>0){
						respuesta = 4;
					}else{
						respuesta = 0;
					}
				}else{
					//en este caso estoy en el primer movimiento... tomo este por defecto, ver cual tomar
					respuesta = 1;
				}

		return respuesta;
}

int main(void) {
	int respuesta =queHago(35,3,3,3,'X');
//	puts(respuesta);
	printf("devuelve %i", respuesta);
	return EXIT_SUCCESS;
}



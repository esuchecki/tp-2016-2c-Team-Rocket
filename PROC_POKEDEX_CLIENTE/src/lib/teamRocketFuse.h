/*
 * fuse.h
 *
 *  Created on: 28/9/2016
 *      Author: utnso
 */

#ifndef LIB_TEAMROCKETFUSE_H_
#define LIB_TEAMROCKETFUSE_H_

#include <stdio.h>
#include <stdlib.h>
#include <fuse.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


int* pmap_pikachu;
int* pmap_squirtle;
int* pmap_bulbasaur;
struct stat pikachuStat;
struct stat squirtleStat;
struct stat bulbasaurStat;

int socketConexion;
int iniciarFuse(int argc,char*argv[]);


#endif /* LIB_TEAMROCKETFUSE_H_ */

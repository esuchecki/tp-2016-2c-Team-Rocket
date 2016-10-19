/*
 * fuse.c
 *
 *  Created on: 28/9/2016
 *      Author: utnso
 */

#include "teamRocketFuse.h"
#include <so/libSockets.h>

static int ejemplo_getattr(const char *path, struct stat *stbuf) {

	t_data * paquete = pedirPaquete(solicitudGetAttr,strlen(path),&path);
	common_send(socketConexion,paquete);

	paquete = leer_paquete(socketConexion);
	switch(paquete->header){
	case respuestaPorArchivo:
		break;
	case respuestaPorDirectorio:
		break;
	}

	int res = 0;
	memset(stbuf, 0, sizeof(struct stat));

	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strcmp(path, "/pikachu") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strcmp(path, "/squirtle") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strcmp(path, "/bulbasaur") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strcmp(path, "/pokemon.txt") == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = 144;
	} else if (strcmp(path, "/pikachu/pikachu.png") == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = pikachuStat.st_size;
	} else if (strcmp(path, "/squirtle/vamo a calmarno.jpg") == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = squirtleStat.st_size;
	} else if (strcmp(path, "/bulbasaur/Bulbasaur.png") == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = bulbasaurStat.st_size;
	} else if (strcmp(path, "/pepito.txt") == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = 5;
	} else {
		res = -ENOENT;
	}

	return res;
}

static int ejemplo_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		off_t offset, struct fuse_file_info *fi) {

	//TODO: ver si falta el \0
	t_data * paquete = pedirPaquete(solicitudReadAddr, strlen(path), &path);

	common_send(socketConexion, paquete);

	paquete = leer_paquete(socketConexion);

	if (paquete->header == respuestaReadAddr) {
		//TODO: parsear string respuesta y fillear
		while (1) {
			if (strlen(paquete->data) == 0) {
				break;
			}
			char * palabra = leerHastaCentinela(paquete->data);
			paquete->data = memchr(paquete->data, '%', strlen(paquete->data));
			paquete->data = (char*) paquete->data + 1;

			printf("palabra detectada: %s\n", palabra);

			//TODO: ahora hay que fillearlo? ver bien el tema de las paternidades
		}

	} else if (paquete->header == errorReadAddr) {
		//TODO: servidor no encontro nada segun el path enviado
	}

	int res = 0;

	if (strcmp(path, "/") == 0) {
		filler(buf, "pikachu", NULL, 0);
		filler(buf, "squirtle", NULL, 0);
		filler(buf, "bulbasaur", NULL, 0);
		filler(buf, "pokemon.txt", NULL, 0);
		filler(buf, "pepito.txt", NULL, 0);
	} else if (strcmp(path, "/pikachu") == 0) {
		filler(buf, "pikachu.png", NULL, 0);
	} else if (strcmp(path, "/squirtle") == 0) {
		filler(buf, "vamo a calmarno.jpg", NULL, 0);
	} else if (strcmp(path, "/bulbasaur") == 0) {
		filler(buf, "Bulbasaur.png", NULL, 0);
	} else {
		res = -ENOENT;
	}

	return res;

}

static int ejemplo_read(const char *path, char *buf, size_t size, off_t offset,
		struct fuse_file_info *fi) {

	enviarLecturaArchivo(path,size,offset);
	t_data * lectura = leer_paquete(socketConexion);
	if(lectura->header == respuestaLectura){
		memcpy(buf,lectura->data,size);
	}



	if (strcmp(path, "/pikachu/pikachu.png") == 0) {
		memcpy(buf, ((char*) pmap_pikachu + offset), size);
	} else if (strcmp(path, "/squirtle/vamo a calmarno.jpg") == 0) {
		memcpy(buf, ((char*) pmap_squirtle + offset), size);
	} else if (strcmp(path, "/bulbasaur/Bulbasaur.png") == 0) {
		memcpy(buf, ((char*) pmap_bulbasaur + offset), size);
	} else if (strcmp(path, "/pokemon.txt") == 0) {
		memcpy(buf,
				"Los pokemon son una clase de criaturas (monstruos) basadas en muchos casos en animales reales o criaturas míticas y mitológicas orientales.\n",
				size);
	} else if (strcmp(path, "/pepito.txt") == 0) {
		memcpy(buf, "Hola\n", size);
	}
	return size;
}

static struct fuse_operations ejemplo_oper = { .getattr = ejemplo_getattr,
		.readdir = ejemplo_readdir, .read = ejemplo_read, };

int iniciarFuse(int argc, char*argv[]) {
	//pikachu
	int fd_pikachu;
	fd_pikachu = open("../pokedex_dir/pikachu.png", O_RDWR);
	fstat(fd_pikachu, &pikachuStat);
	pmap_pikachu = mmap(0, pikachuStat.st_size, PROT_READ | PROT_WRITE,
			MAP_SHARED, fd_pikachu, 0);

	//squirtle
	int fd_squirtle;
	fd_squirtle = open("../pokedex_dir/vamo a calmarno.jpg", O_RDWR);
	fstat(fd_squirtle, &squirtleStat);
	pmap_squirtle = mmap(0, squirtleStat.st_size, PROT_READ | PROT_WRITE,
			MAP_SHARED, fd_squirtle, 0);

	//bulbasaur
	int fd_bulbasaur;
	fd_bulbasaur = open("../pokedex_dir/Bulbasaur.png", O_RDWR);
	fstat(fd_bulbasaur, &bulbasaurStat);
	pmap_bulbasaur = mmap(0, bulbasaurStat.st_size, PROT_READ | PROT_WRITE,
			MAP_SHARED, fd_bulbasaur, 0);

	return fuse_main(argc, argv, &ejemplo_oper, NULL);
}

char * leerHastaCentinela(char *paquete) {
	int i;
	char caracter;
	char centinela = '%';

	for (i = 0; caracter != centinela; i++) {

		caracter = paquete[i];

		if (caracter == centinela) {
			i++;
			break;
		}
	}

	char * buffer = malloc(sizeof(char) * i);

	caracter = paquete[0];

	for (i = 0; caracter != centinela; i++) {
		caracter = paquete[i];
		if (caracter == centinela) {
			break;
		}
		memcpy(buffer + i, paquete + i, sizeof(char));
	}

	buffer[i] = '\0';

	return buffer;
}

void enviarLecturaArchivo(const char *path, size_t size, off_t offset) {

	int tamanio = strlen(path) + sizeof(size_t) + sizeof(off_t);

	void*buffer = malloc(tamanio);

	memcpy(buffer,&path,strlen(path));

	memcpy(buffer + strlen(path), &size, sizeof(size_t));

	memcpy(buffer + strlen(path) + sizeof(size_t), &offset, sizeof(off_t));

	t_data *paquete = pedirPaquete(leerArchivo, tamanio, buffer);

	common_send(socketConexion, paquete);

}

/*
 * fuse.c
 *
 *  Created on: 28/9/2016
 *      Author: utnso
 */

#include "teamRocketFuse.h"
#include <so/libSockets.h>

static int teamRocket_getAttr(const char *path, struct stat *stbuf);

static int teamRocket_readDir(const char *path, void *buf,
		fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);

static int teamRocket_read(const char *path, char *buf, size_t size,
		off_t offset, struct fuse_file_info *fi);

void enviarLecturaArchivo(char *path, size_t size, off_t offset);

static int teamRocket_getAttr(const char *path, struct stat *stbuf) {
	int res = 0;

	//Por defecto el punto de montaje es una carpeta.
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		return res;
	} else {
		//memset(stbuf, 0, sizeof(struct stat));
		char * newPath = malloc(strlen(path) + 1);
		strcpy(newPath, path);

		t_data * paquete = pedirPaquete(poke_solicitudGetAttr,
				strlen(newPath) + 1, newPath);
		common_send(socketConexion, paquete);

		free(newPath);
		paquete = leer_paquete(socketConexion);

		switch (paquete->header) {
		case poke_respuestaPorArchivo:
			;
			//log_error(logCliente, "My socket connection is: %s", string_itoa( (long)paquete->data));
			stbuf->st_mode = S_IFREG | 0666;
			stbuf->st_nlink = 1;
			stbuf->st_size = *((long*) paquete->data);
			//stbuf->st_size = 12312;
			return res;
			break;
		case poke_respuestaPorDirectorio:
			stbuf->st_mode = S_IFDIR | 0755;
			stbuf->st_nlink = 2;
			return res;
			break;
		case poke_errorGetAttr:
			res = -ENOENT;
			return res;
			break;
		default:
			;
			return -ENOENT;
			break;
		}
	}
	return -ENOENT;
}

/*
 static int ejemplo_getattr(const char *path, struct stat *stbuf) {
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
 */

static int teamRocket_readDir(const char *path, void *buf,
		fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
	//TODO: ver si falta el \0
	log_error(logCliente, "My socket connection is: %s",
			string_itoa(socketConexion));

	/*
	 char * palabra = malloc(18);
	 strcpy(palabra, "Pokemons");
	 filler(buf,palabra,NULL,0);
	 return 0;
	 */

	char * newPath = malloc(strlen(path) + 1);
	strcpy(newPath, path);

	t_data * paquete = pedirPaquete(poke_solicitudReadDir, strlen(newPath) + 1,
			newPath);
	common_send(socketConexion, paquete);
	paquete = leer_paquete(socketConexion);

	free(newPath);
	if (paquete->header == poke_respuestaReadDir) {

		int _OSADA_FILENAME_LENGTH = 17;
		int temp = 0;
		int aux = 0;

		int cuantosStringsMePasaron = 0;
		cuantosStringsMePasaron = (((paquete->tamanio) / sizeof(char))
				/ (_OSADA_FILENAME_LENGTH + 1));
		log_debug(logCliente, "Me pasaron x strings: %s",
				string_itoa(cuantosStringsMePasaron));

		while (temp < cuantosStringsMePasaron) {
			aux = sizeof(char) * (_OSADA_FILENAME_LENGTH + 1);
			char * palabra = malloc(aux);
			memcpy(palabra, paquete->data + (aux * temp),
					(_OSADA_FILENAME_LENGTH + 1));
			filler(buf, palabra, NULL, 0);
			free(palabra);
			temp++;
		}

		//TODO: parsear string respuesta y fillear
//		while (1) {
//			if (strlen(paquete->data) == 0)
//				break;
//
//
//			char * palabra = malloc(18);
//			//memcpy(palabra, paquete->data, 18);
//			strcpy(palabra, "Pokemons");
//			/*
//			char * palabra = leerHastaCentinela(paquete->data);
//			paquete->data = memchr(paquete->data, '\0', strlen(paquete->data));
//			paquete->data = (char*) paquete->data + 1;
//
//			printf("palabra detectada: %s\n", palabra);
//			*/
//			//log_trace(logCliente,"palabra detectada: %s\n",palabra);
//			//TODO: ahora hay que fillearlo? ver bien el tema de las paternidades
//			filler(buf,palabra,NULL,0);
//			break;
//		}
		return 0;

	} else if (paquete->header == poke_errorReadDir) {
		//TODO: servidor no encontro nada segun el path enviado, que hago?
		return -ENOENT;
	}

	return -ENOENT;
}

/*
 static int ejemplo_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
 off_t offset, struct fuse_file_info *fi) {


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


 return 0;

 }
 */

static int teamRocket_read(const char *path, char *buf, size_t size,
		off_t offset, struct fuse_file_info *fi) {

	char * newPath = malloc(strlen(path) + 1);
	strcpy(newPath, path);
	enviarLecturaArchivo(newPath, size, offset);
	t_data * lectura = leer_paquete(socketConexion);
	free(newPath);

	if (lectura->header == poke_respuestaLectura) {
		//memcpy(buf,lectura->data,size);
		memcpy(buf, lectura->data, lectura->tamanio);
		return lectura->tamanio;
	}
	return 0;

}

//TODO
static int teamRocket_write(const char *path, const char *buf, size_t size,
		off_t offset, struct fuse_file_info *fi) {
	int tamanioArchivo = size; //Me imagino que aca tengo el ta
	/*t_data *paquete = pedirPaquete(escribirArchivo, tamanioArchivo, path);
	 common_send(socketConexion, paquete);
	 */
	return 0;
}
;

static int teamRocket_mkdir(const char *path, mode_t mode) {

	char * newPath = malloc(strlen(path) + 1);
	strcpy(newPath, path);

	t_data * paquete = pedirPaquete(poke_crearDirectorio, strlen(newPath) + 1,
			newPath);
	common_send(socketConexion, paquete);
	t_data * lectura = leer_paquete(socketConexion);
	log_debug(logCliente, "quiere crear la ruta %s", path);

	free(newPath);
	if (lectura->header == poke_respuestaCreacion) {
		//Si no le devolvieron un 0, entonces devuelvo problema.
		if ((*((int*) lectura->data)) == 0)
			return 0;
	}
	return -ENOENT;
}
;

static int teamRocket_rmdir(const char *path) {

	char * newPath = malloc(strlen(path) + 1);
	strcpy(newPath, path);

	t_data * paquete = pedirPaquete(poke_borrarDirectorio, strlen(newPath) + 1,
			newPath);
	common_send(socketConexion, paquete);
	log_debug(logCliente, "quiere borrar la ruta %s", path);

	free(newPath);
	t_data * lectura = leer_paquete(socketConexion);
	if (lectura->header == poke_respuestaBorrado) {
		//Si no le devolvieron un 0, entonces devuelvo problema.
		if ((*((int*) lectura->data)) == 0)
			return 0;
	}
	return -ENOENT;

}
;
/*
 static int ejemplo_read(const char *path, char *buf, size_t size, off_t offset,
 struct fuse_file_info *fi) {


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
 */

/*
 static struct fuse_operations ejemplo_oper = {
 .getattr = ejemplo_getattr,
 .readdir = ejemplo_readdir,
 .read = ejemplo_read,
 };
 */

static int teamRocket_unlink(const char * path) {
	char * newPath = malloc(strlen(path) + 1);
	strcpy(newPath, path);

	t_data * paquete = pedirPaquete(poke_borrarArchivo, strlen(newPath) + 1,
			newPath);
	common_send(socketConexion, paquete);
	log_debug(logCliente, "quiere borrar el file %s", path);

	free(newPath);
	t_data * lectura = leer_paquete(socketConexion);
	if (lectura->header == poke_respuestaBorradoArchivo) {
		//Si no le devolvieron un 0, entonces devuelvo problema.
		if ((*((int*) lectura->data)) == operacionExitosa)
			return 0;
	}
	return -ENOENT;

}
;

static int teamRocket_rename(const char *path, const char *nombre) {
	char * newPath = malloc(strlen(path) + 1);
	strcpy(newPath, path);
	char * newName = malloc(strlen(nombre) + 1);
	strcpy(newName, nombre);
/*
	int largo = strlen(newPath) + strlen(newName) + 2 * sizeof(int);
	void * buffer = calloc(1,largo);

	memcpy(&buffer, strlen(newPath) + 1, sizeof(int));
	memcpy(&buffer+sizeof(int), newPath, strlen(newPath));

	memcpy(&buffer+strlen(newPath), strlen(newName) + 1, sizeof(int));
	memcpy(&buffer+sizeof(int), newName, strlen(newName));
*/

	t_data * paquete = pedirPaquete(poke_renombrar, strlen(newPath) + 1, newPath);
	common_send(socketConexion, paquete);

	t_data * paquete2 = pedirPaquete(poke_renombrar, strlen(newName) + 1, newName);
	common_send(socketConexion, paquete2);

	log_debug(logCliente, "quiere renombrar %s con el nombre %s\n", path, newName);

	free(newPath);
	free(newName);
	t_data * lectura = leer_paquete(socketConexion);
	if (lectura->header == poke_respuestaRenombrado) {
		//Si no le devolvieron un 0, entonces devuelvo problema.
		if ((*((int*) lectura->data)) == 0)
			return 0;
	}
	return -ENOENT;
}
;

static struct fuse_operations teamRocket_oper = { .getattr = teamRocket_getAttr,
		.readdir = teamRocket_readDir, .read = teamRocket_read, .write =
				teamRocket_write, .mkdir = teamRocket_mkdir, .rmdir =
				teamRocket_rmdir, .rename = teamRocket_rename, .unlink =
				teamRocket_unlink };

int iniciarFuse(int argc, char*argv[]) {
	/*
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

	 */
	return fuse_main(argc, argv, &teamRocket_oper, NULL);
}

/*
 char * leerHastaCentinela(char *paquete) {
 int i;
 char caracter;
 char centinela = '\0';

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
 */
void enviarLecturaArchivo(char *path, size_t size, off_t offset) {

	t_data *paquete = pedirPaquete(poke_leerArchivo, strlen(path) + 1, path);
	common_send(socketConexion, paquete);

	int tamanio = sizeof(size_t) + sizeof(off_t);
	void*buffer = malloc(tamanio);
	memcpy(buffer, &size, sizeof(size_t));
	memcpy(buffer + sizeof(size_t), &offset, sizeof(off_t));
	paquete = pedirPaquete(poke_leerArchivo, tamanio, buffer);
	common_send(socketConexion, paquete);

}

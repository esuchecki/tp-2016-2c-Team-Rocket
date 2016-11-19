/*
 * fuse.c
 *
 *  Created on: 28/9/2016
 *      Author: utnso
 */

#include "teamRocketFuse.h"
#include <so/libSockets.h>
#include <sys/stat.h>
#include "so/tiempos.h"

static int teamRocket_getAttr(const char *path, struct stat *stbuf);

static int teamRocket_readDir(const char *path, void *buf,
		fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);

static int teamRocket_read(const char *path, char *buf, size_t size,
		off_t offset, struct fuse_file_info *fi);

static int teamRocket_mkdir(const char *path, mode_t mode);

static int teamRocket_rmdir(const char *path);

void enviarLecturaArchivo(char *path, size_t size, off_t offset);

static int teamRocket_unlink(const char * path);

static int teamRocket_truncar(const char *path, off_t largo);
void enviarEscrituraArchivo(const char *buf, size_t size, off_t offset);
static int teamRocket_opendir(const char* path, struct fuse_file_info* fi);
static int teamRocket_releasedir(const char* path, struct fuse_file_info* fi);
static int teamRocket_open(const char* path, struct fuse_file_info* fi);
static int teamRocket_close(const char* path, struct fuse_file_info* fi);
int fcAuxiliarOpen(char * path, int enumRespuesta);
int fcAuxiliarClose(char * path, int enumRespuesta);





static int teamRocket_getAttr(const char *path, struct stat *stbuf) {
	int res = 0;
	struct timespec tiempoDeModificacion;

	//Por defecto el punto de montaje es una carpeta.
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		convertirSegundosToTimeSpec(&tiempoDeModificacion,(unsigned) time(NULL));
		stbuf->st_mtim = tiempoDeModificacion;
		return res;
	} else {
		//memset(stbuf, 0, sizeof(struct stat));
		char * newPath = malloc(strlen(path) + 1);
		strcpy(newPath, path);

		if (fcAuxiliarOpen(newPath,poke_solicitudGetAttr) ==0)
		{
			t_data * paquete = pedirPaquete(poke_solicitudGetAttr,
					strlen(newPath) + 1, newPath);
			common_send(socketConexion, paquete);

			free(newPath);
			paquete = leer_paquete(socketConexion);
			uint32_t fechaModificacion;
			struct timespec time1;

			fcAuxiliarClose(path, poke_solicitudGetAttr);	//que cierre el archivo.

			switch (paquete->header) {
			case poke_respuestaPorArchivo:
				;
				//log_error(logCliente, "My socket connection is: %s", string_itoa( (long)paquete->data));
				stbuf->st_mode = S_IFREG | 0666;
				stbuf->st_nlink = 1;

				//asigno el tamanio
				memcpy( &(stbuf->st_size), paquete->data, sizeof(long));
				memcpy( &fechaModificacion, (paquete->data) + sizeof(long), sizeof(uint32_t));

				//stbuf->st_size = *((long*) paquete->data);
				//fechaModificacion = *((uint32_t*) (paquete->data+sizeof(long*)));
				//log_debug(logCliente, "%s", string_itoa(asd));

				if (fechaModificacion != -archivoNoEncontrado)
				{
					log_error(logCliente, "Mira la fechaHora: %s", string_itoa(fechaModificacion ));
					convertirSegundosToTimeSpec(&time1, fechaModificacion);
					stbuf->st_mtim = time1;
				}
				free(paquete);
				return res;
				break;
			case poke_respuestaPorDirectorio:
				;
				fechaModificacion = *((uint32_t *) paquete->data);
				stbuf->st_mode = S_IFDIR | 0755;
				stbuf->st_nlink = 2;

				if (fechaModificacion != -archivoNoEncontrado)
				{
					log_error(logCliente, "Mira la fechaHora: %s", string_itoa(fechaModificacion ));
					convertirSegundosToTimeSpec(&time1, fechaModificacion);
					stbuf->st_mtim = time1;
				}
				free(paquete);
				return res;
				break;
			case poke_errorGetAttr:
				res = -ENOENT;
				free(paquete);
				return res;
				break;
			default:
				;
				free(paquete);
				return -ENOENT;
				break;
			}
		}
		free(newPath);
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
		free(paquete);
		return 0;

	} else if (paquete->header == poke_errorReadDir) {
		//TODO: servidor no encontro nada segun el path enviado, que hago?
		free(paquete);
		return -ENOENT;
	}
	free(paquete);
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
		int tamanioFinal = lectura->tamanio;
		free(lectura);
		return tamanioFinal;
	}
	free(lectura);
	//En caso de else o recibir "poke_errorEnLectura" devuelve 0.
	return 0;

}


static int teamRocket_write(const char *path, const char *buf, size_t size,	off_t offset, struct fuse_file_info *fi) {
	char * newPath = malloc(strlen(path) + 1);
	strcpy(newPath, path);
	int resultadoTruncar = teamRocket_truncar(newPath, (size+offset));

	if ( resultadoTruncar == operacionExitosa)
	{

		t_data * paquete = pedirPaquete(poke_escribirArchivo, strlen(newPath) + 1,	newPath);
		common_send(socketConexion, paquete);

		enviarEscrituraArchivo(buf, size, offset);
		t_data * lectura = leer_paquete(socketConexion);
		free(newPath);
		free(paquete);

		if (lectura->header == poke_respuestaEscritura) {
			//Si no le devolvieron un 0, entonces devuelvo problema.
			if ((*((int*) lectura->data)) == operacionExitosa)
			{
				free(lectura);
				return size;
			}
		}
		free(lectura);
	}else
	{
		free(newPath);
		return resultadoTruncar;
	}
	return -EAGAIN;
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

	free(paquete);
	free(newPath);

	if (lectura->header == poke_respuestaCreacion) {
		//Si no le devolvieron un 0, entonces devuelvo problema.
		int result = (*((int*) lectura->data));
		free(lectura);

		if (result == operacionExitosa)
		{
			return 0;
		}
		if (result <0)
		{
			//devuelvo el codigo de error.
			return result;
		}
		return -EAGAIN;	//intente nuevamente.
	}
	free(lectura);
	return -EAGAIN;	//intente nuevamente.
}
;

static int teamRocket_rmdir(const char *path) {

	char * newPath = malloc(strlen(path) + 1);
	strcpy(newPath, path);

	t_data * paquete = pedirPaquete(poke_borrarDirectorio, strlen(newPath) + 1,
			newPath);
	common_send(socketConexion, paquete);
	log_debug(logCliente, "quiere borrar la ruta %s", path);

	free(paquete);
	free(newPath);

	t_data * lectura = leer_paquete(socketConexion);
	if (lectura->header == poke_respuestaBorrado) {
		//Si no le devolvieron un 0, entonces devuelvo problema.
		int result = (*((int*) lectura->data));
		free(lectura);

		if (result == operacionExitosa)
		{
			return 0;
		}
		if (result <0)
		{
			//devuelvo el codigo de error.
			return result;
		}
		return -EAGAIN;	//intente nuevamente.
	}
	free(lectura);
	return -EAGAIN;	//intente nuevamente.

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
	free(paquete);

	t_data * lectura = leer_paquete(socketConexion);
	if (lectura->header == poke_respuestaBorradoArchivo) {
		//Si no le devolvieron un 0, entonces devuelvo problema.
		int result = (*((int*) lectura->data));
		free(lectura);

		if (result == operacionExitosa)
		{
			return 0;
		}
		if (result <0)
		{
			//devuelvo el codigo de error.
			return result;
		}
		return -EAGAIN;	//intente nuevamente.
	}
	free(lectura);
	return -EAGAIN;	//intente nuevamente.

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

	t_data * paquete = pedirPaquete(poke_renombrar, strlen(newPath) + 1,
			newPath);
	common_send(socketConexion, paquete);

	t_data * paquete2 = pedirPaquete(poke_renombrar, strlen(newName) + 1,
			newName);
	common_send(socketConexion, paquete2);

	log_debug(logCliente, "quiere renombrar %s con el nombre %s\n", path,
			newName);

	free(newPath);
	free(newName);
	free(paquete);
	free(paquete2);

	t_data * lectura = leer_paquete(socketConexion);
	if (lectura->header == poke_respuestaRenombrado) {
		//Si no le devolvieron un 0, entonces devuelvo problema.
		int result = (*((int*) lectura->data));
		free(lectura);

		if (result == operacionExitosa)
		{
			return 0;
		}
		if (result <0)
		{
			//devuelvo el codigo de error.
			return result;
		}
		return -EAGAIN;	//intente nuevamente.
	}
	free(lectura);
	return -EAGAIN;	//intente nuevamente.
}
;

static int teamRocket_truncar(const char *path, off_t size) {
	char * newPath = malloc(strlen(path) + 1);
	strcpy(newPath, path);

	log_debug(logCliente, "va a leer el paquete 1 \n");
	t_data * paquete = pedirPaquete(poke_truncar, strlen(newPath) + 1, newPath);
	common_send(socketConexion, paquete);
	log_debug(logCliente, "va a leer el paquete 2 \n");

	t_data * paquete2 = pedirPaquete(poke_truncar, sizeof(off_t), &size);
	common_send(socketConexion, paquete2);

	log_debug(logCliente, "quiere truncar %s con el largo %l \n", path, size);

	free(newPath);
	free(paquete);
	free(paquete2);

	t_data * lectura = leer_paquete(socketConexion);
	if (lectura->header == poke_respuestaTruncado) {
		//Si no le devolvieron un 0, entonces devuelvo problema.
		int result = (*((int*) lectura->data));
		free(lectura);

		if (result == operacionExitosa)
		{
			return 0;
		}
		if (result <0)
		{
			//devuelvo el codigo de error.
			return result;
		}
		return -EAGAIN;	//intente nuevamente.
	}
	free(lectura);
	return -EAGAIN;	//intente nuevamente.
}
;

static int teamRocket_makeNod(const char *path, mode_t mode, dev_t rdev)
{
	if (S_ISREG(mode))
		return teamRocket_truncar(path, 0);

	return -errno;
}
;

static int teamRocket_utimensat(const char* path, const struct timespec ts[2])
{
	//Update the last access time of the given object from ts[0]
	//Update the last modification time from ts[1]
	char * newPath = malloc(strlen(path) + 1);
	strcpy(newPath, path);

	log_debug(logCliente, "va a leer el paquete 1 \n");
	t_data * paquete = pedirPaquete(poke_utimensat, strlen(newPath) + 1, newPath);
	common_send(socketConexion, paquete);


	uint32_t fechaModificacion = (unsigned) ts[1].tv_sec;

	t_data * paquete2 = pedirPaquete(poke_utimensat, sizeof(uint32_t), &fechaModificacion);
	common_send(socketConexion, paquete2);

	free(newPath);
	free(paquete);
	free(paquete2);

	t_data * lectura = leer_paquete(socketConexion);
	if (lectura->header == poke_respuestaUtimensat) {
		//Si no le devolvieron un 0, entonces devuelvo problema.
		int result = (*((int*) lectura->data));
		free(lectura);

		if (result == operacionExitosa)
		{
			return 0;
		}
		if (result <0)
		{
			//devuelvo el codigo de error.
			return result;
		}
		return -EAGAIN;	//intente nuevamente.
	}
	free(lectura);
	return -EAGAIN;	//intente nuevamente.

}
;


static int teamRocket_opendir(const char* path, struct fuse_file_info* fi)
{
	char * newPath = malloc(strlen(path) + 1);
	strcpy(newPath, path);

	int i = fcAuxiliarOpen(newPath, poke_respuestaPorDirectorio);
	free(newPath);
	return i;
}
;

static int teamRocket_releasedir(const char* path, struct fuse_file_info* fi)
{
	char * newPath = malloc(strlen(path) + 1);
	strcpy(newPath, path);

	int i = fcAuxiliarClose(newPath, poke_respuestaPorDirectorio);
	free(newPath);
	return i;
}
;

int fcAuxiliarOpen(char * path, int enumRespuesta)
{
	log_debug(logCliente, "va a leer el paquete 1 \n");
	t_data * paquete = pedirPaquete(poke_abrirArchivo, strlen(path) + 1, path);
	common_send(socketConexion, paquete);

	uint8_t modoApertura=enumRespuesta;

	t_data * paquete2 = pedirPaquete(poke_abrirArchivo, sizeof(uint8_t), &modoApertura);
	common_send(socketConexion, paquete2);

	t_data * lectura = leer_paquete(socketConexion);
	if (lectura->header == poke_respuestaApertura) {
		//Si no le devolvieron un 0, entonces devuelvo problema.
		if ((*((int*) lectura->data)) == operacionExitosa)
		{
			//chequeo si es un directorio.
			//paquete = pedirPaquete(poke_solicitudGetAttr, strlen(newPath) + 1, newPath);
			//common_send(socketConexion, paquete);



			//paquete = leer_paquete(socketConexion);
			//if (paquete->header == poke_respuestaPorArchivo)
			//{
				free(lectura);
				free(paquete);
				return 0;
			//}

		}
	}
	free(lectura);
	free(paquete);
//}
return -ENOENT;
}


int fcAuxiliarClose(char * path, int enumRespuesta)
{
	log_debug(logCliente, "va a leer el paquete 1 \n");
	t_data * paquete = pedirPaquete(poke_cerrarArchivo, strlen(path) + 1, path);
	common_send(socketConexion, paquete);

	uint8_t modoApertura=enumRespuesta;

	t_data * paquete2 = pedirPaquete(poke_cerrarArchivo, sizeof(uint8_t), &modoApertura);
	common_send(socketConexion, paquete2);

	t_data * lectura = leer_paquete(socketConexion);
	if (lectura->header == poke_respuestaClose) {
		//Si no le devolvieron un 0, entonces devuelvo problema.
		if ((*((int*) lectura->data)) == operacionExitosa)
		{
			//chequeo si es un directorio.
			//paquete = pedirPaquete(poke_solicitudGetAttr, strlen(newPath) + 1, newPath);
			//common_send(socketConexion, paquete);



			//paquete = leer_paquete(socketConexion);
			//if (paquete->header == poke_respuestaPorArchivo)
			//{
				free(lectura);
				free(paquete);
				return 0;
			//}

		}
	}
	free(lectura);
	free(paquete);
//}
return -ENOENT;
}



static int teamRocket_open(const char* path, struct fuse_file_info* fi)
{
	char * newPath = malloc(strlen(path) + 1);
	strcpy(newPath, path);

	int i = fcAuxiliarOpen(newPath, poke_respuestaPorArchivo);
	free(newPath);
	return i;
}
;

static int teamRocket_close(const char* path, struct fuse_file_info* fi)
{
	char * newPath = malloc(strlen(path) + 1);
	strcpy(newPath, path);

	int i = fcAuxiliarClose(newPath, poke_respuestaPorArchivo);
	free(newPath);
	return i;
}
;


static struct fuse_operations teamRocket_oper = {
		.getattr 	= teamRocket_getAttr,
		.readdir 	= teamRocket_readDir,
		.read 		= teamRocket_read,
		.write 		= teamRocket_write,
		.mkdir 		= teamRocket_mkdir,
		.rmdir 		= teamRocket_rmdir,
		.truncate 	= teamRocket_truncar,
		.rename 	= teamRocket_rename,
		.unlink 	= teamRocket_unlink,
		.mknod		= teamRocket_makeNod,
		.utimens 	= teamRocket_utimensat,
		.opendir 	= teamRocket_opendir,
		.releasedir	= teamRocket_releasedir,
		.open		= teamRocket_open,
		.release	= teamRocket_close,
		.flag_nullpath_ok = 0,
};

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

	free(buffer);
	free(paquete);
}

void enviarEscrituraArchivo(const char *buf, size_t size, off_t offset) {
	t_data *paquete = pedirPaquete(poke_escribirArchivo, size, buf);
	common_send(socketConexion, paquete);

	int tamanio = sizeof(size_t) + sizeof(off_t);
	void*buffer = malloc(tamanio);
	memcpy(buffer, &size, sizeof(size_t));
	memcpy(buffer + sizeof(size_t), &offset, sizeof(off_t));
	paquete = pedirPaquete(poke_escribirArchivo, tamanio, buffer);
	common_send(socketConexion, paquete);
}

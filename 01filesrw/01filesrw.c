#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <ctype.h>

/*
 * return 1 if file exists, else 0
 */
int check_file_exist(char * fname) {

	// test per permessi di lettura, scrittura...?? usare R_OK, W_OK....

	/*
	 * prova da fare:
	 * creare un file come root, e tirare via permessi rwx a resto del mondo.
	 * cosa succede se verifico esistenza file da utente?
	 *
	 * prova 2:
	 * come root: crea una cartella (e poi chmod go-rwx cartella), dentro la cartella crea un file; cosa succederà?
	 */

	if( access( fname, F_OK ) != -1 ) {
	    // file exists
		return 1;
	} else {
	    // file doesn't exist
		perror("access");
		return 0;
	}

}

/*
 * return 1 if successful, else 0
 */
int create_file(char * fname) {

	int fd;

	// se il file esiste già, open ha successo ma il file non viene 'troncato'
	fd = open(fname, O_CREAT | O_RDWR, 00600);

	if (fd == -1) {
		perror("create_file error\n");
		return 0;
	}

	close(fd);

	return 1;
}

/*
 * return 1 if successful, else 0
 */
int create_file_trunc(char * fname) {

	int fd;

	// se il file esiste già, il file viene 'troncato' (precedenti contenuti sono eliminati e size diventa zero)
	fd = open(fname, O_CREAT | O_TRUNC | O_WRONLY, 00600);

	if (fd == -1) {
		perror("create_file error\n");
		return 0;
	}

	close(fd);

	return 1;
}

int write_to_file(char * fname, char * content) {

	int fd;

	// se il file esiste già, open ha successo ma il file non viene 'troncato'
	fd = open(fname, O_CREAT | O_RDWR, 00600);

	if (fd == -1) {
		perror("write_to_file error\n");
		return 0;
	}

	int res;

	res = write(fd, content, strlen(content));

	if (res == -1) {
		perror("write_to_file error\n");
	}

	close(fd);

	return 1;
}

int write_to_file_append(char * fname, char * content) {
	int fd;

	// se il file esiste già, open ha successo ma il file non viene 'troncato'
	fd = open(fname, O_CREAT | O_RDWR | O_APPEND, 00600);

	if (fd == -1) {
		perror("write_to_file_append error\n");
		return 0;
	}

	int res;

	res = write(fd, content, strlen(content));

	if (res == -1) {
		perror("write_to_file_append error\n");
	}

	close(fd);

	return 1;
}


int write_to_file_seek(char * fname, char * content) {
	int fd;

	// se il file esiste già, open ha successo ma il file non viene 'troncato'
	fd = open(fname, O_CREAT | O_RDWR, 00600);

	if (fd == -1) {
		perror("write_to_file_seek error\n");
		return 0;
	}

	// chiedi dove posizionarti nel file
	printf("posizione all'interno del file: ");
	int seek_position;

	scanf("%d", & seek_position);

	off_t offset = lseek(fd, seek_position, SEEK_SET);

	if (offset == -1) {
		perror("write_to_file_seek error\n");
	}


	int res;

	res = write(fd, content, strlen(content));

	if (res == -1) {
		perror("write_to_file_seek error\n");
	}

	close(fd);

	return 1;
}


int read_file(char * fname) {


	return 1;
}


unsigned long get_file_size(char * fname) {

	struct stat st;

	int res = stat(fname, &st);

	if (res == -1) {
		perror("stat error");
		return -1;
	} else
		return st.st_size;

}


int unlink_file(char * fname) {

	int res = unlink(fname);

	if (res == -1) {
		perror("unlink error");
		return 0;
	} else
		return 1;

}

int main(int argc, char *argv[]) {

	if (argc < 3) {

		printf("%s <comando> <nome_file> [contenuto]\n", argv[0]);

		printf("comandi:\n");

		printf("esiste  : controlla se esiste file\n");

		printf("crea    : crea file (se non esiste già)\n");

		printf("crea-tronca  : crea e tronca (cancella contenuti se esiste già)\n");

		printf("scrivi : scrivi il contenuto nel file\n");

		printf("scrivi-in-fondo : scrivi il contenuto alla fine del file (append)\n");

		printf("scrivi-in-mezzo : scrivi il contenuto in mezzo al file\n");

		printf("leggi : leggi e mostra su stdout il contenuto del file\n");

		printf("size: restituisci dimensione del file\n");

		printf("unlink: cancella il nome dal filesystem\n");

		return EXIT_FAILURE;
	}

	char * cmd = argv[1];

	for (char * p = cmd ; *p; ++p) *p = tolower(*p);

	char * file_name = argv[2];
	char * contenuto = argc > 3 ? argv[3] : "blah blah";

	if (!strcmp(cmd,"esiste")) {

		printf("check_file_exist %s : %s\n", file_name, check_file_exist(file_name) ? "true" : "false");

	} else if (!strcmp(cmd,"crea")) {

		printf("create_file %s : %s\n", file_name, create_file(file_name) ? "OK" : "ERR");

	} else if (!strcmp(cmd,"crea-tronca")) {

		printf("create_file_trunc %s : %s\n", file_name, create_file_trunc(file_name) ? "OK" : "ERR");

	} else if (!strcmp(cmd,"scrivi")) {

		printf("write_to_file %s : %s\n", file_name, write_to_file(file_name, contenuto) ? "OK" : "ERR");

	} else if (!strcmp(cmd,"scrivi-in-fondo")) {

		printf("write_to_file_append %s : %s\n", file_name, write_to_file_append(file_name, contenuto) ? "OK" : "ERR");

	} else if (!strcmp(cmd,"scrivi-in-mezzo")) {

		printf("write_to_file_seek %s : %s\n", file_name, write_to_file_seek(file_name, contenuto) ? "OK" : "ERR");

	} else if (!strcmp(cmd,"leggi")) {

		printf("read_file %s : %s\n", file_name, read_file(file_name) ? "OK" : "ERR");

	} else if (!strcmp(cmd,"size")) {

		printf("get_file_size %s : %lu\n", file_name, get_file_size(file_name));

	} else if (!strcmp(cmd,"unlink")) {

		printf("unlink_file %s : %s\n", file_name, unlink_file(file_name) ? "OK" : "ERR");

	}


	return 0;

}

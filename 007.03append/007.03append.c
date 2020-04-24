#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

/*
 * esempio in cui un file viene modificato aggiungendo in fondo allo stesso del contenuto
 */


int main(int argc, char * argv[]) {

	char * file_name;
	int res;
	off_t file_offset;

	if (argc == 1) {
		printf("specificare come parametro il nome del file\n");
		exit(EXIT_FAILURE);
	}

	file_name = argv[1];
	printf("opero sul file %s\n", file_name);

	// apro un file già esistente in modalità lettura/scrittura
	// ad esempio, possiamo lavorare su un file prodotto dall'esempio 005.8writefile
	int fd = open(file_name, O_RDWR); // O_RDWR: apri il file in modalità lettura e scrittura

	if (fd == -1) { // errore!
		perror("open()");

		exit(EXIT_FAILURE);
	} else {
		printf("open() ok\n");
	}

	// posizioniamo l'offset in fondo al file
	if ((file_offset = lseek(fd, 0, SEEK_END)) == -1) {
		perror("lseek()");
		exit(EXIT_FAILURE);
	}

	printf("current file length: %ld\n", file_offset);

	char * append_message = "0123456789";
	res = write(fd, append_message, strlen(append_message));

	if (res == -1) {
		perror("write()");
		exit(EXIT_FAILURE);
	} else {
		printf("write ok\n");
	}

	if (close(fd) == -1) {
		perror("close()");
	}

	printf("bye!\n");

	return 0;
}

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>


#define BUFFER_SIZE 1024

char buffer[BUFFER_SIZE];

int main(int argc, char *argv[]) {

	//char * file_name = "/home/utente/prova.txt";
	//char * file_name = "/proc/cpuinfo";

	char * file_name;

	if (argc == 1) {
		printf("specificare come parametro il nome del file da leggere\n");
		exit(EXIT_FAILURE);
	}

	file_name = argv[1];
	printf("leggo il file %s\n", file_name);

	int bytes_read;
	unsigned int total_bytes_read = 0;

	int fd = open(file_name, O_RDONLY); // O_RDONLY: apri il file in sola lettura

	if (fd == -1) { // errore!

		perror("open()");

		exit(EXIT_FAILURE);
	} else {
		printf("open() ok\n");
	}

	// i file descriptor 0,1,2 sono già utilizzati

	printf("'file descriptor' restituito da open():  fd = %d\n", fd);

	while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {

		printf("read() ha restituito %d bytes\n", bytes_read);

		// facciamo qualcosa con i dati letti...
		// ...

		total_bytes_read += bytes_read;
	}

	if (bytes_read == -1) {
		perror("read()");
	} else if (bytes_read == 0) {
		printf("OK, usciti da while per EOF (End Of File)\n");
	}
	printf("total_bytes_read = %u\n", total_bytes_read);

	if (close(fd) == -1) {
		perror("close()");
	}


	return EXIT_SUCCESS;
}

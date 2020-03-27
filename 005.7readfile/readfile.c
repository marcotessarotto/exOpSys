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

	char * file_name = "/home/marco/prova.txt";
//	char * fileName = "/proc/cpuinfo";

	int bytes_read;
	int total_bytes_read = 0;

	int fd = open(file_name, O_RDONLY);

	if (fd == -1) { // errore!

		perror("open()");

		exit(EXIT_FAILURE);
	}

	printf("fd = %d\n", fd);

	while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {

		printf("ho letto %d bytes\n", bytes_read);

		// facciamo qualcosa con i dati letti...
		// ...

		total_bytes_read += bytes_read;
	}

	if (bytes_read == -1) {
		perror("read()");
	} else if (bytes_read == 0) {
		printf("OK, usciti da while per EOF (End Of File)\n");
	}

	if (close(fd) == -1) {
		perror("close()");
	}


	return EXIT_SUCCESS;
}

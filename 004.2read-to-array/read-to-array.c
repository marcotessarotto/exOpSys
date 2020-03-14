#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


static void sig_handler(int sig) {
	printf("ho ricevuto il segnale %d\n", sig);
}


int main(int argc, char * argv[]) {

	// il file descriptor 0 è stato predisposto dal kernel per ...
	// quali altri file descriptor sono pronti per l'uso?

	// provare a mandare il segnale -2 al programma
	if (signal(SIGINT, sig_handler) == SIG_ERR) {
		perror("signal");
	}

	char * data;
	unsigned int size = 10;

	// allochiamo un array di dimensione total in bytes ...
	data = calloc(size, sizeof(char));

	// e leggiamo dei dati da stdin con la system call read

	ssize_t res;

	// system call read
	// chiediamo al kernel di leggere dal file descriptor 0 (stdin)
	// fino a (size * sizeof(char)) bytes
	res = read(STDIN_FILENO, data, size * sizeof(char));

	if (res == -1) {
		perror("read error/signal");
	} else if (res == 0) {
		printf("read ha restituito EOF (End of file)\n");
	} else { // res > 0
		printf("read ha scritto nell'array data %ld bytes.\n", res);

		for (int i = 0; i < res; i++) {
			printf("%02x  ", data[i] & 0xff);
		}
	}

	// provare a mandare 😸 al programma

	return 0;
}

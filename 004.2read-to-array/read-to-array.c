#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>


static void sig_handler(int sig) {
	printf("ho ricevuto il segnale %d\n", sig);
}

//#define DESCRITTORE_ZERO 0
//const int FD0 = DESCRITTORE_ZERO;

int main(int argc, char * argv[]) {

	// il file descriptor 0 è stato predisposto dal kernel per ...
	// quali altri file descriptor sono pronti per l'uso?

	// provate questo (da shell):
	// /home/utente/git/exOpSys/004.2read-to-array
	// Debug/004.2read-to-array < /home/utente/prova.txt

	// come trovo il process id di questo programma?
	// da shell: https://www.youtube.com/watch?v=JxSc5mQltII

	// oppure con la system call getpid(), vedere: man 2 getpid
	pid_t pid = getpid();

	printf("il mio process id è %u\n", pid);


	// provare a mandare il segnale 2 al programma
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

//	while ((res = read(.....)) > 0) {
//
//	}

	res = read(STDIN_FILENO , data, size * sizeof(char));
	// esattamente equivalente a:
	// res = read(0 , data, size * sizeof(char));

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


/*
 * esempio:
 *
 * processo padre prepara una pipe e la usa per inviare al processo figlio dei dati
 *
 */

#define READ_BUFFER_SIZE 4096

#define WRITE_BUFFER_SIZE 4096*16*4

int main(int argc, char * argv[]) {

	int pipe_fd[2];
	int res;
	char * buffer;

	if (pipe(pipe_fd) == -1) {
		perror("pipe()");

		exit(EXIT_FAILURE);
	}

	// pipe_fd[0] : estremità di lettura della pipe
	// pipe_fd[1] : estremità di scrittura della pipe

	switch (fork()) {
		case -1:
			perror("problema con fork");

			exit(EXIT_FAILURE);

		case 0: // processo FIGLIO: legge dalla PIPE

			close(pipe_fd[1]); // chiudiamo l'estremità di scrittura della pipe

			buffer = malloc(READ_BUFFER_SIZE);
			if (buffer == NULL) {
				perror("malloc()");
				exit(EXIT_FAILURE);
			}

			// pipe vuota: read() si blocca in attesa di dati
			while ((res = read(pipe_fd[0], buffer, READ_BUFFER_SIZE)) > 0) {
				printf("[child] received %d bytes from pipe\n", res);
				// facciamo qualcosa con i dati ricevuti...
			}

			if (res == -1) {
				perror("read()");
			} else {
				printf("[child] EOF on pipe\n");
			}

			printf("[child] bye\n");

			close(pipe_fd[0]);

			exit(EXIT_SUCCESS);

		default: // processo PADRE: scrive nella PIPE

			printf("[parent] starting\n");

			close(pipe_fd[0]); // chiudiamo l'estremità di lettura della pipe

			buffer = malloc(WRITE_BUFFER_SIZE);

			memset(buffer, 'Z', WRITE_BUFFER_SIZE);

			// se pipe piena (capacità: 16 pages) allora write() si blocca
			res = write(pipe_fd[1], buffer, WRITE_BUFFER_SIZE);
			if (res == -1) {
				perror("write()");
			}

			printf("[parent] %d bytes written to pipe\n", res);

			close(pipe_fd[1]); // chiudiamo estremità di scrittura della pipe
			// dall'altra parte verrà segnalato EOF

			printf("[parent] before wait()\n");

			wait(NULL);

			printf("[parent] bye\n");

			exit(EXIT_SUCCESS);
	}



}

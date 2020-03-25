#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {

	pid_t child_pid;

	child_pid = fork();

	if (child_pid == 0) {
		// sono il processo figlio (child process)

		printf("[child]hello world! sono il processo figlio. ora dormirò per qualche secondo...\n");

		sleep(5);

		printf("[child]sto per terminare! bye!\n");

		exit(EXIT_SUCCESS); // processo figlio termina

	} else if (child_pid > 0) {
		// sono il processo padre

		// aspetto la conclusione del processo figlio (c'è ne uno solo)
		if (wait(NULL) == -1) {
			perror("wait error");
		} else {
			printf("[parent]il processo figlio ha terminato! ora termino anche io.\n");
		}

		exit(EXIT_SUCCESS);

	} else {
		perror("fork()");

		exit(EXIT_FAILURE);
	}


}


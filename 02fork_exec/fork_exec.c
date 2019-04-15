#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int var;

/*
 * esempio dove si utilizzano fork, execve, wait
 *
 */

int main(int argc, char *argv[]) {

	var = 100;

	printf("[parent] prima di fork, var = %d\n", var);

	printf("[parent] il mio PID è: %d\n", getpid());

	int child_pid;

	if ((child_pid = fork()) == 0) {

		// questo è il processo FIGLIO

		printf("[child] sono il processo figlio, il mio PID è: %d\n", getpid());

		printf("[child] var = %d\n", var);


		char * newargv[] = { "ls", "-l", "/home/utente", NULL };

	    char * newenviron[] = { NULL };

	    printf("[child] prima di execve\n");

	    execve("/bin/ls", newargv, newenviron);

	    perror("[child] se siamo qui, qualcosa è andato storto\n");

	    exit(EXIT_FAILURE);

	} else {

		// questo è il processo PARENT

		var = 200;

		printf("[parent] dopo il fork, sono il processo parent\n"
			   "[parent] il PID del processo figlio è %d\n", child_pid);

		// se vogliamo aspettare la terminazione del processo figlio:
		if (wait(NULL) == -1) {
			perror("wait error");
		}

	}

	printf("io sono il %s\n", child_pid == 0 ? "figlio" : "parent");


	return 0;


//    char * newargv[] = { "123", "-l", "/home/utente", NULL };
//
//    char * newenviron[] = { NULL };
//
//    printf("ciao, prima di execve\n");
//
//    execve("/bin/ls", newargv, newenviron);
//
//    perror("se siamo qui, qualcosa è andato storto\n");
//
//	return 1;
}

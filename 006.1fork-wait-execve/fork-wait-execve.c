#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h> // errno
#include <signal.h> // signal

void replace_program_in_process(char * program) {

	// parametri per il programma che invocheremo con execve
	char * new_arguments[] = { program, NULL };

	// variabili di environment per il programma che invocheremo con execve
	char * new_environment_variables[] = { NULL };

	execve(program, new_arguments, new_environment_variables);

	// On success, execve() does not return, on error -1 is returned, and errno is set appropriately.

	perror("execve");

	exit(EXIT_FAILURE);
}


int main(int argc, char *argv[]) {

	pid_t child_pid;

	switch(fork()) {
	case 0:
		// child process

		replace_program_in_process("/usr/bin/pwd");

		break;
	case -1:

		perror("fork()");

		exit(EXIT_FAILURE);

		break;
	default:

		child_pid = wait(NULL);

		if (child_pid == -1) {
			perror("wait()");

			exit(EXIT_FAILURE);
		} else {
			printf("\nil processo figlio %u è terminato.\n", child_pid);
		}

	}

	return 0;
}

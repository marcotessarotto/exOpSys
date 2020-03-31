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

//#define SHOW_RETURN_VALUE

int main(int argc, char *argv[]) {

	pid_t child_pid;
	int wstatus;
	int modal_result = -1;

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

#ifdef SHOW_RETURN_VALUE
		do {
        	pid_t ws = waitpid(child_pid, &wstatus, WUNTRACED | WCONTINUED);
            if (ws == -1) {
                perror("[parent] waitpid");
                exit(EXIT_FAILURE);
            }

            if (WIFEXITED(wstatus)) {

            	modal_result = WEXITSTATUS(wstatus);

                printf("[parent] child process è terminato, ha restituito: %d\n", modal_result);
            } else if (WIFSIGNALED(wstatus)) {
                printf("[parent] child process killed by signal %d\n", WTERMSIG(wstatus));
            } else if (WIFSTOPPED(wstatus)) {
                printf("[parent] child process stopped by signal %d\n", WSTOPSIG(wstatus));
            } else if (WIFCONTINUED(wstatus)) {
                printf("[parent] child process continued\n");
            }
        } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

#else

		// parent process
		child_pid = wait(NULL);

		if (child_pid == -1) {
			perror("wait()");

			exit(EXIT_FAILURE);
		} else {
			printf("\nil processo figlio %u è terminato.\n", child_pid);
		}

#endif

	}

	return 0;
}

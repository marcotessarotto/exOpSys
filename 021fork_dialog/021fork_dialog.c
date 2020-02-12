#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * questo esempio utilizza fork, execve e wait per utilizzare un applicativo esterno (dialog)
 * per proporre all'utente un dialogo modale
 */

//extern char **environ;  /* Or define _GNU_SOURCE to get it from <unistd.h> */

// è necessario dialog, su debian : sudo apt install dialog


// esercizio 1 : aggiungere come parametri alla funzione modal_dialog le stringhe del dialogo
// esercizio 2 : riscrivere modal_dialog usando system(3) invece di fork, execve, waitpid


int modal_dialog() {

	pid_t child_pid;
	int wstatus;

	int modal_result = -1;


	printf("[parent] il mio PID è: %d\n", getpid());


	if ((child_pid = fork()) == 0) {

		// questo è il processo FIGLIO (child process)

		printf("[child] sono il processo figlio (child process), il mio PID è: %d\n", getpid());

		char * newargv[] = {"dialog", "--backtitle", "backtitle del dialog" ,
		       "--title", "titolo del dialog" ,
		       "--yesno", "\nBla bla bla...\n\nTi va bene?", "10", "30", NULL };

		printf("[child] prima di execve\n");

		execve("/usr/bin/dialog", newargv, environ); // passiamo l'environment completo a dialog

		perror("[child] se siamo qui, qualcosa è andato storto\n");

		exit(EXIT_FAILURE);

	} else {

		// questo è il processo PADRE (parent process)

		printf("[parent] dopo il fork, sono il processo padre\n"
			   "[parent] il PID del processo figlio è %d\n", child_pid);

		// aspettiamo il termine del processo figlio e riceviamo il risultato

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

        return modal_result;

	}

}



int main(int argc, char **argv) {

	int scelta_utente = modal_dialog();

	system("/usr/bin/clear"); // puliamo il terminale

	printf("scelta_utente = %d\n", scelta_utente);


	return 0;
}

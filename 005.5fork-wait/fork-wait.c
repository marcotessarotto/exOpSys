#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


/*
 * meccanismo fork-wait per avviare un nuovo processo ed aspettare la sua conclusione
 */

int main(int argc, char *argv[]) {

	if (1) {

		// esempio di uso della sola system call fork()

		// quando il processo figlio è terminato
		// ed il processo padre non lo "aspetta" (wait)
		// allora il processo figlio diventa "zombie"

		// definizione processo "zombie":
		// processo terminato su cui il processo padre
		// non ha invocato la system call wait

		// tutto questo serve a restituire il risultato
		// del processo figlio al processo padre

		switch (fork()) {
			case 0:
				printf("sono il processo figlio\n");

				exit(0);
				break;
			case -1:
				printf("fork() ha fallito! niente processo figlio!\n");

				exit(1);
				break;
			default:

				printf("sono il processo padre\n");

				// per vedere il processo "zombie", decommentare:
				// getchar();
				// ed eseguire in una shell il comando:
				// ps -aux | grep fork-wait

				exit(0);
				// quando il processo padre termina, il processo figlio diventa
				// "orfano" e viene "adottato" dal processo init,
				// che automaticamente invoca wait() per rimuovere gli zombie.
		}


	}

	// esempio di uso delle system call fork() e wait()

	pid_t child_pid;

	child_pid = fork();

	if (child_pid == 0) {
		// sono il processo figlio (child process)

		printf("[child]hello world! sono il processo figlio. ora dormirò per qualche secondo...\n");

		for (int i = 0; i < 5; i++) {
			printf("%d...\n", i+1);
			sleep(1); // dorme 1 secondo
		}

		printf("[child]sto per terminare! bye!\n\n");

		exit(EXIT_SUCCESS); // processo figlio termina

	} else if (child_pid > 0) {
		// sono il processo padre

		printf("[parent]ora aspetterò la conclusione del processo figlio.\n\n");

		// il processo padre aspetta la conclusione del processo figlio (c'è ne uno solo)
		// per vedere come ricevere il risultato del processo figlio,
		// vedere il progetto successivo

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


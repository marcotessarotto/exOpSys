#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


/*
 * meccanismo fork-wait per avviare un nuovo processo ed aspettare la sua conclusione
 *
 * nel primo caso, viene usato fork() e basta
 *
 * nel secondo caso, vengono usati fork() e wait()
 */

int main(int argc, char *argv[]) {

/*
 * man fork
 *
 * fork() creates a new process by duplicating the calling process.
 * The new process is referred to as the child process.
 * The calling process is referred to as the parent process.
 *
 * The child process and the parent process run in separate memory spaces.
 * At the time of fork() both memory spaces have the same content.
 * ...
 *
 * The child process is an exact duplicate of the parent process except for the following points:
 *  The child has its own unique process ID ...
 *  The child's parent process ID is the same as the parent's process ID.
 *  [ child.PPID = parent.PID ]
 *  ...
 * The child inherits copies of the parent's set of open file descriptors.
 * [file descriptors 0, 1, 2 ....]
 *
 * The child inherits the parent's signal handler settings ...
 *
 */

	if (0) {

		// ***CASO 1***

		// esempio di uso della sola system call fork()

		// quando il processo figlio è terminato
		// ed il processo padre non lo "aspetta" (wait)
		// allora il processo figlio diventa "zombie"

		// definizione processo "zombie":
		// processo terminato su cui il processo padre
		// non ha invocato la system call wait

		// tutto questo serve a restituire il risultato
		// del processo figlio al processo padre

		printf("sono il processo padre  (prima del fork)\n");

		/*
		int result = fork();

		if (result == 0) {
			// proecesso figlio
		} else if (result > 0) {
			// processo padre
			// ....
		} else if (result == -1) {
			// errore!
		}
		 */

		// lo switch si svolge in due modi diversi su due processi diversi
		switch (fork()) {
			case 0:
				printf("sono il processo figlio\n");

				exit(0);
				break;
			case -1:
				printf("fork() ha fallito! niente processo figlio!\n");

				exit(1);
				break;
			default: // valore > 0, ovvero il pid del processo figlio

				printf("sono il processo padre\n");

				// per vedere il processo "zombie", decommentare:
				//getchar();
				// ed eseguire in una shell il comando:
				// ps -aux | grep fork-wait

				exit(0);
				// quando il processo padre termina, il processo figlio diventa
				// "orfano" e viene "adottato" dal processo init,
				// che automaticamente invoca wait() per rimuovere gli zombie.
		}

		return 0;

	}

	// ***CASO 2***

	// esempio di uso delle system call fork() e wait()

	printf("[parent] prima di fork()\n");

	pid_t child_pid;

	child_pid = fork();

	// printf("post-fork...\n"); // quante volte viene scritto a video?

	if (child_pid == 0) {

		pid_t my_pid = getpid();

		printf("[child] sono il processo figlio. il mio pid è %u\n", my_pid);

		if (0) {

			printf("[child] ora dormirò per qualche secondo...\n");

			for (int i = 0; i < 5; i++) {
				printf("[child] %d...\n", i+1);
				sleep(1); // dorme 1 secondo
			}
		} else {
			printf("[child] sto per invocare la system call pause()\n");
			printf("[child] usa il comando 'kill %u' per terminarmi.\n\n", my_pid);

			// man pause
			// pause() causes the calling process (or thread) to sleep until
			// a signal is delivered that either terminates the process
			// or causes the invocation of a signal-catching function.
			pause();
		}

		printf("[child] sto per terminare! bye!\n\n");

		exit(EXIT_SUCCESS); // processo figlio termina

	} else if (child_pid > 0) {
		// sono il processo padre

		printf("[parent] ora aspetterò la conclusione del processo figlio.\n\n");

		// il processo padre aspetta la conclusione del processo figlio (c'è ne uno solo)
		// per vedere come ricevere il risultato del processo figlio,
		// vedere il progetto successivo

		// la system call wait sospende l'esecuzione del processo chiamante
		// fino a quando uno dei suoi processi figli termina
		if (wait(NULL) == -1) {
			perror("wait error");
		} else {
			printf("[parent] il processo figlio ha terminato! ora termina il process padre.\n");
		}

		exit(EXIT_SUCCESS);

	} else {
		perror("fork()");

		exit(EXIT_FAILURE);
	}


}


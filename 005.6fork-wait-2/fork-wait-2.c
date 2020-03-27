#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> // nanosleep
#include <errno.h> // errno
#include <signal.h> // signal

int counter = 0;
pid_t my_pid;

void signal_handler(int signum) {

	counter++;

	printf("signal handler processo [%u] - segnale SIGUSR1 ricevuto!  counter=%d\n\n", my_pid, counter);

}

/*
 * il processo padre crea un processo figlio con la system call fork()
 *
 * questo esempio mostra come le chiamate di sistema (system call)
 * possano essere interrotte e come si comportano i processi
 *
 * nanosleep() è una system call di lunga durata
 * (perchè chiediamo di "dormire" per 10 secondi)
 *
 * nel processo figlio:
 * mentre la system call nanosleep è in esecuzione (dentro il kernel),
 * arriva il segnale SIGUSR1 che interrompe il processo.
 * allora la system call nanosleep interrompe quello che stava facendo ("dormire")
 * facendo continuare l'esecuzione al processo in user space.
 * nanosleep ritorna e restituisce quanto "lavoro" è stato fatto; sta al processo
 * decidere se chiamare ancora la system call (nanosleep)
 * [questo è l'approccio Unix alla interruzione delle system call]
 *
 *
 * il processo padre manda, ad intervallo di 1 secondo, per tre volte,
 * un segnale SIGUSR1 (User-defined signal 1) al processo figlio
 * (la trasmissione di SIGUSR1 serve a mostrare che nanosleep può essere interrotto)
 *
 * poi il processo padre aspetta la conclusione del processo figlio e mostra il valore restituito
 *
 * il processo figlio fa una system call di "lunga durata" (dorme per 10 secondi)
 * poi termina restituendo il valore 10
 *
 * entrambi i processi hanno un gestore di segnali per SIGUSR1
 *
 */

#define SIGNAL_HANDLER


#ifdef SIGNAL_HANDLER

int main(int argc, char *argv[]) {

	pid_t child_pid;
	int wstatus;
	int modal_result = -1;

	// questo gestore di segnali viene ereditato anche dal processo figlio
	if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
		perror("signal");
		exit(EXIT_FAILURE);
	}


	child_pid = fork();

	if (child_pid == 0) {
		// sono il processo figlio (child process)

		my_pid = getpid();

		printf("[child] sono il processo figlio, il mio PID è %u\n\n", my_pid);
		printf("[child] ora dormirò per qualche secondo\n\n");

		// faccio qualcosa... dormo per qualche secondo

		struct timespec t;

		struct timespec remaining;

		t.tv_sec = 10;  // seconds
		t.tv_nsec = 0; // nanoseconds

		while (nanosleep(&t, &remaining) == -1) {

			if (errno == EINTR) {

				printf("[child] nanosleep è stato interrotto! counter=%d\n", counter);

				printf("[child] tempo rimanente: tv_sec=%lu seconds, tv_nsec=%lu nanoseconds\n\n", remaining.tv_sec, remaining.tv_nsec);

				t = remaining;

			} else if (errno == EFAULT) {
				perror("nanosleep");
				break;
			} else if (errno == EINVAL) {
				perror("nanosleep");
				break;
			}
		}

		// https://www.tldp.org/LDP/abs/html/exitcodes.html

		int return_value = 10;

		printf("[child] sto per terminare restituendo il valore %u\n", return_value);

		// processo figlio termina
		exit(return_value);

	} else if (child_pid > 0) {
		// sono il processo padre

		my_pid = getpid();

		printf("[parent] sono il processo padre, il mio PID è %u\n\n", my_pid);

		// mando dei segnali al processo figlio

		for (int i = 0; i < 3; i++) {
			sleep(3); // dormi tre secondi

			printf("[parent] sto per mandare segnale SIGUSR1 a processo %u\n\n", child_pid);

			kill(child_pid, SIGUSR1);
			// kill(0 , SIGUSR1); // il segnale viene inviato ad ogni processo del "process group"
		}

		//kill(child_pid, SIGINT);

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

		if (modal_result != -1) {
			printf("[parent] il valore restituito dal processo figlio è %d\n", modal_result);
		}


	} else {
		perror("fork()");

		exit(1);
	}


}




#else


int main(int argc, char *argv[]) {

	pid_t child_pid;
	int wstatus;
	int modal_result = -1;


	child_pid = fork();

	if (child_pid == 0) {
		// sono il processo figlio (child process)

		my_pid = getpid();

		printf("[child] sono il processo figlio, il mio PID è %u\n", my_pid);
		printf("[child] ora dormirò per qualche secondo\n\n");

		// faccio qualcosa... dormo per qualche secondo

		struct timespec t;

		struct timespec remaining;

		t.tv_sec = 10;  // seconds
		t.tv_nsec = 0; // nanoseconds

		if (nanosleep(&t, &remaining) == -1) {
			perror("nanosleep");
		}

		printf("[child] tempo rimanente: tv_sec=%lu seconds, tv_nsec=%lu nanoseconds\n\n", remaining.tv_sec, remaining.tv_nsec);

		// https://www.tldp.org/LDP/abs/html/exitcodes.html

		int return_value = 10;

		printf("[child] sto per terminare restituendo il valore %u\n", return_value);

		// processo figlio termina
		exit(return_value);

	} else if (child_pid > 0) {
		// sono il processo padre

		my_pid = getpid();

		printf("[parent] sono il processo padre, il mio PID è %u\n\n", my_pid);

		// mando un segnale al processo figlio

		for (int i = 0; i < 3; i++) {
			sleep(2);

			printf("[parent] sto per mandare segnale SIGUSR1 a processo %u\n\n", child_pid);

			kill(child_pid, SIGUSR1);
			// kill(0 , SIGUSR1); // il segnale viene inviato ad ogni processo del "process group"
		}


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

		if (modal_result != -1) {
			printf("[parent] il valore restituito dal processo figlio è %d\n", modal_result);
		}

	} else {
		perror("fork()");

		exit(1);
	}


}




#endif


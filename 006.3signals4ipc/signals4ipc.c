#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h> // errno
#include <signal.h> // signal

/*

processo padre e processo figlio:

quando processo figlio riceve il segnale:

  SIGUSR1 -> accendere la macchina che produce "cookies"

  SIGUSR2 -> spegne la macchina che produce "cookies"

  SIGTERM -> termina il processo

processo padre riceve comandi dall'utente (+ e -) per accendere e spegnere la macchina
comando 'q' fa terminare tutto

comando 'q':
parent invia SIGUSR2 a child
parent invia SIGUSR2 a child
parent va in pause()
parent viene svegliato da SIGCHLD che arriva quando child è terminato

 */

#define OFF 0
#define ON 1

int cookie_machine_status = OFF;

int get_cookie_machine_status() {
	return cookie_machine_status;
}

void child_process_signal_handler(int signum) {
	if (signum == SIGUSR1) {
		if (cookie_machine_status == ON) {
			printf("[child] cookie_machine è già ON!!!\n");
		}

		cookie_machine_status = ON;
	} else if (signum == SIGUSR2) {
		if (cookie_machine_status == OFF) {
			printf("[child] cookie_machine è già OFF!!!\n");
		}

		cookie_machine_status = OFF;
	} else if (signum == SIGTERM) {
		printf("[child] cookie_machine deve terminare! OK!\n");
		exit(EXIT_SUCCESS);
	}

	printf("[child] set cookie_machine_status=%d\n", cookie_machine_status);
}


void child_process() {

	if (signal(SIGUSR1, child_process_signal_handler) == SIG_ERR) {
		perror("signal");
		exit(EXIT_FAILURE);
	}

	if (signal(SIGUSR2, child_process_signal_handler) == SIG_ERR) {
			perror("signal");
			exit(EXIT_FAILURE);
	}

	if (signal(SIGTERM, child_process_signal_handler) == SIG_ERR) {
			perror("signal");
			exit(EXIT_FAILURE);
	}

	printf("[child] pid=%u\n", getpid());

	while (1) {

		while (get_cookie_machine_status() == ON) {

			// printf("inizio a produrre... ");
			sleep(3);
			printf("[child]*COOKIE*\n");

		}

		printf("[child] ora chiamo pause()...\n");
		pause();
		printf("[child] uscito da pause()\n");
	}

}



void parent_process_signal_handler(int signum) {
	// riceviamo SIGCHLD: Child stopped or terminated

	pid_t child_pid;

	printf("[parent] parent_process_signal_handler\n");

	child_pid = wait(NULL);

	printf("[parent] signal handler: processo %u è terminato, ora termino anche io\n", child_pid);

	exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[]) {

	pid_t child_pid;

	if ((child_pid = fork()) == 0) {

		child_process();

	} else if (child_pid == -1) {
		perror("fork()");
		exit(EXIT_FAILURE);
	} else {

		if (signal(SIGCHLD, parent_process_signal_handler) == SIG_ERR) {
			perror("signal");
			exit(EXIT_FAILURE);
		}

		printf("[parent] pid=%u\n", getpid());

		printf("comandi a disposizione:\n");
		printf("+ : accendi cookie_machine\n");
		printf("- : spegni cookie_machine\n");
		printf("q : fine\n");

		printf("\n");

		char ch;

		while (1) {

			ch = getchar();

			switch (ch) {
				case '+':
					printf("[parent] comando '+', mando SIGUSR1 al processo figlio (child)\n");
					kill(child_pid, SIGUSR1);

					break;
				case '-':
					printf("[parent] comando '-', mando SIGUSR2 al processo figlio (child)\n");
					kill(child_pid, SIGUSR2);

					break;
				case 'q':

					printf("[parent] comando 'q'\n");

					// fermiamo cookie_machine
					printf("[parent] mando SIGUSR2 al processo figlio (child)\n");
					kill(child_pid, SIGUSR2);

					// terminiamo cookie_machine
					printf("[parent] mando SIGTERM al processo figlio (child)\n");
					kill(child_pid, SIGTERM);

					printf("[parent] chiamo pause()...\n");
					pause(); // verremo svegliati da SIGCHLD

					break;
				case '\n':
					break;
				default:
					printf("?!?\n");
					break;
			}

		}

	}


}



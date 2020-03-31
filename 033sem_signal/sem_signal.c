#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <semaphore.h>
#include <signal.h>

sem_t sem;

static void
handler(int sig)
{
	// NON è sicuro chiamare printf da un signal handler!
	// printf non è async-signal-safe (vedere Kerrisk sezione 21.1.2)
	// printf è usato qui solo a scopo diagnostico/didattico

    if (sig == SIGINT) {
    	printf("[child]ricevuto SIGINT, il processo %d rimane attivo.\n", getpid());
    } else if (sig == SIGCHLD) {
    	printf("[child]ricevuto SIGCHLD da child process terminato\n");
    }
}

void wait_on_signal(sem_t * semaphore) {

	int res;
	int sem_val;

	sem_getvalue(&sem, &sem_val);
	printf("[child]prima di sem_wait, sem_val=%d\n", sem_val);

	// il semaforo ha valore zero, quindi sem_wait bloccherà il processo.
	while ((res = sem_wait(semaphore)) == -1 && errno == EINTR) {

		sem_getvalue(&sem, &sem_val);

		printf("[child]dentro while, sem_wait ha restituito -1 e errno == EINTR, sem_val=%d\n", sem_val);

	}

	if (res == -1) {
		perror("sem_wait");
		exit(EXIT_FAILURE);
	}

	printf("[child]bye! fine child process\n");

}

// abilita/disabilita il flag SA_RESTART per il segnale indicato
int siginterrupt(int sig, int flag)
{
    int status;
    struct sigaction act;

    status = sigaction(sig, NULL, &act);
    if (status == -1)
        return -1;

    if (flag)
        act.sa_flags &= ~SA_RESTART;
    else
        act.sa_flags |= SA_RESTART;

    return sigaction(sig, &act, NULL);
}



int main(int argc, char * argv[]) {

	// esempio in cui il processo figlio intenzionalmente viene fatto bloccare su un semaforo di valore 0

	int pid;

	switch (pid = fork()) {
		case 0: // processo figlio

			if (sem_init(&sem, 1, 0) == -1) { // valore iniziale del semaforo: 0
				perror("sem_init");
				exit(EXIT_FAILURE);
			}

			// gestiamo noi questi segnali
			signal(SIGINT, handler);
			signal(SIGCHLD, handler);

			siginterrupt(SIGINT, 1); // disabilitiamo SA_RESTART per questi handler
			siginterrupt(SIGCHLD, 1);

			if (fork() == 0) { // lanciamo questo processo per provocare un segnale SIGCHLD
				sleep(2);
				exit(EXIT_SUCCESS);
			}

			// in alternativa, potrei usare pause() al posto di wait_on_signal()

			wait_on_signal(&sem);

			// non passiamo di qua, il processo rimane bloccato sul semaforo

			sem_close(&sem);

			exit(EXIT_SUCCESS);

			break;
		default:
			break;
	}


	printf("premi enter per mandare il segnale SIGINT al processo %d,'q' per uscire\n", pid);

	int ch = 0;

	while ((ch = getchar()) != 'q') {

		printf("mando segnale 2 (SIGINT) al processo %d\n", pid);

		if (kill(pid, 2) == -1) {
			perror("kill");
		}

	}

	// terminiamo il processo figlio, altrimenti ci blocchiamo anche noi
	kill(pid, 9);

	// ci bloccheremo ad aspettare il child process bloccato sul semaforo...
	wait(NULL);


}

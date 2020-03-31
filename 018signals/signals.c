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
#include <pthread.h>
#include <signal.h>


static void sigHandler(int sig) {
	// NON è sicuro chiamare printf da un signal handler!
	// printf non è async-signal-safe (vedere Kerrisk sezione 21.1.2)
	// printf è usato qui solo a scopo diagnostico/didattico
	printf("ricevuto segnale %d\n", sig);

//	printf("ricevuto segnale %d, ora termino. bye!\n", sig);
//
//	exit(EXIT_SUCCESS);

}

void test_sleep_signal() {
	// verifica che sleep può essere interrotto da un segnale

	printf("sto per invocare sleep(10), digitare Ctrl-C per interrompere sleep\n");

	sleep(10);

	printf("fine sleep(10)\n");

}


int main(int argc, char * argv[]) {

	if (signal(SIGINT, sigHandler) == SIG_ERR) {
		perror("signal");
	}

	test_sleep_signal();

	int pid = getpid();

	printf("prova uno dei seguenti comandi da shell:\nkill %d\nkill -9 %d\nkill -2 %d\n", pid, pid, pid);

	getchar();


	return 0;
}


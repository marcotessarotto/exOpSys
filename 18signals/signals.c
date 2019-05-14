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

	printf("ricevuto segnale\n");

	exit(EXIT_SUCCESS);

}



int main(int argc, char * argv[]) {

	if (signal(SIGINT, sigHandler) == SIG_ERR) {
		perror("signal");
	}

	int pid = getpid();

	printf("prova uno dei seguenti comandi da shell:\nkill %d\nkill -9 %d\nkill -2 %d\n", pid, pid, pid);

	getchar();


	return 0;
}


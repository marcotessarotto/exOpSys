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
#include <time.h>


/*
 * "volatile" NON va utilizza per multithreading/multiprocessing, è stata introdotta per altri utilizzi.
 *
 * esempio:
 *
 * volatile int condition;
 *
 * da ISO/IEC 9899:2018
	A volatile declaration can be used to describe an object corresponding to a memory-mapped input/output port
	or an object accessed by an asynchronously interrupting function.
	Actions on objects so declared are not allowed to be "optimized out" by an implementation
	or reordered except as permitted by the rules for evaluating expressions.
 */

// condition è una variabile condivisa tra thread diversi, pertanto va utilizzando un mutex per accedervi
int condition = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex statico inizializzato



void * mythread(void * mythread) {

	printf("[thread]prima di sleep(2)\n");
	sleep(2);

	pthread_mutex_lock(&mutex);

	condition = 1; // sezione critica

	pthread_mutex_unlock(&mutex);

	printf("[thread]bye\n");

	return NULL;
}



int main(int argc, char * argv[]) {

	pthread_t t1;
	int loc;
	int dowhile_counter = 0;

	printf("esempio in cui il thread principale è impegnato (ed impegna la risorsa CPU)\nad aspettare che una variabile condivisa cambi valore (viene modificata in un altro thread).\n"
			"Utilizza un semaforo.\n");


	if (pthread_create(&t1, NULL, mythread, NULL) != 0) {
		perror("pthread_create");

		exit(EXIT_FAILURE);
	}


	struct timespec start, stop;

	// http://man7.org/linux/man-pages/man2/clock_gettime.2.html
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);


	printf("[main]prima del do-while\n");

	do {

		pthread_mutex_lock(&mutex);

		loc = condition; // sezione critica

		pthread_mutex_unlock(&mutex);

		dowhile_counter++;

	} while (loc == 0);

	printf("[main]dopo il do-while\n");

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);

	double result = (stop.tv_sec - start.tv_sec) * 1e6 + (stop.tv_nsec - start.tv_nsec) / 1e3;    // in microseconds

	printf("[main]tempo di CPU consumato (microsecondi): %lf, numero cicli do-while: %d\n", result, dowhile_counter);

	if (pthread_join(t1, NULL) != 0) {
		perror("pthread_join");
	}


	printf("[main]bye\n");

	return 0;
}

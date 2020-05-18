#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <errno.h>
#include <pthread.h>


/*
 * esempio di accessi concorrenti ad una variabile condivisa
 * gestiti attraverso mutex (mutual exclusion)
 *
 * http://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf
 * 1.5.2 Concurrent updates pag. 5
 * 3.4 Mutex pag. 16
 *
 * due threads operano in modo concorrente sulla stessa variabile condivisa,
 * un thread la incrementa NUMBER_OF_CYCLES volte
 * e l'altro thread la decrementa NUMBER_OF_CYCLES volte.
 *
 * lo schema di mutua esclusione (mutex) per gestire l'accesso concorrente alla variabile condivisa
 * è realizzato con pthread_mutex_t.
 *
 * il mutex garantisce che soltanto un thread alla volta acceda alla variabile condivisa.
 *
 *
 * vedere anche l'esempio 007.07mmap-sem e 008.01thread-sem
 */

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int shared_counter; // variabile condivisa tra i due thread

// esercizio:
// nei due thread t1 e t2, tenere traccia del valore max e valore minimo assunto da shared_counter
// e scriverlo prima di terminare

#define NUMBER_OF_CYCLES 10000000

void * thread_function_1(void * arg) {
	int s;

	for (int i = 0; i < NUMBER_OF_CYCLES; i++) {

		// 3.4.2 Mutual exclusion solution, pag. 19
		s = pthread_mutex_lock(&mutex);
		if (s != 0) {
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}

		shared_counter++;

		s = pthread_mutex_unlock(&mutex);
		if (s != 0) {
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}

	}

	return NULL;
}


void * thread_function_2(void * arg) {
	int s;

	for (int i = 0; i < NUMBER_OF_CYCLES; i++) {

		// 3.4.2 Mutual exclusion solution, pag. 19
		s = pthread_mutex_lock(&mutex);
		if (s != 0) {
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}

		shared_counter--;

		s = pthread_mutex_unlock(&mutex);
		if (s != 0) {
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}

	}

	return NULL;
}


int main(int argc, char * argv[]) {

	pthread_t t1;
	pthread_t t2;
	void * res;

	int s;

	printf("initial value of shared_counter=%d, NUMBER_OF_CYCLES=%d\n", shared_counter, NUMBER_OF_CYCLES);

	s = pthread_create(&t1, NULL, thread_function_1, NULL);

	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	s = pthread_create(&t2, NULL, thread_function_2, NULL);

	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}


	s = pthread_join(t1, &res);

	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}

	s = pthread_join(t2, &res);

	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}

	printf("final value of shared_counter=%d\n", shared_counter);

	printf("bye\n");

	return 0;
}


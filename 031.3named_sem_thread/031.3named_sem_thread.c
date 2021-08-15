#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <sys/mman.h>
#include <pthread.h>

/*
 * i threads possono usare un semaforo con nome, se ciascun thread lo apre per conto suo
 *
 */


char * semaphore_name = "/il_mio_semaforo_031.3";

int shared_counter;

#define N 200000000

#define THREADS 2


void * do_operation(void * arg) {

	sem_t * semaphore;

	semaphore = sem_open(semaphore_name, 0);
	if (semaphore == SEM_FAILED) {
		perror("sem_open");
		exit(1);
	}

	for (int i = 0; i < N; i++) {

		if (sem_wait(semaphore) != 0) {
			perror("sem_wait");
			exit(1);
		}

		shared_counter++;

		if (sem_post(semaphore) != 0) {
			perror("sem_post");
			exit(1);
		}

	}

	return NULL;
}



int main() {

	struct timespec ts1, ts2;

	sem_t * semaphore;

	if (sem_unlink(semaphore_name) == -1) {
		perror("sem_unlink");
	}

	// semaforo usato come mutex
	semaphore = sem_open(semaphore_name, O_CREAT, 0600, 1);
	if (semaphore == SEM_FAILED) {
		perror("sem_open");
		exit(1);
	}

	sem_close(semaphore);

	pthread_t t[THREADS];

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts1);

	for (int i = 0; i < THREADS; i++) {
		if (pthread_create(&t[i], NULL, do_operation, NULL) != 0) {
			perror("pthread_create");
			exit(1);
		}
	}

	for (int i = 0; i < THREADS; i++) {
		if (pthread_join(t[i], NULL) != 0) {
			perror("pthread_join");
		}
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts2);

	printf("shared_counter = %d\n", shared_counter);
	printf("risultato corretto? %s\n", shared_counter == N * THREADS ? "OK" : "NO");

	unsigned long dt = ((ts2.tv_sec - ts1.tv_sec) * 1000000000L + ts2.tv_nsec) - ts1.tv_nsec;

	printf("dt = %lu ns, %.3lf s\n", dt, dt / 1000000000.);


	return 0;
}

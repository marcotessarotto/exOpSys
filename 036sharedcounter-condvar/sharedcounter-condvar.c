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


// condition è una variabile condivisa tra thread diversi, pertanto va utilizzando un mutex per accedervi
int condition = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex statico, inizializzato
pthread_cond_t condvar = PTHREAD_COND_INITIALIZER; // condition variable statica, inizializzata


void * mythread(void * mythread) {

	printf("[thread]prima di sleep(2)\n");
	sleep(2);

	pthread_mutex_lock(&mutex);

	condition = 1; // sezione critica

	printf("[thread]prima di pthread_cond_signal\n");

	pthread_cond_signal(&condvar); // 'svegliamo' un thread eventualmente in attesa sulla condition variable
	// se l'altro thread non è ancora entrato in wait sulla condition variable, la 'sveglia' inviata attraverso pthread_cond_signal è persa

	pthread_mutex_unlock(&mutex);

	printf("[thread]bye\n");

	return NULL;
}



int main(int argc, char * argv[]) {

	pthread_t t1;

	printf("esempio in cui il thread principale aspetta\n(senza impegnare la CPU in un loop, grazie all'uso di una 'condition variable')\nche una variabile condivisa cambi valore (viene modificata in un altro thread).\n"
			"Utilizza un semaforo ed una 'condition variable'.\n");


	if (pthread_create(&t1, NULL, mythread, NULL) != 0) {
		perror("pthread_create");

		exit(EXIT_FAILURE);
	}


	struct timespec start, stop;

	// http://man7.org/linux/man-pages/man2/clock_gettime.2.html
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);


	printf("[main]prima del pthread_cond_wait\n");

	// cosa succede se entriamo in pthread_cond_wait dopo che pthread_cond_signal è stata chiamata nell'altro thread?
	// se abilitamo la riga successiva, cosa succede?
	// sleep(3);

	pthread_mutex_lock(&mutex);

	while (condition == 0) {
		pthread_cond_wait(&condvar, &mutex); // la chiamata a pthread_cond_wait richiede di avere già il lock sul mutex
		printf("[main]dopo pthread_cond_wait\n");
	}

	pthread_mutex_unlock(&mutex);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);

	double result = (stop.tv_sec - start.tv_sec) * 1e6 + (stop.tv_nsec - start.tv_nsec) / 1e3;    // in microseconds

	printf("[main]tempo di CPU consumato (microsecondi): %lf\n", result);

	if (pthread_join(t1, NULL) != 0) {
		perror("pthread_join");
	}


	printf("[main]bye\n");

	return 0;
}

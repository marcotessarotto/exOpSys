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

#define NUM_CICLI 10000000

int global_counter = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void * thread_function(void * arg) {

	int loc;

	for (int i = 0; i < NUM_CICLI; i++) {

//		loc = global_counter;
//		loc++;
//		global_counter = loc;

		// acquisizione mutex
		pthread_mutex_lock(&mutex);

		global_counter++; // sezione critica

		// rilascio mutex
		pthread_mutex_unlock(&mutex);

	}

	return NULL;

}



int main(int argc, char * argv[]) {

	pthread_t t1, t2;
	int s;


	s = pthread_create(&t1, NULL, thread_function, NULL);
	if (s != 0) {
		perror("pthread_create");
	}

	s = pthread_create(&t2, NULL, thread_function, NULL);
	if (s != 0) {
		perror("pthread_create");
	}

	s = pthread_join(t1, NULL);
	if (s != 0) {
		perror("pthread_join");
	}

	s = pthread_join(t2, NULL);
	if (s != 0) {
		perror("pthread_join");
	}

	printf("global_counter = %d, valore atteso = %d\n", global_counter, NUM_CICLI * 2);

	return 0;
}

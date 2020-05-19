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

// barriera per N threads realizzata con pthread_barrier_t

// https://linux.die.net/man/3/pthread_barrier_init
// https://linux.die.net/man/3/pthread_barrier_wait


#define N 10

pthread_barrier_t thread_barrier;

int number_of_threads = N;

void * thread_function(void * arg) {
	int s;

	printf("rendezvous\n");

	// https://linux.die.net/man/3/pthread_barrier_wait
	s = pthread_barrier_wait(&thread_barrier);

	/*
	 The pthread_barrier_wait() function shall synchronize participating threads at
	 the barrier referenced by barrier. The calling thread shall block until
	 the required number of threads have called pthread_barrier_wait() specifying the barrier.
	*/

	printf("critical point\n");

	return NULL;
}




#define CHECK_ERR(a,msg) {if ((a) == -1) { perror((msg)); exit(EXIT_FAILURE); } }

int main() {

	int s;
	pthread_t threads[N];

	// https://linux.die.net/man/3/pthread_barrier_init
	s = pthread_barrier_init(&thread_barrier, NULL, N);
	CHECK_ERR(s,"pthread_barrier_init")

	for (int i=0; i < number_of_threads; i++) {
		s = pthread_create(&threads[i], NULL, thread_function, NULL);

		if (s != 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	for (int i=0; i < number_of_threads; i++) {
		s = pthread_join(threads[i], NULL);

		if (s != 0) {
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}

	}

	// https://linux.die.net/man/3/pthread_barrier_init
	s = pthread_barrier_destroy(&thread_barrier);
	CHECK_ERR(s,"pthread_barrier_destroy")

	printf("bye\n");

	return 0;
}

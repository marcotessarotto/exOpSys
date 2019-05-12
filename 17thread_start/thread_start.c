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


void * threadFunc(void * arg) {

	char * str = (char *) arg;

	printf("[thread] parametro: %s\n", str);

	return (void *) strlen(str);
}


int main(int argc, char * argv[]) {

	pthread_t t1;
	void * res;

	int s;

	s = pthread_create(&t1, NULL, threadFunc, "ciao\n");

	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	printf("ho creato un thread\n");

	s = pthread_join(t1, &res);

	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}

	printf("valore restituito dal thread: %ld\n", (long) res);

	exit(EXIT_SUCCESS);

}

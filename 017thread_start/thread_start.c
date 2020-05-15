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

int special_return_value = 123;

int * prepare_return_value(int value) {

	int * result_value_ptr;

	result_value_ptr = malloc(sizeof(int));

	if (result_value_ptr == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	*result_value_ptr = value;

	return result_value_ptr;
}

void * thread_function(void * arg) {

	char * str = (char *) arg;

	printf("[thread] parametro: %s\n", str);

	//// ATTENZIONE!!! fuori standard!!! funziona su Linux
	pthread_t tid = pthread_self();
	printf("thread ID: %lu\n", tid);
	////

	printf("premi x per exit, t per pthread_exit, qualsiasi altro per continuare\n");

	int ch = getchar();

	switch (ch) {
	case 'x':
		printf("[thread] prima di exit()\n");

		exit(EXIT_SUCCESS);
		break;
	case 't':
		printf("[thread] prima di pthread_exit\n");

		pthread_exit((void *) prepare_return_value(123));
		break;
	default:
		break;
	}

	return (void *) prepare_return_value(strlen(str));
}


int main(int argc, char * argv[]) {

	pthread_t t1;
	void * res;
	int * ret_value;

	int s;

	//// ATTENZIONE!!! fuori standard!!! funziona su Linux
	pthread_t tid = pthread_self();
	printf("thread ID: %lu\n", tid);
	////

	s = pthread_create(&t1, NULL, thread_function, "ciao\n");

	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	printf("ho creato un thread, prima di pthread_join\n");

	s = pthread_join(t1, &res);

	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}

	ret_value = (int *) res;

	printf("valore restituito dal thread: %d\n",  *ret_value);

	free(res); // superfluo perchè il programma subito dopo termina

	exit(EXIT_SUCCESS);

}

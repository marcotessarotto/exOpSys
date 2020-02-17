#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <semaphore.h>

#include <errno.h>

const char * SEM_NAME = "/my_semaphore2";

void consumer() {
	sem_t * semaphore;
	printf("CCC starting consumer\n");

	semaphore = sem_open(SEM_NAME, 0);

	while (-1) {

		printf("CCC waiting for product...\n");

		sem_wait(semaphore);

		printf("CCC consumed product!\n");

	}

}


void producer() {
	sem_t * semaphore;
	printf("PPP starting producer\n");

	semaphore = sem_open(SEM_NAME, 0);

	while (-1) {

		sleep(1);

//		time_t now = time(NULL);

		printf("PPP new product\n");

		int res = sem_post(semaphore);

		if (res != 0) {
			printf("PPP error in sem_post!\n");
		} else {
			int current_sem_value;
			sem_getvalue(semaphore, &current_sem_value);

			printf("PPP semaphore current value: %d\n", current_sem_value);
		}

	}
}



int main(int argc, char *argv[]) {

	sem_t * semaphore;

	semaphore = sem_open(SEM_NAME, O_CREAT, S_IRWXU, 0); //

	if (semaphore == SEM_FAILED) {
		printf("SEM_FAILED");

		return -1;
	}

	int current_sem_value;
	sem_getvalue(semaphore, &current_sem_value);

	sem_close(semaphore);

	printf("MAIN semaphore current value: %d\n", current_sem_value);

	pid_t producer_pid = fork();

	pid_t consumer_pid;

	if (producer_pid > 0) {
		consumer_pid = fork();

		if (consumer_pid == 0)
			consumer();
	} else {
		producer();
	}



	return 0;
}

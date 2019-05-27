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

/*
ho tre processi A, B e C; simile all'esercizio precedente; i tre processi
condividono un file aperto.
A scrive sul file; sleep 1 secondo; avvisa B di procedere
B scrive sul file; sleep 1 secondo; avvisa C di procedere
C scrive sul file; sleep 1 secondo; "tira il dado" per decidere se avvisare
il processo A o il processo B di procedere.
 */


int fd;
const char * file_name = "/tmp/esercizio4.log";


void * create_anon_mmap(size_t size) {

	// MAP_SHARED: condivisibile tra processi
	// PROT_READ | PROT_WRITE: posso leggere e scrivere nello spazio di memoria
	void * memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	if (memory == MAP_FAILED) {
		perror("mmap");

		return NULL;
	}

	return memory;

}


#define TURNO_DI_A 0
#define TURNO_DI_B 1

sem_t * semA;
sem_t * semB;
sem_t * semC;

#define NUM_CICLI 10

int child_process_terminate = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void child_signal_handler(int sig) {
	pthread_mutex_lock(&mutex);
	child_process_terminate = 1;
	pthread_mutex_unlock(&mutex);
}

void init() {

	///
	semA = create_anon_mmap(sizeof(sem_t));
	semB = create_anon_mmap(sizeof(sem_t));
	semC = create_anon_mmap(sizeof(sem_t));

	if (sem_init(semA, 1, 1) == -1) {
		perror("sem_init");
		exit(EXIT_FAILURE);
	}

	if (sem_init(semB, 1, 0) == -1) {
		perror("sem_init");
		exit(EXIT_FAILURE);
	}

	if (sem_init(semC, 1, 0) == -1) {
			perror("sem_init");
			exit(EXIT_FAILURE);
		}

}

void cleanup() {

	if (close(fd) == -1) {
		perror("close");
	}

	if (sem_destroy(semA) == -1) {
		perror("sem_destroy semA");
	}

	if (sem_destroy(semB) == -1) {
		perror("sem_destroy semB");
	}

	if (munmap(semA, sizeof(sem_t)) == -1) {
		perror("munmap1");
	}

	if (munmap(semB, sizeof(sem_t)) == -1) {
		perror("munmap1");
	}

}

int get_child_process_terminate() {

	int retval;

	pthread_mutex_lock(&mutex);
	retval = child_process_terminate;
	pthread_mutex_unlock(&mutex);

	return retval;
}

int main(int argc, char * argv[]) {

	printf("starting...\n");
	printf("lanciare in una shell il comando:\ntail -f /tmp/esercizio4.log\n");

	// se il file esiste già, il file viene 'troncato' (precedenti contenuti sono eliminati e size diventa zero)
	fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 00600);

	if (fd == -1) {
		perror("create_file error\n");
		return 0;
	}

	init();


	// comincia il processo A (processo padre)

	int pid1, pid2;
	char buffer[1024];
	int counter = 0;


	if (signal(SIGUSR1, child_signal_handler) == SIG_ERR) {
		perror("signal");
	}

	switch( pid1 = fork() ) {
	case 0:

		while (get_child_process_terminate() == 0) {

			if (sem_wait(semB) == -1) {
				perror("sem_wait");
				exit(EXIT_FAILURE);
			}

			printf("[procB]scrivo su file la linea %d\n", counter);

			sprintf(buffer,"processo figlio1, linea %d\n", counter++);
			write(fd,buffer,strlen(buffer));

			sleep(1);

			if (sem_post(semC) == -1) {
				perror("sem_post");
				exit(EXIT_FAILURE);
			}

		} // while

		printf("[child1] exit for\n");

		cleanup();

		exit(EXIT_SUCCESS);

		break;
	case -1:
		perror("fork");
		exit(EXIT_FAILURE);
	default:
		;
	}


	switch( pid2 = fork() ) {
	case 0:

		while (get_child_process_terminate() == 0) {

			if (sem_wait(semC) == -1) {
				perror("sem_wait");
				exit(EXIT_FAILURE);
			}

			printf("[procC]scrivo su file la linea %d\n", counter);

			sprintf(buffer,"processo figlio2, linea %d\n", counter++);
			write(fd,buffer,strlen(buffer));

			sleep(1);

			sem_t * next_sem;

//			if (random() % 2 == 0)
//				next_sem = semA;
//			else
//				next_sem = semB;

			next_sem = random() % 2 == 0 ? semA : semB;

			if (sem_post(next_sem) == -1) {
				perror("sem_post");
				exit(EXIT_FAILURE);
			}

		} // while

		printf("[child2] exit for\n");

		cleanup();

		exit(EXIT_SUCCESS);

		break;
	case -1:
		perror("fork");
		exit(EXIT_FAILURE);
	default:
		;
	}



	for (int i = 0; i < NUM_CICLI; i++) {

		if (sem_wait(semA) == -1) {
			perror("sem_wait");
			exit(EXIT_FAILURE);
		}

		printf("[procA]scrivo su file la linea %d\n", i);

		sprintf(buffer,"processo padre, linea %d\n", i);
		write(fd,buffer,strlen(buffer));

		sleep(1);

		if (sem_post(semB) == -1) {
			perror("sem_post");
			exit(EXIT_FAILURE);
		}


	} // for



	// quando esco, semA vale 0
	if (sem_post(semA) == -1) {
		perror("sem_post semA finale");
		exit(EXIT_FAILURE);
	}


	if (kill(pid1, SIGUSR1) == -1) { // oppure SIGTERM
		perror("kill");
	}

	if (kill(pid2, SIGUSR1) == -1) { // oppure SIGTERM
		perror("kill");
	}


	if (wait(NULL) == -1) {
		perror("wait");
	}

	if (wait(NULL) == -1) {
		perror("wait");
	}


	cleanup();

	printf("finito, bye!\n");

	return 0;
}

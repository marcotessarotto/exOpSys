#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <sys/mman.h>

/**
 * il processo padre:
 * - crea un named semaphore (per usarlo come mutex) e lo condivide con tutti i processi figli
 * - crea una memory map condivisa per usarlo come contatore condiviso
 * - ogni processo figlio incrementa N volte il contatore condiviso
 * - il processo padre crea 10 processi figli
 * - il risultato finale atteso è (N * PROCESSES)
 *
 *
 */


char * semaphore_name = "/il_mio_semaforo_031.1";

sem_t * semaphore;

char * shared_memory_map;

unsigned long * shared_counter;

#define MMAP_SIZE sizeof(unsigned long)

#define N 1000000

#define PROCESSES 10


void do_operation() {

	for (int i = 0; i < N; i++) {

		if (sem_wait(semaphore) != 0) {
			perror("sem_wait");
			exit(1);
		}

		(*shared_counter)++;

		if (sem_post(semaphore) != 0) {
			perror("sem_post");
			exit(1);
		}


	}
}

int main() {

	if (sem_unlink(semaphore_name) == -1) {
		perror("sem_unlink");
	}

	// semaforo usato come mutex
	semaphore = sem_open(semaphore_name, O_CREAT, 0600, 1);

	if (semaphore == SEM_FAILED) {
		perror("sem_open");
		exit(1);
	}

	printf("[parent] ho creato il named semaphore : %s\n", semaphore_name);

	// chiedo una memoria condivisa per il contatore condiviso tra i processi
	shared_memory_map = mmap(NULL, MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1,0 );
	if (shared_memory_map == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}

	shared_counter = (unsigned long *) shared_memory_map;

	for (int i = 0; i < PROCESSES; i++) {
		switch (fork()) {
		case -1:
			perror("fork");
			exit(1);
		case 0: // processo figlio i-mo

			printf("[child %d]start\n", i);

			do_operation();

			printf("[child %d]end\n", i);

			exit(0);
		default:
			// processo padre
			;

		}

	}


//	printf("[parent]start work\n");
//
//	do_operation();
//
//	printf("[parent]end work\n");

	// processo padre aspetta la conclusione del processo figlio
	while (wait(NULL) != -1) ;

	printf("shared_counter = %lu\n", *shared_counter);
	printf("valore atteso     =%d\n", N*PROCESSES);

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <sys/mman.h>

/*
 * il processo padre:
 * - crea un unnamed semaphore (per usarlo come mutex) e lo condivide con tutti i processi figli
 * - crea una memory map condivisa per usarlo come contatore condiviso
 * - ogni processo figlio incrementa N volte il contatore condiviso
 * - il processo padre crea 10 processi figli
 * - il risultato finale atteso è (N * PROCESSES)
 *
 *
 */



sem_t * semaphore;

char * shared_memory_map;

unsigned long * shared_counter;

#define MMAP_SIZE (sizeof(unsigned long) + sizeof(sem_t))

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


	// chiedo una memoria condivisa per il contatore condiviso tra i processi e per il semaforo
	shared_memory_map = mmap(NULL, MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1,0 );
	if (shared_memory_map == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}

	shared_counter = (unsigned long *) shared_memory_map;

	semaphore = (sem_t *) (shared_memory_map + sizeof(unsigned long));
	// equivalente a:
	//semaphore = (sem_t *) &shared_memory_map[sizeof(unsigned long)];

	// semaforo usato come mutex, valore iniziale = 1
	if (sem_init(semaphore, 1, 1) == -1) {
		perror("sem_init");
		exit(1);
	}

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

	// il processo padre aspetta la conclusione di tutti i processi figli
	while (wait(NULL) != -1) ;

	printf("shared_counter = %lu\n", *shared_counter);
	printf("valore atteso     = %d\n", N*PROCESSES);

	return 0;
}

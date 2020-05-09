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
#include <semaphore.h>

/*
 * esempio di accessi concorrenti ad una variabile condivisa
 * gestiti attraverso mutex (mutual exclusion)
 *
 * http://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf
 * 1.5.2 Concurrent updates pag. 5
 * 3.4 Mutex pag. 16
 *
 * due processi operano in modo concorrente sulla stessa variabile condivisa,
 * un processo la incrementa NUMBER_OF_CYCLES volte
 * e l'altro processo la decrementa NUMBER_OF_CYCLES volte.
 *
 * lo schema di mutua esclusione (mutex) per gestire l'accesso concorrente alla variabile condivisa
 * è realizzato con semaforo POSIX senza nome.
 *
 * il mutex garantisce che soltanto un processo alla volta accede alla variabile condivisa.
 *
 * il semaforo si trova in una memory map condivisa tra i due processi.
 *
 */


sem_t * process_semaphore;

int * shared_counter;


#define CHECK_ERR(a,msg) {if ((a) == -1) { perror((msg)); exit(EXIT_FAILURE); } }

#define CHECK_ERR_MMAP(a,msg) {if ((a) == MAP_FAILED) { perror((msg)); exit(EXIT_FAILURE); } }

#define NUMBER_OF_CYCLES 1000000

int main(int argc, char * argv[]) {

	int res;
	pid_t pid;

	process_semaphore = mmap(NULL, // NULL: è il kernel a scegliere l'indirizzo
			sizeof(sem_t) + sizeof(int), // dimensione della memory map
			PROT_READ | PROT_WRITE, // memory map leggibile e scrivibile
			MAP_SHARED | MAP_ANONYMOUS, // memory map condivisibile con altri processi e senza file di appoggio
			-1,
			0); // offset nel file

	CHECK_ERR_MMAP(process_semaphore,"mmap")

	shared_counter = (int *) (process_semaphore + 1);

	printf("shared_counter initial value: %d\n", *shared_counter);

	res = sem_init(process_semaphore,
					1, // 1 => il semaforo è condiviso tra processi, 0 => il semaforo è condiviso tra threads del processo
					1 // valore iniziale del semaforo (se mettiamo 0 che succede?)
				  );

	CHECK_ERR(res,"sem_init")

	pid = fork();
	CHECK_ERR(pid,"fork")

	if (pid == 0) {
		for (int i = 0; i < NUMBER_OF_CYCLES; i++) {

			// 3.4.2 Mutual exclusion solution, pag. 19
			if (sem_wait(process_semaphore) == -1) {
				perror("sem_wait");
				exit(EXIT_FAILURE);
			}

			(*shared_counter)++;

			if (sem_post(process_semaphore) == -1) {
				perror("sem_post");
				exit(EXIT_FAILURE);
			}

		}

		exit(EXIT_SUCCESS);
	} else {
		for (int i = 0; i < NUMBER_OF_CYCLES; i++) {

			if (sem_wait(process_semaphore) == -1) {
				perror("sem_wait");
				exit(EXIT_FAILURE);
			}

			(*shared_counter)--;

			if (sem_post(process_semaphore) == -1) {
				perror("sem_post");
				exit(EXIT_FAILURE);
			}

		}
	}

	wait(NULL);

	printf("shared_counter final value: %d\n", *shared_counter);

	// il semaforo senza nome va distrutto solo quando non ci sono processi bloccati su di esso
	res = sem_destroy(process_semaphore);

	CHECK_ERR(res,"sem_destroy")

	printf("bye\n");

	return 0;
}

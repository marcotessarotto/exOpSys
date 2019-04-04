#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>

#include "files_common.h"


#define CONTINUE 0
#define FINISHED 1

#define USE_SLEEP 0
#define USE_USLEEP1 0
#define USE_USLEEP2 0

int launch_program(char program_to_launch[]) {

	int child_pid;
	char buf[5];

	/*

       fork() creates a new process by duplicating the calling process.  The new process is referred to as the child process.  The calling process is referred to as the parent process.

       The  child process and the parent process run in separate memory spaces.  At the time of fork() both memory spaces have the same content.  Memory writes, file mappings (mmap(2)),
       and unmappings (munmap(2)) performed by one of the processes do not affect the other.

       The child process is an exact duplicate of the parent process except for the following points:

       *  The child has its own unique process ID, and this PID does not match the ID of any existing process group (setpgid(2)) or session.

...

       *  The  child process is created with a single thread—the one that called fork().  The entire virtual address space of the parent is replicated in the child, including the states
          of mutexes, condition variables, and other pthreads objects; the use of pthread_atfork(3) may be helpful for dealing with problems that this can cause.

       *  After a fork() in a multithreaded program, the child can safely call only async-signal-safe functions (see signal-safety(7)) until such time as it calls execve(2).

       *  The child inherits copies of the parent's set of open file descriptors.  Each file descriptor in the child refers to the same open file description (see open(2)) as the corre‐
          sponding file descriptor in the parent.  This means that the two file descriptors share open file status flags, file offset, and signal-driven I/O attributes (see the descrip‐
          tion of F_SETOWN and F_SETSIG in fcntl(2)).


	 */

	sem_t * shared_memory_semaphore = create_anon_mmap(sizeof(sem_t));
	char * shared_memory = create_anon_mmap(1024);


	printf("sizeof(sem_t) = %ld\n", sizeof(sem_t));

	if (shared_memory_semaphore == NULL || shared_memory == NULL) {
		perror("non abbiamo la shared memory! fine!");
		exit(EXIT_FAILURE);
	}

	if (sem_init(shared_memory_semaphore, 1, 1) == -1) {
		perror("errore in sem_init");
		exit(EXIT_FAILURE);
	}

	strcpy(shared_memory, "messaggio per child process!");

	// usiamo l'ultima posizione della shared memory per comunicare al child process che abbiamo finito il lavoro
	shared_memory[1023] = CONTINUE;

	// contatore dei "produtti"
	shared_memory[1022] = 0;



	printf("[parent]sto per fare fork\n");

	if ((child_pid = fork()) != 0) { // parent

		if (USE_USLEEP1) {
			printf("[parent]sto per dormire per n microsecondi...\n");
			usleep(100);
		}

		printf("[parent]avviato child process: il suo PID é %d\n", child_pid);

		printf("[parent]ora 'lavorerò' per 5 cicli...\n");

		for (int i = 1; i <= 5; i++) {

			if (USE_SLEEP)
				sleep(1);

			if (USE_USLEEP2)
				usleep(100); // dormo n microsecondi

			// occupiamo il semaforo come processo parent
			printf("\n[parent]provo ad occupare il semaforo...\n");
			sem_wait(shared_memory_semaphore);
			printf("\n[parent]ho occupato il semaforo!\n");

			shared_memory[1022]++; // contatore dei "prodotti"

			// facciamo finta che stiamo lavorando sulla shared memory...
			if (USE_SLEEP)
				sleep(1);

			sprintf(buf, "%d...", i);
			strcat(shared_memory, buf);

			printf("[parent]ciclo %d, nuovo contenuto shared memory:[%s]\n", i, shared_memory);

			printf("[parent]libero il semaforo\n");
			sem_post(shared_memory_semaphore);
		}

		printf("\n[parent]fine ciclo sleep\n");

		printf("[parent]provo ad occupare il semaforo....\n");
		sem_wait(shared_memory_semaphore);

		printf("[parent]comunico al child process di avere concluso\n");
		shared_memory[1023] = FINISHED;

		printf("\n[parent]libero il semaforo\n");
		sem_post(shared_memory_semaphore);

		munmap(shared_memory_semaphore, sizeof(sem_t));
		munmap(shared_memory, 1024);

		return -1;
	}

	// se sono qui, sono il processo figlio

	printf("\n[child]sono il processo figlio, il mio PID è:%d\n", getpid());

	int finished = 0;
	int contatore_produttore = 0;

	while (!finished) {
		// aspettiamo il via libera dal semaforo...
		printf("[child]sto per eseguire wait sul semaforo...\n");
		sem_wait(shared_memory_semaphore);

		printf("[child]semaforo libero!\n");

		if (shared_memory[1022] != contatore_produttore) {

			printf("[child]contenuto della shared memory condivisa con parent: [%s]\n", shared_memory);

			contatore_produttore = shared_memory[1022];
		} else {
			printf("[child]niente di nuovo dal processo parent\n");
		}

		finished = shared_memory[1023];
		if (finished)
			printf("[child]il processo parent mi ha comunicato di avere finito; allora finisco anche io!\n");

		printf("[child]liberiamo il semaforo\n");
		sem_post(shared_memory_semaphore);

		if (USE_SLEEP) {
			printf("[child]non ho nulla da fare, dormo un po'....\n");
			sleep(1);
		}

//		printf("[child]modifico la shared memory con un messaggio per il parent process\n");
//		strcpy(shared_memory,"ok ricevuto! messaggio da child process");

	}


	munmap(shared_memory_semaphore, sizeof(sem_t));
	munmap(shared_memory, 1024);


	printf("[child]ora avvierò un altro programma\n");

	char *newargv[] = { NULL, "hello", "world", NULL };
	char *newenviron[] = { NULL };

	newargv[0] = program_to_launch;

	execve(program_to_launch, newargv, newenviron);

	// qua non ci passiamo, a meno di errori
    printf("[child]c'è stato un problema con execve!\n");
	perror("[child]execve");   /* execve() returns only on error */

	exit(EXIT_FAILURE);
}


int main(int argc, char *argv[]) {

   printf("esempio 01files\n\n");

   test_mmap();


   launch_program("../01printargs/Debug/01printargs");

   printf("[parent]fine\n\n");

   return EXIT_SUCCESS;
}

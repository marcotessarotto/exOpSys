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

const char * semaphore_name = "/13fork_semaphore";

int * shared_counter;

sem_t * semaphore;

//#define DEBUG

// provare a commentare questo define e vedere che valori si ottengono per il contatore
#define USE_SEMAPHORE

sem_t * create_named_semaphore() {
	sem_t * result;

	// se il named semaphore esiste, lo cancelliamo
	sem_unlink(semaphore_name);

	// valore iniziale del semaforo: 1 (provare a mettere valore iniziale 0 e vedere cosa succede)
	if ((result = sem_open(semaphore_name, O_CREAT, 0600, 1)) == SEM_FAILED) {
		perror("sem_open");
		return NULL;
	}

/*
 * ls /dev/shm/ -l
 * -rw------- 1 utente   utente     32 Apr 30 23:20 sem.13fork_semaphore
 *
 */

	printf("sem_open ok - nome semaforo: %s (vedere files in /dev/shm/) \n", semaphore_name);

	return result;
}


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

void add_counter(int val, char * prefix) {

#ifdef DEBUG
	printf("%sprima di sem_wait\n", prefix);
#endif

#ifdef USE_SEMAPHORE
	if (sem_wait(semaphore) == -1) {
		perror("sem_wait");
		exit(EXIT_FAILURE);
	}

	// se sono stati impostati gestori di segnali senza il flag SA_RESTART, allora il modo corretto di usare sem_wait è il seguente.
	// vedere esempio 16sem_signal
	// vedere http://man7.org/linux/man-pages/man7/signal.7.html
	// mentre stiamo aspettando che il semaforo vanga sbloccato, la chiamata a sem_wait potrebbe essere interrotta
	// da un segnale il cui handler è stato impostato senza flag SA_RESTART

//	int res;
//	while ((res = sem_wait(semaphore)) == -1 && errno == EINTR)
//		continue;
//
//	if (res == -1) {
//		perror("sem_wait");
//		exit(EXIT_FAILURE);
//	}

#endif
	// inizio sezione critica
	*shared_counter += val;
	// fine sezione critica
#ifdef DEBUG
	printf("%snuovo valore contatore: %d\n",prefix, *shared_counter);

	usleep(100); // dormo 100 usec
#endif

#ifdef USE_SEMAPHORE
	if (sem_post(semaphore) == -1) {
		perror("sem_post");
		exit(EXIT_FAILURE);
	}
#endif

}


int main(int argc, char * argv[]) {

#ifdef DEBUG
	int cicli = 10;
#else
	int cicli = 1000000;
#endif

	// creato da parent process; condiviso tra i processi
	semaphore = create_named_semaphore();

	shared_counter = create_anon_mmap(sizeof(int));

	*shared_counter = 0;

    if (semaphore == NULL || shared_counter == NULL) {
    	printf("non posso proseguire!\n");

    	exit(EXIT_FAILURE);
    }

    char * prefix;
    int parent = 0;

	switch (fork()) {
	case -1:
		perror("fork");
		break;
	case 0: // child process

		prefix = "[child]";

		break;
	default: // parent process

		prefix = "[parent]";
		parent = 1;
	}

	printf("%sdormirò un secondo...\n", prefix);

	sleep(1);

	printf("%sprima del loop\n", prefix);
	// entrambi i processi incrementano il contatore per lo stesso numero di volte
	for (int i = 0; i < cicli; i++)
		add_counter(1, prefix);

	printf("%sdopo il loop\n",prefix);

	//il semaforo va chiuso da ogni processo che lo ha aperto
	if (sem_close(semaphore) == -1) {
		perror("sem_close");
	}

	sleep(1);

	if (parent && sem_unlink(semaphore_name) == -1) {
		perror("sem_unlink");
	}

	printf("%svalore finale contatore: %d, valore atteso: %d\n",prefix, *shared_counter, cicli * 2);

	munmap(shared_counter, sizeof(int));

	if (parent)
		wait(NULL);

	printf("%sbye\n", prefix);

	return 0;
}

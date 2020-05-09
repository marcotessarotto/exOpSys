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
 * esempio di sincronizzazione tra processi tramite segnalazione con semaforo
 *
 * http://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf
 * 3.1 Signaling pag. 16
 *
 */



sem_t * process_semaphore;


#define CHECK_ERR(a,msg) {if ((a) == -1) { perror((msg)); exit(EXIT_FAILURE); } }

#define CHECK_ERR_MMAP(a,msg) {if ((a) == MAP_FAILED) { perror((msg)); exit(EXIT_FAILURE); } }



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

	res = sem_init(process_semaphore,
					1, // 1 => il semaforo è condiviso tra processi, 0 => il semaforo è condiviso tra threads del processo
					0 // valore iniziale del semaforo
				  );

	CHECK_ERR(res,"sem_init")

	pid = fork();
	CHECK_ERR(pid,"fork")

	if (pid == 0) {

		printf("[child] before sem.signal()\n");

		// 3.1 Signaling
		// sem.signal() (libro pag. 11)
		if (sem_post(process_semaphore) == -1) {
			perror("sem_post");
			exit(EXIT_FAILURE);
		}

		printf("[child] terminating\n");

		exit(EXIT_SUCCESS);
	} else {

		printf("[parent] before sem.wait()\n");

		// 3.1 Signaling
		// sem.wait() (libro pag. 11)
		if (sem_wait(process_semaphore) == -1) {
			perror("sem_wait");
			exit(EXIT_FAILURE);
		}

		printf("[parent] after sem.wait()\n");

	}

	wait(NULL);

	// il semaforo senza nome va distrutto solo quando non ci sono processi bloccati su di esso
	res = sem_destroy(process_semaphore);

	CHECK_ERR(res,"sem_destroy")

	printf("[parent] bye\n");

	return 0;
}

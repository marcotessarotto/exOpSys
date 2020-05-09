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
 * esempio di sincronizzazione tra due processi tramite rendezvous
 *
 * http://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf
 * 3.3 Rendezvous pag. 12-15
 *
 * vogliamo garantire che:
 * a1 < b2 e b1 < a2
 *
 *
 */



sem_t * a_arrived;
sem_t * b_arrived;

#define CHECK_ERR(a,msg) {if ((a) == -1) { perror((msg)); exit(EXIT_FAILURE); } }

#define CHECK_ERR_MMAP(a,msg) {if ((a) == MAP_FAILED) { perror((msg)); exit(EXIT_FAILURE); } }



int main(int argc, char * argv[]) {

	int res;
	pid_t pid;

	printf("Rendezvous: a1 < b2 && b1 < a2\n");

	a_arrived = mmap(NULL, // NULL: è il kernel a scegliere l'indirizzo
			sizeof(sem_t) * 2, // dimensione della memory map
			PROT_READ | PROT_WRITE, // memory map leggibile e scrivibile
			MAP_SHARED | MAP_ANONYMOUS, // memory map condivisibile con altri processi e senza file di appoggio
			-1,
			0); // offset nel file

	CHECK_ERR_MMAP(a_arrived,"mmap")

	b_arrived = a_arrived + 1;

	res = sem_init(a_arrived,
					1, // 1 => il semaforo è condiviso tra processi, 0 => il semaforo è condiviso tra threads del processo
					0 // valore iniziale del semaforo
				  );

	CHECK_ERR(res,"sem_init")

	res = sem_init(b_arrived,
					1, // 1 => il semaforo è condiviso tra processi, 0 => il semaforo è condiviso tra threads del processo
					0 // valore iniziale del semaforo
				  );

	CHECK_ERR(res,"sem_init")

	pid = fork();
	CHECK_ERR(pid,"fork")

	if (pid == 0) {

		printf("[child] statement a1\n");

		printf("[child] before a_arrived.signal()\n");

		// 3.3.2 Rendezvous solution (libro pag. 15)
		if (sem_post(a_arrived) == -1) {
			perror("sem_post");
			exit(EXIT_FAILURE);
		}

		printf("[child] before b_arrived.wait()\n");

		if (sem_wait(b_arrived) == -1) {
			perror("sem_wait");
			exit(EXIT_FAILURE);
		}

		printf("[child] statement a2\n");

		printf("[child] terminating\n");

		exit(EXIT_SUCCESS);
	} else {

		printf("[parent] statement b1\n");

		printf("[parent] before b_arrived.signal()\n");

		// 3.3.2 Rendezvous solution (libro pag. 15)
		if (sem_post(b_arrived) == -1) {
			perror("sem_post");
			exit(EXIT_FAILURE);
		}

		printf("[parent] before a_arrived.wait()\n");

		if (sem_wait(a_arrived) == -1) {
			perror("sem_wait");
			exit(EXIT_FAILURE);
		}

		printf("[parent] statement b2\n");

	}

	wait(NULL);

	// il semaforo senza nome va distrutto solo quando non ci sono processi bloccati su di esso
	res = sem_destroy(a_arrived);

		CHECK_ERR(res,"sem_destroy")

	res = sem_destroy(b_arrived);

	CHECK_ERR(res,"sem_destroy")

	printf("[parent] bye\n");

	return 0;
}

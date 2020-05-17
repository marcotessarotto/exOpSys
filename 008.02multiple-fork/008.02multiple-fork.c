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

// soluzione con mutex di:
// https://repl.it/@MarcoTessarotto/fork-counter

typedef struct {
	sem_t sem; // semaforo per implementare il meccanismo di mutual exclusion (mutex)
	int val; // variabile condivisa
} shared_int;


shared_int * fork_counter;


#define CHECK_ERR(a,msg) {if ((a) == -1) { perror((msg)); exit(EXIT_FAILURE); } }

#define CHECK_ERR_MMAP(a,msg) {if ((a) == MAP_FAILED) { perror((msg)); exit(EXIT_FAILURE); } }

#define NUMBER_OF_GENERATIONS 4

int main(int argc, char * argv[]) {

	int res;
	pid_t pid;
	pid_t boss;
	int generation = 0;

	boss = getpid(); // il pid del primo processo
	printf("generation 0 process pid: %d\n", boss);

	fork_counter = mmap(NULL, // NULL: è il kernel a scegliere l'indirizzo
			sizeof(shared_int), // dimensione della memory map
			PROT_READ | PROT_WRITE, // memory map leggibile e scrivibile
			MAP_SHARED | MAP_ANONYMOUS, // memory map condivisibile con altri processi e senza file di appoggio
			-1, // nessun file di appoggio alla memory map
			0); // offset nel file

	CHECK_ERR_MMAP(fork_counter,"mmap")

	printf("fork_counter initial value: %d\n", fork_counter->val);

	res = sem_init(&fork_counter->sem,
					1, // 1 => il semaforo è condiviso tra processi, 0 => il semaforo è condiviso tra threads del processo
					1 // valore iniziale del semaforo (se mettiamo 0 che succede?)
				  );

	CHECK_ERR(res,"sem_init")

/*	pid = fork();
	CHECK_ERR(pid,"fork")

	if (pid != 0)
		printf("*new process %d cloned from %d\n", pid, getpid());
	else
		generation = 1;

	pid = fork();
	CHECK_ERR(pid,"fork")

	if (pid != 0)
		printf("**new process %d cloned from %d\n", pid, getpid());
	else
		generation = 2;

	pid = fork();
	CHECK_ERR(pid,"fork")

	if (pid != 0)
		printf("***new process %d cloned from %d\n", pid, getpid());
	else
		generation = 3;

	pid = fork();
	CHECK_ERR(pid,"fork")

	if (pid != 0)
		printf("****new process %d cloned from %d\n", pid, getpid());
	else
		generation = 4;*/

	for (int i = 0; i < NUMBER_OF_GENERATIONS; i++) {
		pid = fork();
		CHECK_ERR(pid,"fork")

		if (pid == 0) {
			generation = i+1;
			printf("*new process %d, gen=%d cloned from %d\n", getpid(), generation , getppid());
		}

	}

	// 3.4.2 Mutual exclusion solution, pag. 19
	if (sem_wait(&fork_counter->sem) == -1) {
		perror("sem_wait");
		exit(EXIT_FAILURE);
	}

	printf("before INCREMENT val=%d, process %d, gen=%d\n", fork_counter->val, getpid(), generation);
	// sezione critica
	fork_counter->val++;
	//

	if (sem_post(&fork_counter->sem) == -1) {
		perror("sem_post");
		exit(EXIT_FAILURE);
	}

	int child_counter = 0;

	do {
		printf("before wait - process %d\n", getpid());
		res = wait(NULL); // tutti i processi aspettano i processi figli, se he hanno

		if (res > 0) {
			printf("wait ok - process %d, gen=%d, has waited for process %d\n", getpid(), generation, res);
			child_counter++;
		} else {
			printf("wait err - process %d, gen=%d\n", getpid(), generation);
		}
	} while (res != -1);

	printf("+process %d, gen=%d has waited for %d processes\n", getpid(), generation, child_counter);

	printf("fork_counter value: %d, gen=%d, pid=%d\n", fork_counter->val, generation, getpid());

	// il semaforo senza nome va distrutto solo quando non ci sono processi bloccati su di esso
	// viene distrutto dal primo processo, che sarà l'ultimo a terminare
	if (generation == 0) {
		res = sem_destroy(&fork_counter->sem);

		CHECK_ERR(res,"sem_destroy")
	}

	printf("!bye pid=%d, gen=%d\n", getpid(), generation);


	return 0;
}

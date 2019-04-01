#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "files_common.h"


int launch_program(char program_to_launch[]) {

	int child_pid;

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

	char * shared_memory = create_anon_mmap(1024);

	if (shared_memory != NULL)
		strcpy(shared_memory, "messaggio per child process!");


	printf("[parent]sto per fare fork\n");

	if ((child_pid = fork()) != 0) { // parent

		printf("[parent]avviato child process: il suo PID é %d\n", child_pid);

		printf("[parent]ora aspetterò 5 secondi\n");

		for (int i = 1; i <= 5; i++) {
			printf("[parent]%d...\n", i);

			sleep(1);
		}

		printf("\n[parent]fine sleep\n");

		printf("[parent]contenuto shared memory:[%s]\n", shared_memory);

		munmap(shared_memory, 1024);

		return -1;
	}

	// se sono qui, sono il processo figlio

	printf("\n[child]sono il processo figlio, il mio PID è:%d\n", getpid());

	if (shared_memory != NULL) {
		printf("[child]contenuto della shared memory condivisa con parent: [%s]\n", shared_memory);

		printf("[child]modifico la shared memory con un messaggio per il parent process\n");
		strcpy(shared_memory,"ok ricevuto! messaggio da child process");

		munmap(shared_memory, 1024);
	}

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

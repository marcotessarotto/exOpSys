#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "files_common.h"

void string_token()
{
  char * word;
  char * saveptr;
  char cmd [] = "ls -la   notes.txt abcd.txt";

  printf( "[%s]\n", cmd );

//  char temp[1024];
//  strcpy( temp, cmd );   /* first make a copy of the cmd */

   /* before: "ls -la    notes.txt" */
  word = strtok_r( cmd, " \t", &saveptr );
   /* after:  "ls\0-la    notes.txt" */
   /*          ^
             word   */

  while ( word != NULL )
  {
    printf( "[%s] %s\n", cmd, word );
    word = strtok_r( NULL, " \t", &saveptr );
  }

   /* after: "ls\0-la\0   notes.txt\0 ...." */

  printf( "[%s]\n", cmd );

}

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

	if ((child_pid = fork()) != 0) { // parent

		printf("started child process, PID=%d\n", child_pid);

		return -1;
	}

	char *newargv[] = { NULL, "hello", "world", NULL };
	char *newenviron[] = { NULL };

	newargv[0] = program_to_launch;

	execve(program_to_launch, newargv, newenviron);

	perror("execve");   /* execve() returns only on error */

	exit(EXIT_FAILURE);
}


int main() {

   string_token();

   test_mmap();


   launch_program("../01printargs/Debug/01printargs");

   return EXIT_SUCCESS;
}

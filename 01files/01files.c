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

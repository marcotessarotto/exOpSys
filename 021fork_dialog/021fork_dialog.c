#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

//#include <libexplain/waitpid.h>

//extern char **environ;  /* Or define _GNU_SOURCE to get it from <unistd.h> */

// è necessario dialog, su debian : sudo apt install dialog

int main(int argc, char **argv) {

	pid_t child_pid;
	int childExitStatus;
	int status;



	printf("[parent] il mio PID è: %d\n", getpid());


	if ((child_pid = fork()) == 0) {

		// questo è il processo FIGLIO

		printf("[child] sono il processo figlio, il mio PID è: %d\n", getpid());

		char * newargv[] = {"dialog", "--backtitle", "backtitle del dialog" ,
		       "--title", "titolo del dialog" ,
		       "--yesno", "\nBla bla bla...\n\nTi va bene?", "10", "30", NULL };

		//char * newenviron[] = { NULL };

		printf("[child] prima di execve\n");

		//execve("/usr/bin/dialog", newargv, environ);



		char *argv2[] = { "sleep", "5", NULL };
		execve("/usr/bin/sleep", argv2, environ);


		perror("[child] se siamo qui, qualcosa è andato storto\n");

		exit(EXIT_FAILURE);

	} else {

		// questo è il processo PARENT


		printf("[parent] dopo il fork, sono il processo parent\n"
			   "[parent] il PID del processo figlio è %d\n", child_pid);

		// aspettiamo il termine del processo figlio e riceviamo il risultato
		if (wait(NULL) == -1) {
			perror("wait error");
		}

		// https://stackoverflow.com/a/48049663/974287

		pid_t ws = waitpid(child_pid, &childExitStatus, 0); //

		printf("ws=%d\n", ws);

		if (-1 == ws)
		{
			printf("was waiting for pid %d\n", child_pid);
			perror("[parent] - failed wait.");

//			fprintf(stderr, "%s\n", explain_waitpid(child_pid, status, 0));
//			exit(EXIT_FAILURE);

			return -1;
		}
		else if (0 == ws)
		{
			printf("[parent] - child is still running\n");
		}

		printf("[parent] - successfully waited for child with PID %d\n", (int) ws);

	    if (WIFEXITED(childExitStatus)) /* exit code in childExitStatus */
	    {
	    	printf("parent - got status %d", childExitStatus);
	        status = WEXITSTATUS(childExitStatus); /* zero is normal exit */
	        if (0 != status)
	        {
	        	printf("parent - picked up bad exit status\n");
	            return status;
	        }
	        return 0;
	    }
	    else
	    {
	    	printf("parent - bad exit route\n");
	        return -1;
	    }



	}




	return 0;
}

#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <errno.h>


int main(int argc, char *argv[]) {

	printf("*P* fork example (this is the parent: %d)\n", getpid());

	pid_t pid = fork(); // http://man7.org/linux/man-pages/man2/fork.2.html

	switch(pid) {
		case -1:
			fprintf(stderr,"fork returned error %d, quitting\n", errno);
			break;
		case 0: // child
			printf("*C* hello, child here! my process id is: %d\n", getpid());

			sleep(1); // http://man7.org/linux/man-pages/man3/sleep.3.html
			printf("*C* after sleep\n");

			break;
		default: // parent
			printf("*P* this is the parent! after fork. my process id is: %d\n", getpid());

			int status = 0;

			printf("*P* waiting for child termination...\n");

			pid_t child_pid = wait(&status); // http://man7.org/linux/man-pages/man2/wait.2.html

			printf("*P* child_pid=%i status=%i\n", child_pid, status);

	}

//	fflush(stdout);

	printf("process %d terminating\n", getpid());

	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>


/*
 * meccanismo fork-wait per avviare un nuovo processo ed aspettare la sua conclusione
 *
 * il processo padre apre due file, il secondo con flag O_CLOEXEC
 *
 * poi il processo padre avvia il processo figlio e aspetta la sua conclusione con wait()
 *
 * il processo figlio usa la funzione execlp (che a sua volta usa la system call execve)
 * per mostrare i propri file descriptors aperti
 *
 */

const char * file_name1 = "output1.txt";
const char * file_name2 = "output2.txt";

int main(int argc, char *argv[]) {

	int fdA, fdB;
	int flags;

	fdA = open(file_name1, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fdA == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	/* man 2 open :
			By default, the new file descriptor is set to remain open across an execve(2)
			(i.e., the FD_CLOEXEC file descriptor  flag  described  in  fcntl(2)  is  initially  disabled);
			the O_CLOEXEC flag, described below, can be used to change this default.
	*/

	fdB = open(file_name2, O_CREAT | O_RDWR | O_TRUNC | O_CLOEXEC, S_IRUSR | S_IWUSR);
	if (fdB == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	printf("[parent] file descriptors: fdA = %d, fdB = %d\n", fdA, fdB);

	// ottieni i flag di fdA
	flags = fcntl(fdA, F_GETFD);
	if (flags == -1) {
		perror("fcntl");
	}

	printf("[parent] flags fdA: %x\n", flags);

	// ottieni i flag di fd1
	flags = fcntl(fdB, F_GETFD);
	if (flags == -1) {
		perror("fcntl");
	} else if (flags & FD_CLOEXEC) {
		printf("[parent] fdB flags: FD_CLOEXEC\n");
	}



	if (fork() == 0) {
		int res;
		char ch = 'A';

		int pid;
		pid = getpid(); // child process pid

		printf("[child] fdA and fdB are shared with parent process");

		res = write(fdA, &ch, sizeof(ch));
		if (res == -1) {
			perror("write 1");
		} else {
			printf("[child] write to fdA ok\n");
		}

		res = write(fdB, &ch, sizeof(ch));
		if (res == -1) {
			perror("write 2");
		} else {
			printf("[child] write to fdB ok\n");
		}

		printf("[child] pid = %d, ppid = %d\n", pid, getppid());


		//exit(EXIT_SUCCESS);


		printf("[child] this process shows its own open file descriptors (inherited by parent process)\n");

		char cmd0[256] = { 0 };

		sprintf(cmd0, "/proc/%d/fd", pid);

		printf("[child] command: ls -l %s\n", cmd0);

		//getchar();

		// ci aspettiamo di trovare elencati tra i file descriptor aperti: fdA ma NON fbB

		execlp("ls", "ls", "-l", cmd0, NULL);

		exit(EXIT_FAILURE);

	}

	close(fdA);

	close(fdB);

	wait(NULL); // aspetta conclusione del processo figlio

	// facciamo pulizia: cancelliamo i due file

	unlink(file_name1);
	unlink(file_name2);

	printf("[parent] bye\n");

	return 0;

}

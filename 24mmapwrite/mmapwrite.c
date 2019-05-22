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
#include <time.h>

/**
 * esempio dove viene creato un file e viene scritto attraverso mmap
 */

#define FILE_SIZE 1024

int main(int argc, char * argv[]) {

	char * filename = "/tmp/prova.txt";
	int fd;
	char * memory;

	if ((fd = open(filename, O_RDWR
			 | O_CREAT | O_TRUNC, 0600 )) == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	if (ftruncate(fd, FILE_SIZE) == -1) {
		perror("ftruncate");
		exit(EXIT_FAILURE);
	}

	memory = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED , fd, 0);

	if (memory == MAP_FAILED) {
		perror("mmap");

		exit(EXIT_FAILURE);
	}

	close(fd);

	sprintf(memory, "questo è il processo con pid=%d, processo padre=%d\n", getpid(), getppid());

	if (msync(memory, FILE_SIZE, MS_SYNC) == -1) {
		perror("msync");
	}

	if (munmap(memory, FILE_SIZE) == -1) {
		perror("munmap");
	}

	printf("bye!\n");

	return 0;
}

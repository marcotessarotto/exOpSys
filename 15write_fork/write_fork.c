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

#define FILE_SIZE 1024
#define PIECES 16

const char * filename = "/tmp/prova.txt";

void process(const char * filename, int offset, int len, char ch) {

	int fd;

	if ((fd = open(filename, O_WRONLY)) == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	char * buffer = malloc(sizeof(char) * len);

	memset(buffer, ch, len-1);
	buffer[len-1] = '\n';

	lseek(fd, offset, SEEK_SET);

	write(fd, buffer, len);

	free(buffer);

	close(fd);

}

int main(int argc, char * argv[]) {

	int fd;

	// creiamo un file nella cartella dei file temporanei /tmp/

	if ((fd = open(filename,
			O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH )) == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	if (ftruncate(fd, FILE_SIZE) == -1) {
		perror("ftruncate");
		exit(EXIT_FAILURE);
	}

	close(fd);

	int pid;
	int offset, len;
	int boss = 1;

	for (int i = 0; i < PIECES; i++) {

		switch(pid = fork()) {
		case 0:

			offset = i * FILE_SIZE / PIECES;

			len = FILE_SIZE / PIECES;

			process(filename, offset, len, 'A' + i);

			boss = 0;

			goto end;
		default:
			;
		}

	} // for

	end:

	if (boss)
		while (wait(NULL) != -1)
			;

	printf("bye! pid=%d\n", getpid());

	return 0;
}


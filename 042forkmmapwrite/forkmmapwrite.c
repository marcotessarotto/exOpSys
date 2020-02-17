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


void get_current_time(char * buffer)
{
    time_t timer;
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
}

#define PIECES 16

#define FILE_SIZE (80*PIECES)

#define SECONDS 30

const char * filename = "/tmp/prova123.txt";

char * memory;


int counter = 0;

void process(char * buffer, int offset, int len, int id) {




	int n = sprintf(&buffer[offset], "pid=%d counter=%d ", getpid(), counter++);

	get_current_time(&buffer[offset + n]);

	buffer[offset + len - 1] = '\n';
}


int main(int argc, char * argv[]) {

	int fd;

	printf("main process pid: %d\n", getpid());
	printf("eseguire in shell il comando:\nwatch -n 1 -d cat %s\n", filename);

	// creiamo un file nella cartella dei file temporanei /tmp/

	if ((fd = open(filename, O_RDWR
			 | O_CREAT, 0600 )) == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	if (ftruncate(fd, FILE_SIZE) == -1) { // resize del file
		perror("ftruncate");
		exit(EXIT_FAILURE);
	}

	memory = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED , fd, 0);

	if (memory == MAP_FAILED) {
		perror("mmap");

		exit(EXIT_FAILURE);
	}

	close(fd);

	memset(memory, ' ', FILE_SIZE);

	// watch -n 1 -d cat  /tmp/prova123.txt



	int pid;
	int offset, len;
	int boss = 1;

	for (int i = 0; i < PIECES; i++) {

		switch(pid = fork()) {
		case 0:

			printf("starting child process %d\n", getpid());

			offset = i * FILE_SIZE / PIECES;

			len = FILE_SIZE / PIECES;

			for (int i = 0; i < SECONDS; i++) {
				process(memory, offset, len, i);
				sleep(1);
			}


			boss = 0;

			printf("ending child process %d\n", getpid());

			if (munmap(memory, FILE_SIZE) == -1) {
				perror("munmap");
			}

			exit(EXIT_SUCCESS);

		default:
			//munmap(memory, FILE_SIZE); <<--- interessante... se ripetiamo munmap n volte, gli altri processi non vedono più la memory map....
			;
		}

	} // for

	if (munmap(memory, FILE_SIZE) == -1) {
		perror("munmap");
	}

	if (boss)
		while (wait(NULL) != -1)
			;

	printf("bye! pid=%d\n", getpid());

	return 0;
}

//

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

// abilitare questo define per mostrare i problemi di concorrenza tra processi di questo esempio, quando i processi condividono lo stesso file handler
//#define PROBLEMA_CONCORRENZA


#ifdef PROBLEMA_CONCORRENZA

void process(int fd, int offset, int len, char ch) {

	// posizione corrente?
	__off_t position = lseek(fd, 0, SEEK_CUR);

	printf("posizione corrente nel file: %ld [%d]\n", position, getpid());

	char * buffer = malloc(sizeof(char) * len);

	memset(buffer, ch, len-1);
	buffer[len-1] = '\n';

	position = lseek(fd, offset, SEEK_SET);

	printf("posizione corrente nel file dopo lseek %ld, posizione attesa: %d [%d] %s\n", position, offset, getpid(), position == offset ? "OK" : "ERR");

	usleep(1000); // sleep per un millisecondo

	write(fd, buffer, len);

	// attenzione: tra il write e lseek successivo, un altro processo potrebbe avere cambiato l'offset
	position = lseek(fd, 0, SEEK_CUR);

	printf("posizione corrente nel file dopo write %ld, posizione attesa: %d [%d] %s\n", position, offset + len, getpid(), position == offset + len ? "OK" : "ERR");

	free(buffer);

	close(fd);

}

#else

void process(const char * filename, int offset, int len, char ch) {

	int fd;

	if ((fd = open(filename, O_WRONLY)) == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	// posizione corrente?
	__off_t position = lseek(fd, 0, SEEK_CUR);

	printf("posizione corrente nel file: %ld [%d]\n", position, getpid());

	char * buffer = malloc(sizeof(char) * len);

	memset(buffer, ch, len-1);
	buffer[len-1] = '\n';

	position = lseek(fd, offset, SEEK_SET);

	printf("posizione corrente nel file dopo lseek %ld, posizione attesa: %d [%d] %s\n", position, offset, getpid(), position == offset ? "OK" : "ERR");

	write(fd, buffer, len);

	free(buffer);

	close(fd);

}

#endif

int main(int argc, char * argv[]) {

	int fd;

	printf("main process pid: %d\n", getpid());

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

#ifndef PROBLEMA_CONCORRENZA
	// condividere il file aperto con gli altri processi comporta la condivisione del file offset tra tutti i processi
	// in questo esempio, vogliamo che tutti i processi lavorino sullo stesso file ma che ciascun processo abbia il proprio file offset
	close(fd);
#endif


	int pid;
	int offset, len;
	int boss = 1;

	for (int i = 0; i < PIECES; i++) {

		switch(pid = fork()) {
		case 0:

			offset = i * FILE_SIZE / PIECES;

			len = FILE_SIZE / PIECES;
#ifdef PROBLEMA_CONCORRENZA
			process(fd, offset, len, 'A' + i);
#else
			process(filename, offset, len, 'A' + i);
#endif

			boss = 0;

			goto end;
		default:
			;
		}

	} // for

	end:

#ifdef PROBLEMA_CONCORRENZA
	close(fd);
#endif

	if (boss)
		while (wait(NULL) != -1)
			;

	printf("bye! pid=%d\n", getpid());

	if (boss) {

		printf("\ncat %s\n\n", filename);
		printf("sono attese %d righe\n\n", PIECES);

		char * newargv[] = { "cat", filename , NULL };

		char * newenviron[] = { NULL };

		execve("/bin/cat", newargv, newenviron);
	}

	return 0;
}


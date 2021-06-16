#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h> // signal

#include <errno.h>

/*

create a very large file (i.e. 100 GB size). write '!' at position 0 and '?' at position N-1
but the file is mostly 'empty' so it does not occupy 100 GB on disk.

 */

#define _1_MB 1024*1024
#define _1_GB 1024*_1_MB
#define _100_GB 100UL*_1_GB

#define N _100_GB

int main() {
	char * file_name = "large_file.dat";
	int fd;
	char * map;

	fd = open(file_name, O_CREAT | O_TRUNC | O_RDWR, 0600);
	if (fd == -1) {
		perror("open");
		exit(1);
	}

	if (ftruncate(fd, N) == -1) {
		perror("ftruncate");
		exit(1);
	}

	map = mmap(NULL, N, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (map == (void *) -1) {
		perror("mmap");
		exit(1);
	}

	close(fd);

	map[0] = '!';
	map[N-1] = '?';

	system("/usr/bin/ls -l large_file.dat");

	printf("press enter to continue\n");
	getchar();

	if (munmap(map, N) == -1) {
		perror("munmap");
		exit(1);
	}

	if (unlink(file_name) == -1) {
		perror("unlink");
		exit(1);
	}

	printf("bye\n");

	return 0;
}

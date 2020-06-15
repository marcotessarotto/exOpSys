#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

/*
 * crea un file e poi lo cancella
 *
 */


const char * file_name = "output.txt";

#define CASE 2

int main(int argc, char *argv[]) {

	int fdA;
	int res;

	char * current_working_directory;

	current_working_directory = getcwd(NULL, 0);

	printf("current_working_directory: %s\n", current_working_directory);

	fdA = open(file_name, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fdA == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	if (CASE == 1) {

		// chiudo il file
		res = close(fdA);
		if (res == -1) {
			perror("close");
		} else {
			printf("close ok\n");
		}

		// cancello il file
		res = unlink(file_name);
		if (res == -1) {
			perror("unlink");
		} else {
			printf("unlink ok\n");
		}


	} else if (CASE == 2) {

		// chiudo il file
		res = close(fdA);
		if (res == -1) {
			perror("close");
		} else {
			printf("close ok\n");
		}

		int fd_cwd;

		// apro la directory
		fd_cwd = open(".", O_DIRECTORY);
		if (fd_cwd == -1) {
			perror("open");
			exit(EXIT_FAILURE);
		}

		// int unlinkat(int dirfd, const char *pathname, int flags);

		// cancello il file output.txt nella cartella specificata dal file descriptor fd_cwd
		res = unlinkat(fd_cwd, file_name, 0);
		if (res == -1) {
			perror("unlinkat");
		} else {
			printf("unlinkat ok\n");
		}


	} else if (CASE == 3)  {
		// cosa succederà? cancello il file prima di chiuderlo

		// da man 2 unlink:
		// If the name was the last link to a file but any processes still have the file open,
		// the file will remain in existence until the last file descriptor referring to it is closed.

		res = unlink(file_name);
		if (res == -1) {
			perror("unlink");
		} else {
			printf("unlink ok\n");
		}

		res = close(fdA);
		if (res == -1) {
			perror("close");
		} else {
			printf("close ok\n");
		}
	}


	return 0;

}

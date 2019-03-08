#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


char * sample_file_name = "/boot/initrd.img-4.9.0-8-amd64";

long getFileSize(int fd) {

	struct stat st;

	int res = fstat(fd, &st);

	if (res == -1) {
		perror("stat error");
		return -1;
	} else
		return st.st_size;

}

int main() {
	int fd;

	fd = open(sample_file_name, O_RDONLY);

	if (fd == -1) {
		perror("errore nell'apertura del file");
		exit(EXIT_FAILURE);
	}

	long file_size = getFileSize(fd);

	printf("file size: %ld\n", file_size);


	char * file_content = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);

	if (file_content == MAP_FAILED) {
		perror("mmap failed!");
	} else {
		// processing

		close(fd);

		///
		int counter = 0;

		for (int i = 0; i < file_size; i++) {

			if (file_content[i] == 'A')
				counter++;

		}

		printf("counter=%d\n", counter);

		///


		int res = munmap(file_content, file_size);

		if (res == -1) {
			perror("munmap error");
		}
	}

//	close(fd);

	printf("finished!\n");
}

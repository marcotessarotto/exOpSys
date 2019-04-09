#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>




long get_file_size(int fd) {

	struct stat st;

	int res = fstat(fd, &st);

	if (res == -1) {
		perror("stat error\n");
		return -1;
	} else
		return st.st_size;

}

/*
 * restituisce il contenuto del file, come se fosse una stringa (ovvero lo termina con 0);
 * per funzionare, è necessario che la dimensione del file sia != 0
 */
char * cat_mmap(char * file_name) {

	int fd;

	fd = open(file_name, O_RDONLY);

	if (fd == -1) {
		perror("error opening file\n");
		return NULL;
	}

	long file_size = get_file_size(fd);

	printf("file size: %ld\n", file_size);

	char * file_content = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);

	close(fd);

	if (file_content == MAP_FAILED) {
		perror("mmap fail\n");
		return NULL;
	}

	char * result = malloc(file_size + 1);

	memcpy(result, file_content, file_size);

	munmap(file_content, file_size);

	result[file_size] = 0;

	return result;
}

#define SIZE 1024
#define DEBUG 0

/*
 * restituisce il contenuto del file, come se fosse una stringa (ovvero lo termina con 0)
 */
char * cat(char * file_name) {

	int bytes_read;

	int fd;

	fd = open(file_name, O_RDONLY);

	if (fd == -1) {
		perror("error open file\n");
		return NULL;
	}

	char * result = malloc(SIZE + 1); // il +1 è per lo '\0' finale
	char * ptr = result;
	int total_size = SIZE + 1;

	while ((bytes_read = read(fd, ptr, SIZE)) > 0) {

		if (DEBUG)
			printf("total_size=%d bytes_read=%d\n", total_size, bytes_read);

		if (bytes_read < SIZE) {
			total_size -= (SIZE - bytes_read);
			result = realloc(result, total_size);

			break;
		}

		total_size += SIZE;
		result = realloc(result, total_size);
		ptr += SIZE;
	}

	if (bytes_read == 0) {
		if (DEBUG)
			printf("realloc 3\n");

		total_size -= SIZE;
		result = realloc(result, total_size);
	}

	close(fd);

	// aggiungiamo '\0' finale
	result[total_size - 1] = '\0';

	if (DEBUG)
		printf("total_size=%d\n", total_size);

	return result;
}


#define _GNU_SOURCE
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
#include <pthread.h>

long get_file_size(int fd) {

	struct stat st;

	int res = fstat(fd, &st);

	if (res == -1) {
		perror("stat error\n");
		return -1;
	} else
		return st.st_size;

}

#define ONE_MB (1024*1024)

const char * file_name = "/tmp/32mapped_file.dat";

int fd;

long int file_size = ONE_MB;


char * file_map;

int main(int argc, char * argv[]) {

	printf("mappiamo in memoria un file intero di dimensione iniziale 1 MB\n"
			"poi raddoppiamo la dimensione del file e "
			"ridimensioniamo la 'mappa del file' per "
			"continuare a coprire l'intero file\n\n");

	// se il file esiste già...
	if( access( file_name, F_OK ) != -1 ) {
		unlink(file_name); // lo cancelliamo
	}

	printf("creazione del file %s\n", file_name);

	if ((fd = open(file_name, O_RDWR
			 | O_CREAT, 0600 )) == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	printf("dimensione attuale del file appena creato: %ld bytes,\n"
			"ora imposto la dimensione del file a %ld bytes\n\n", get_file_size(fd), file_size);

	if (ftruncate(fd, file_size) == -1) { // resize del file
		perror("ftruncate");
		exit(EXIT_FAILURE);
	}

	printf("nuova dimensione del file: %ld bytes\n\n", get_file_size(fd));

	printf("creo la mappa del file in memoria\n");
	if ((file_map = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}

	printf("indirizzo della mappa del file: %p\n\n", file_map);

	int old_file_size = file_size;

	file_size += ONE_MB;

	printf("dimensione attuale del file: %ld bytes,\n"
				"ora imposto la dimensione del file a %ld bytes\n\n", get_file_size(fd), file_size);

	if (ftruncate(fd, file_size) == -1) { // resize del file
		perror("ftruncate");
		exit(EXIT_FAILURE);
	}

	printf("nuova dimensione del file: %ld bytes\n\n", get_file_size(fd));

	printf("ora aggiorno la dimensione della mappa del file in memoria\n\n");
	if ((file_map = mremap(file_map, old_file_size, file_size, MREMAP_MAYMOVE)) == MAP_FAILED) {
		perror("mremap");
		exit(EXIT_FAILURE);
	}

	printf("dopo mremap, indirizzo della mappa del file: %p\n\n", file_map);

	// proviamo ad accedere alla mappa del file
	for (int i = 0; i < 10; i++) {
		printf("file_map[%i] = %i\n", i, file_map[i]);
	}

	printf("\nfinito, chiudo tutto\n");

	if (munmap(file_map, file_size) == -1) {
		perror("munmap");
	}

	if (close(fd) == -1) {
		perror("close");
	}

	return 0;
}

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

char * read_random_bytes(int size) {

	char * fileName = "/dev/urandom";

	int fd = open(fileName, O_RDONLY);

	if (fd == -1) { // errore!

		fprintf(stderr,"open error %d\n", errno);

		return NULL;
	}

	printf("descriptor = %d\n", fd);

	char * buffer;
	int bytesRead;
	int pos = 0;
	int totalBytesToRead = size * sizeof(char);

	buffer = malloc(totalBytesToRead);

	while (pos < totalBytesToRead) {

		bytesRead = read(fd, buffer + pos, totalBytesToRead - pos);

		if (bytesRead == -1) {
			fprintf(stderr, "error while reading %d\n", errno);

			free(buffer);

			return NULL;
		}

		pos += bytesRead;

		fprintf(stdout,"read %d bytes from file, now at position %d of %d\n", bytesRead, pos, totalBytesToRead);

	}

	fprintf(stdout,"total read %d bytes from file\n", pos);

	close(fd);

	return buffer;
}

ssize_t min(ssize_t a, ssize_t b) {
	return a < b ? a : b;
}

int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("scrive N bytes random nel file indicato\n");
		printf("utilizzo: write_file_sc FILE [numero_bytes]\n");
		printf("numero_bytes è opzionale, default value: 16384\n");
		exit(EXIT_SUCCESS);
	}

	char * file_name = argv[1];
	int output_file_size = 1024 * 16;

	if (argc >= 3) {
		output_file_size = strtol(argv[2], NULL, 10); // base 10

		if (output_file_size < 0)
			output_file_size = 1024 * 16;
	}

	printf("scrivo file %s, dimensione finale=%d\n", file_name, output_file_size);

	int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

	if (fd == -1) {

		fprintf(stderr ,"error in opening file for writing! err=%d\n", errno);

		exit(EXIT_FAILURE);
	}

	ssize_t totalBytesToWrite = output_file_size;
	ssize_t readSize = 1024;

	char * random_data = read_random_bytes(totalBytesToWrite);


	int pos = 0;

	ssize_t bytesWritten;

	while (pos < totalBytesToWrite) {

		bytesWritten = write(fd, random_data + pos, min(totalBytesToWrite - pos,readSize) );

		if (bytesWritten == -1) {
			fprintf(stderr, "error while writing %d\n", errno);

			free(random_data);

			close(fd);

			exit(EXIT_FAILURE);
		}

		pos += bytesWritten;

		fprintf(stdout,"have written %ld bytes to file, now at position %d of %ld\n", bytesWritten, pos, totalBytesToWrite);

	}

	close(fd);

	free(random_data);

	printf("complete!\n");


	exit(EXIT_SUCCESS);
}

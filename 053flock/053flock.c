#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//#define DEBUG

#define USE_FLOCK


ssize_t read_from_fd(int fd, char * buffer, size_t count) {


	ssize_t bytes_read;
	size_t position = 0;

	while ( (bytes_read = read(fd, &buffer[position], count - position)) > 0) {
		position += bytes_read;

		if (position == count) {
			//printf("*1* position=%d\n", position);
			return position;
		}
	}

	if (bytes_read < 0) {
		perror("read()");
		return -1;
	} else {
		//printf("*2*\n");
		return position;
	}

}

ssize_t write_to_fd(int fd, char * buffer, size_t count) {

	ssize_t bytes_written;
	size_t position = 0;

	while ( (bytes_written = write(fd, &buffer[position], count - position)) > 0 ) {
		position += bytes_written;

		if (position == count) {
			//printf("+1+\n");
			return position;
		}
	}

	if (bytes_written < 0) {
		perror("write()");
		return -1;
	} else {
		//printf("+2+\n");
		return position;
	}

}


int increment_counter(char * counter_filename) {

	int fd;
#ifdef DEBUG
	__off_t current_file_offset;
#endif
	int counter;
	int res;

	fd = open(counter_filename, O_RDWR);

	if (fd == -1) {
		perror("open()");
		exit(1);
	}

#ifdef USE_FLOCK
	res = flock(fd, LOCK_EX);
    if (res == -1) {
    	perror("flock()");
    	return -1;
    }
#endif

#ifdef DEBUG
	current_file_offset = lseek(fd, 0, SEEK_CUR);
	printf("current_file_offset = %ld\n", current_file_offset);
#endif

	res = read_from_fd(fd, (char *) &counter, sizeof(counter));

	if (res == -1) {
		printf("errore in read_from_fd\n");
		return -1;
	}

	counter++;


#ifdef DEBUG
	current_file_offset = lseek(fd, 0, SEEK_CUR);
	printf("current_file_offset = %ld\n", current_file_offset);
#endif


	res = lseek(fd, - sizeof(counter), SEEK_CUR);
	if (res == -1) {
		perror("lseek");
		return -1;
	} else {
#ifdef DEBUG
		printf("current_file_offset = %d\n", res);
#endif
	}

	res = write_to_fd(fd, (char *) &counter, sizeof(counter));

	if (res == -1) {
		printf("errore in write_to_fd\n");
		return -1;
	}

#ifdef USE_FLOCK
	res = flock(fd, LOCK_UN);
    if (res == -1) {
    	perror("flock()");
    	return -1;
    }
#endif


	if (close(fd) == -1) {
		perror("close()");
	}

	return counter;
}


int main() {

	// ipotesi: il file che contiene il valore del contatore esiste già

	// es:
	// rm counter_file
	// truncate -s 4 counter_file

	// od -t x1 counter_file

	char * filename = "counter_file";
	int counter_value;

	int fd;
	fd = open(filename, O_RDWR);
	if (fd == -1) {
		printf("devi creare il file con il comando:\ntruncate -s 4 counter_file\n");
		exit(1);
	}
	close(fd);

	for (int i = 0; i < 5000000; i++) {
		counter_value = increment_counter(filename);
	}

	printf("counter value=%d\n", counter_value);

    return 0;
}


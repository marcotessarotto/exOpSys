#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/*
 * legge tutto il contenuto di un file prova.txt copiandolo in un vettore
 * allocato sulla heap ed aumentandolo fino a contenere tutto il file
 *
 *
 */


#define INIT_BUF_SIZE 256
#define INCREMENT INIT_BUF_SIZE

#define PRINT_FILE_CONTENTS 0

char * buffer;
unsigned int buf_len;

int main() {

	char * file_name = "prova.txt";
	int fd;

	buffer = malloc(INIT_BUF_SIZE);
	if (buffer == NULL) {
		perror("malloc()");

		exit(EXIT_FAILURE);
	}
	buf_len = INIT_BUF_SIZE;

	fd = open(file_name, O_RDONLY);

	if (fd == -1) {
		perror("open()");

		exit(EXIT_FAILURE);
	}

	printf("fd = %d\n", fd);

    int position = 0; // posizione in buffer dove scriverò i dati
    int bytes_read; // numero di byte letti da read()

    // IPOTESI: il file che leggeremo ha dimensione <= 1024 bytes

    while ( (bytes_read = read(fd, &buffer[position], buf_len-position) ) > 0) {
    	printf("read() ha restituito %d bytes\n", bytes_read);

    	position += bytes_read;

    	printf("position = %d\n", position);

    	if (buf_len == position) { // il buffer si è riempito
    		buf_len += INCREMENT;
    		// oppure:
    		// buf_len *= 2;

    		printf("realloc() - nuovo buf_len=%d\n", buf_len);
    		buffer = realloc(buffer, buf_len);
    		if (buffer == NULL) {
    			perror("malloc()");

    			exit(EXIT_FAILURE);
    		}
    	}
    }

    if (bytes_read == 0) {
    	printf("ok, siamo a EOF ed abbiamo letto complessivamente %d bytes\n", position);
    } else {
    	perror("problema con read()");
    }

    printf("buf_len finale: %d\n", buf_len);

    // facciamo qualcosa con i dati ora nella memoria del processo
    // ....
    // scriviamo su stdout il contenuto del file letto, byte per byte

    if (PRINT_FILE_CONTENTS)
		for (int i = 0; i < position; i++) {
			printf("buffer[%d] = %u    %c\n", i, buffer[i], buffer[i]); // %u : intero senza segno
		}


    if (close(fd) == -1) { // non necessario, dato che il programma sta per terminare
    	perror("problema con close()");
    }

    free(buffer); // non necessario, dato che il programma sta per terminare

    // quando il processo termina, eventuali file aperti vengono chiusi automaticamente dal kernel

    return 0;
}

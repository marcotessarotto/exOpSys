#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/*
 * leggi tutto il contenuto di un file prova.txt copiandolo in un vettore di dimensione fissa
 * stando attento a non superare il limite del vettore stesso
 *
 *
 */

#define BUF_SIZE 1024

char buffer[BUF_SIZE];


int main() {

	char * file_name = "prova.txt";
	int fd;

	fd = open(file_name, O_RDONLY);

	if (fd == -1) {
		perror("open()");

		exit(EXIT_FAILURE);
	}

	printf("fd = %d\n", fd);

    int position = 0; // posizione in buffer dove scriverò i dati
    int bytes_read; // numero di byte letti da read()

    // IPOTESI: il file che leggeremo ha dimensione <= 1024 bytes

    while ( (bytes_read = read(fd, &buffer[position], 1024-position) ) > 0) {
    	printf("read() ha restituito %d bytes\n", bytes_read);

    	position += bytes_read;

    	printf("position = %d\n", position);
    }

    if (bytes_read == 0) {
    	printf("ok, siamo a EOF ed abbiamo letto complessivamente %d bytes\n", position);
    } else {
    	perror("problema con read()");
    }

    // facciamo qualcosa con i dati ora nella memoria del processo
    // ....
    // scriviamo su stdout il contenuto del file letto, byte per byte

    for (int i = 0; i < position; i++) {
    	printf("buffer[%d] = %u    %c\n", i, buffer[i], buffer[i]); // %u : intero senza segno
    }


    if (close(fd) == -1) {
    	perror("problema con close()");
    }

    // quando il processo termina, eventuali file aperti vengono chiusi automaticamente dal kernel

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 * in questo esempio, leggiamo un file di testo, una riga alla volta;
 * la riga la intendiamo terminata da \n
 *
 * utilizziamo lseek per "restituire" la parte di riga dopo \n
 * ovvero con lseek spostiamo all'indietro il file offset
 *
 */


// max length of line of text:
// wc -L /usr/share/dict/words
// 23

#define DEBUG_MSG 1


void process_line(char * line, int line_len) {
	// do something...
	printf("process_line: %s\n", line);

	// ATTENZIONE!
	// la stringa di caratteri line va COPIATA, se si vuole conservare
	// per successive elaborazioni
}

void read_lines_from_file(char * filename) {

	int fd;
	int bytes_read; // bytes letti da system call read()

	char * buffer; // usato per i dati restituiti da system call read()
	int buffer_size = 1; // dimensione della memoria allocata dinamicamente per buffer

	// naturalmente possiamo partire con una dimensione maggiore di 1,
	// ma ci interessava verificare il comportamento del codice
	buffer = malloc(buffer_size * sizeof(char));
	if (buffer == NULL) {
		perror("malloc");
		exit(1);
	}

	int buffer_len;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(1);
	}

	printf("reading file %s\n", filename);

	int count_lines = 0;
	int realloc_counter = 0;

	while ((bytes_read = read(fd, buffer, buffer_size)) > 0) {

		// cerchiamo la posizione della prima occorrenza di \n dentro i dati restituiti da read()
		int pos = 0;
		while (buffer[pos] != '\n' && pos < bytes_read )
			pos++;
		// se \n non è presente, la condizione è pos == bytes_read

		// se non abbiamo trovato 'n' nei dati letti E buffer_size == bytes_read,
		// allora: aumentiamo la dimensione del buffer, restituiamo i dati con lseek e rileggiamo la linea
		if (pos == bytes_read && buffer_size == bytes_read) {

			// aumentiamo la dimensione del buffer
			buffer_size = buffer_size * 2; // raddoppiamo la dimensione
			buffer = realloc(buffer, buffer_size);
			if (buffer == NULL) {
				perror("realloc");
				exit(1);
			}

#if DEBUG_MSG
			printf("***realloc: buffer_size=%d\n", buffer_size);
#endif

			realloc_counter++;

			// spostiamo indietro il file offset (cioè restituiamo tutti i dati della linea corrente)
			if (lseek(fd, -bytes_read, SEEK_CUR) == -1) {
				perror("lseek");
				exit(1);
			}

			// e rileggiamo i dati (della linea corrente)
			continue;
		}

		count_lines++;

		buffer[pos] = 0; // 'terminiamo' la stringa (sostituiamo \n con \0)
		buffer_len = pos; // strlen(buffer); la lunghezza della stringa è nota (è pos)
#if DEBUG_MSG
		printf("buffer_size=%6d   buffer_len = %6d    buffer=%s\n", buffer_size, buffer_len, buffer);
#endif

		// la linea (stringa di caratteri) corrente si trova in buffer ed è terminata da \0
		// process_line fa "qualcosa" con il contenuto letto
		// per comodità di process_line, passiamo anche buffer_len
		process_line(buffer, buffer_len);

		// se ci sono dati non processati in buffer, li "restituiamo"
		// spostando all'indietro l'offset del file
		// così i dati non processati saranno "ri-restituiti" nella prossima invocazione di read()
		__off_t new_offset =  pos + 1 - bytes_read; // calcoliamo lo spostamento relativo

#if DEBUG_MSG
		// spostiamo l'offseet rispetto alla posizione corrente
		printf("+++ lseek: SEEK_CUR %ld\n", new_offset);
#endif
		// invochiamo la system call se e solo se new_offset != 0
		if (new_offset != 0 && lseek(fd,new_offset , SEEK_CUR) == -1) {
			perror("lseek");
			exit(1);
		}

	} // fine while


	if (bytes_read == 0) {
		printf("EOF\n");
	} else if (bytes_read == -1) {
		perror("read");
	}

	printf("final buffer_size = %d\n"
			"count_lines=%d\n"
			"realloc_counter=%d\n",
			buffer_size, count_lines, realloc_counter);

}


int main() {
	char * filename;
	//filename = "/usr/share/dict/words";
	filename = "sample.txt";
	//filename = "/proc/cpuinfo";

	read_lines_from_file(filename);

	return 0;
}

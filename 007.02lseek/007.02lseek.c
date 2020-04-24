#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>


off_t show_current_file_offset(int fd);

int main(int argc, char * argv[]) {

/*

                    *FILE OFFSET*

Per ogni file aperto, il kernel mantiene un "file offset",
talvolta chiamato anche "read-write offset" o "puntatore".

Questa è la posizione nel file in cui inizierà il prossimo read() o write().
L'offset del file è espresso come una posizione di byte rispetto all'inizio del file.
Il primo byte del file è all'offset 0.

L'offset del file viene impostato in modo che punti all'inizio del file
quando viene aperto il file e viene automaticamente
aggiornato da ogni chiamata successiva read() o write()
in modo che punti, nel file, al byte successivo
a quello appena letto o scritto.

Pertanto, chiamate successive a read() e write() percorrono il file
in modo sequenziale (nella direzione che va dall'inizio del file verso la fine dello stesso).

*/

	char * file_name;
	int res;
	off_t file_offset;

	if (argc == 1) {
		printf("specificare come parametro il nome del file\n");
		exit(EXIT_FAILURE);
	}

	file_name = argv[1];
	printf("opero sul file %s\n", file_name);

	// apro un file già esistente in modalità lettura/scrittura
	// ad esempio, possiamo lavorare su un file prodotto dall'esempio 005.8writefile
	// oppure sul file sample.txt in questa cartella di progetto
	int fd = open(file_name, O_RDWR); // O_RDWR: apri il file in modalità lettura e scrittura

	if (fd == -1) { // errore!

		perror("open()");

		exit(EXIT_FAILURE);
	} else {
		printf("open() ok\n");
	}

	// all'apertura del file, il file offset vale 0
	// non occorre impostare il file offset

	show_current_file_offset(fd);

	// spostiamo il file offset alla posizione 256 (dall'inizio del file)

	file_offset = lseek(fd, 256, SEEK_SET); // SEEK_SET: posizione rispetto all'inizio del file
	// lseek: libro Kerrisk pag. 81

	if (file_offset == -1) {
		perror("lseek()");
		exit(EXIT_FAILURE);
	}


	// leggiamo 16 bytes
	char buf[16];
	res = read(fd, buf, sizeof(buf) / sizeof(char));

	if (res == -1) {
		perror("read()");

		exit(EXIT_FAILURE);
	}

	buf[15] = 0;
	printf("read: %s\n", buf);


	// poi spostiamo il file offset all'inizio del file e scriviamo qualcosa

	file_offset = lseek(fd, 0, SEEK_SET);
	if (file_offset == -1) {
		perror("lseek()");
		exit(EXIT_FAILURE);
	} else {
		printf("offset set to start of file\n");
	}

	char * msg = "ciao!";

	res = write(fd, msg, strlen(msg));

	if (res == -1) {
		perror("write()");

		exit(EXIT_FAILURE);
	} else {
		printf("write ok\n");
	}

	// ...
	if (close(fd) == -1) {
		perror("close()");
	}

	printf("bye!\n");

	return 0;
}

off_t show_current_file_offset(int fd) {

	off_t file_offset;

	file_offset = lseek(fd, 0, SEEK_CUR); // SEEK_CUR: posizione relativa rispetto alla posizione corrente

	if (file_offset == -1) {
		perror("lseek()");
		exit(EXIT_FAILURE);
	}

	printf("current file offset: %ld\n", file_offset);

	return file_offset;

}

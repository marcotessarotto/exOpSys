#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>


/*
 * creare un file e impostare la sua dimensione con ftruncate
 *
 * si può impostare la dimensione di un file anche con lseek() (probabilmente svilupperò un altro esempio)
 */


int main(int argc, char * argv[]) {

	char * file_name;
	int res;

	if (argc == 1) {
		printf("specificare come parametro il nome del file\n");
		exit(EXIT_FAILURE);
	}

	file_name = argv[1];
	printf("opero sul file %s\n", file_name);

	// se il file esiste, mi fermo e restituisco errore

	// apriamo il file in scrittura, se non esiste verrà creato,
	// se esiste già la sua dimensione viene troncata a 0
	// e poi scriviamo nel file (vedere text_to_write)

	// tratto da man 2 open
	// O_EXCL Ensure that this call creates the file: if this flag is specified in conjunction with O_CREAT, and pathname already exists, then open() fails with the error EEXIST.
	// O_CREAT  If pathname does not exist, create it as a regular file.
	// O_TRUNC  If the file already exists and is a regular file and the access mode allows writing ... it will be truncated to length 0.
	// O_RDONLY, O_WRONLY, or O_RDWR  These request opening the file read-only, write-only, or read/write, respectively.

	int fd = open(file_name,
				  O_CREAT | O_TRUNC | O_WRONLY | O_EXCL,
				  S_IRUSR | S_IWUSR // l'utente proprietario del file avrà i permessi di lettura e scrittura sul nuovo file
				 );

	if (fd == -1) { // errore!
			perror("open()");
			exit(EXIT_FAILURE);
		}

	// da man ftruncate :
	// int ftruncate(int fd, off_t length);

	// off_t è un intero senza segno a 64 bit

#define ONE_MEGA_BYTE 1000 * 1000UL

	// The truncate() and ftruncate() functions cause the regular file named by path
	// or referenced by fd to be truncated to a size of precisely length bytes.

	// usiamo ftruncate per impostare la dimensione del file (appena creato) a 100 MB
	res = ftruncate(fd, 100 * ONE_MEGA_BYTE);

	if (res == -1) {
		perror("ftruncate()");
		exit(EXIT_FAILURE);
	}

	if (close(fd) == -1) {
		perror("close()");
	}

	return 0;
}

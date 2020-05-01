#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/random.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_LEN 1024*4

char content_to_write[BUF_LEN];

// vedere anche:
// https://repl.it/@MarcoTessarotto/getrandom-syscall

void fill_with_random_material(char * buffer, size_t buf_len) {
	ssize_t pos = 0;
	ssize_t res;

	while (pos < buf_len) {
		res = getrandom(&buffer[pos],
			  buf_len - pos,
			  //GRND_RANDOM |
			  //GRND_NONBLOCK |
			  0);

		if (res == -1) {
			perror("getrandom()");
			if (errno == EINTR || errno == EAGAIN) {
				continue;
			} else {
				exit(EXIT_FAILURE);
			}
		}

		pos += res;

	}

}

#define CHECK_ERR(a,msg) {if ((a) == -1) { perror((msg)); exit(EXIT_FAILURE); } }

int main(int argc, char * argv[]) {

	char * file_name = "output.txt";
	int res;

	// riempiamo content_to_write di materiale random
	fill_with_random_material(content_to_write, BUF_LEN);

	//	il processo padre crea un file output.txt (se il file esiste già, va troncato a zero e sovrascritto).

	int fd = open(file_name,
					  O_CREAT | O_WRONLY | O_TRUNC,
					  S_IRUSR | S_IWUSR // l'utente proprietario del file avrà i permessi di lettura e scrittura sul nuovo file
					 );
	// fd == 3

	CHECK_ERR(fd, "open()")

	// usiamo ftruncate per impostare la dimensione del file (appena creato) a BUF_LEN bytes
	res = ftruncate(fd, BUF_LEN);

	CHECK_ERR(res, "ftruncate()")

//	il processo padre tiene aperto il file output.txt, salvare il descrittore in una variabile fd

	// il file output.txt è stato creato se non esisteva
	// la sua dimensione è stata impostata a BUF_LEN bytes

	// content_to_write è grande BUF_LEN bytes


	// processo padre e processo figlio condividono lo stesso file descriptor

	// i due processi operano contemporaneamente sul file, senza sincronizzarsi

	//	il processo padre scrive la prima metà di content_to_write nella prima metà del file
	//	cominciando a scrivere dalla posizione iniziale del file.

	//	il processo figlio scrive l'altra metà di content_to_write del file,
	//	scrivendo nella seconda metà del file.

	//	il processo padre attende la conclusione del processo figlio e poi termina.

	printf("processo padre e processo figlio condividono lo stesso file descriptor\n"
			"il file offeset è condiviso tra i due processi\n"
			"ma grazie alla system call pwrite i due processi possono scrivere sul file descriptor condiviso in contemporanea\n\n");

	// una alternativa è che ciascun processo apra il file per conto suo e ci operi senza condividere il file offset

	switch(fork()) {
	case 0:
		// scriviamo la seconda metà di content_to_write nella seconda metà del file
		res = pwrite(fd,
				&content_to_write[BUF_LEN/2], // indirizzo della zona di memoria da scrivere
				BUF_LEN/2, // lunghezza della zona di memoria
				BUF_LEN/2); // file offset da utilizzare per la scrittura su file

		// ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);

		// pwrite scrive i contenuti nel file all'offset specificato come parametro

		// pwrite NON CAMBIA il file offset del file descriptor

		// non occorre impostare il file offset prima di chiamare pwrite

		CHECK_ERR(res, "pwrite()")

		printf("[child] pwrite ok\n");

		exit(EXIT_SUCCESS);
	case -1:
		perror("fork");
		exit(EXIT_FAILURE);
		break;
	default:

		res = pwrite(fd,
				content_to_write, // indirizzo della zona di memoria da scrivere
				BUF_LEN/2, // lunghezza della zona di memoria
				0); // file offset da utilizzare per la scrittura su file

		printf("[parent] pwrite ok\n");

		res = wait(NULL);

		CHECK_ERR(res, "wait()")


	}


	// il descrittore

//	il processo padre crea un processo figlio: l'obiettivo finale è che il file output.txt
//	contenga tutta la stringa text_to_write e che i due processi (processo padre e processo figlio)
//	si dividano il lavoro di scrittura nel file. La scrittura sul file avviene in "parallelo".

//	il processo figlio apre per conto suo il file output.txt.



	return 0;
}




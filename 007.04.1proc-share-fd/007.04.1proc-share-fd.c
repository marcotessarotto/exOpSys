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

/*
 * processo padre crea un file e ne imposta la dimensione
 *
 * processo padre e processo figlio condividono il file descriptor e quindi il file offset
 *
 * viene mostrato come se uno dei processi modifica il file offset, anche l'altro processo vede la modifica
 *
 * (il file non viene scritto)
 */


#define CHECK_ERR(a,msg) {if ((a) == -1) { perror((msg)); exit(EXIT_FAILURE); } }

int main(int argc, char * argv[]) {

	char * file_name = "output.txt";
	int res;
	off_t file_offset;

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

	// il file output.txt è stato creato se non esisteva
	// la sua dimensione è stata impostata a BUF_LEN bytes
	printf("open file descriptor: %d\n", fd);


	// processo padre e processo figlio condividono lo stesso file descriptor
	// vediamo come il file offset sia condiviso tra i due processi

	printf("processo padre e processo figlio condividono lo stesso file descriptor\n"
			"il file offset verrà condiviso tra i due processi\n");

	file_offset = lseek(fd, 0, SEEK_CUR); // SEEK_CUR: posizione relativa rispetto alla posizione corrente

	CHECK_ERR(file_offset, "lseek()")

	printf("[parent] current file offset: %ld\n", file_offset);

	switch(fork()) {
	case 0:
		printf("[child] before lseek:\n");

		file_offset = lseek(fd, BUF_LEN/2, SEEK_SET); // cambiamo il file offset di fd=3
		// questa modifica è immediatamente visibile anche dal processo padre
		// il processo padre è bloccato su wait() ed aspetta la terminazione del processo figlio

		CHECK_ERR(file_offset, "lseek()")

		printf("[child] after lseek(): current file offset: %ld\n", file_offset);

		printf("[child] bye!\n");

		close(fd); // superfluo, perchè quando il processo termina, il kernel chiude i file aperti

		exit(EXIT_SUCCESS);
	case -1:
		perror("fork");
		exit(EXIT_FAILURE);
	default:
		if (wait(NULL) == -1) {
			perror("wait");
		} else {

			file_offset = lseek(fd, 0, SEEK_CUR);
			CHECK_ERR(file_offset, "lseek()")

			printf("[parent] current file offset: %ld\n", file_offset);
		}
	}


	printf("[parent] bye!\n");

	close(fd); // superfluo, perchè quando il processo termina, il kernel chiude i file aperti

	return 0;
}




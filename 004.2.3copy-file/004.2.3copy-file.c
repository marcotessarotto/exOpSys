#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_LEN 1024


int main(int argc, char * argv[]) {

	if (argc <= 3) {
	  printf("questo programma si aspetta tre parametri: nome del file sorgente ,"
			  "nome del file destinazione e (opzionale) dimensione del buffer in bytes\n");
	  exit(1);
	}

	int fd_source;
	int fd_target;

	int buffer_size = BUFFER_LEN;

	char * filename_source;
	char * filename_target;

	char * buffer;

	filename_source = argv[1];
	filename_target = argv[2];

	if (argc > 3) {
		buffer_size = atoi(argv[3]);

		if (buffer_size <= 0)
			buffer_size = BUFFER_LEN;
	}

	// definire due file descriptor, perchè apriremo un file in lettura ed uno in scrittura
	// aprire in sola lettura il file sorgente
	// se il file non esiste (fd_source == -1), messaggio di errore ed uscire

	// aprire il file destinazione (O_CREAT | O_TRUNC | O_RDWR)

	fd_source = open(filename_source, O_RDONLY);
	if (fd_source == -1) {
	  // se il file non esiste, finiamo qui
	  perror("open()");
	  exit(1);
	}

	fd_target = open(filename_target, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd_target == -1) {
	  perror("open()");
	  exit(1);
	}

	// introduciamo un #define BUFFER_LEN 512 per indicare la dimensione della zona di memoria
	// che verrà usata per copiare il contenuto del file sorgente (un po' alla volta)
	// allocare una zona di memoria con malloc()
	buffer = malloc(buffer_size);
	if (buffer == NULL) {
	  perror("malloc()");
	  exit(1);
	}


	int res;
	int bytes_totali_copiati = 0;
	int numero_cicli = 0;

	while ((res = read(fd_source, buffer, buffer_size)) > 0) {

	  numero_cicli++;

	  //printf("ho letto %d bytes\n", res);

	  bytes_totali_copiati += res;

	  res = write(fd_target, buffer, res);
	  if (res == -1) {
		  perror("write()");
		  exit(1);
	  }

	  //printf("ho scritto %d bytes\n", res);

	}

	if (res == -1) {
	  perror("read()");
	  exit(1);
	}

	printf("TOTALE: ho copiato %d bytes in %d cicli\n", bytes_totali_copiati, numero_cicli);


	return 0;
}

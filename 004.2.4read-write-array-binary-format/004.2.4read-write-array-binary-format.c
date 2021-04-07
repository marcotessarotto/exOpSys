#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



int main(int argc, char * argv[]) {


	int fd_target;
	char * filename_target = "prova.txt";
	int res;

	fd_target = open(filename_target, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd_target == -1) {
	  perror("open()");
	  exit(1);
	}



	// dato un array di int, salvarlo su file in questo modo:
	// scrivere tutti i numeri in formato "binario"
	// e come prima cosa scrivere il numero di celle
	// poi a seguire i dati dell'array

	int dimensione_array = 10;
	int esempio_array[10] = {1,2,3,4};

	// scrivo il numero di celle del vettore:
	res = write(fd_target, &dimensione_array, sizeof(dimensione_array));
	if (res == -1) {
		perror("write");
	}

	// poi scrivo i dati del vettore:
	for (int i = 0; i < dimensione_array; i++) {
		res = write(fd_target, &esempio_array[i], sizeof(int));
		if (res == -1) {
			perror("write");
		}
	}


	off_t current_file_offset;

	current_file_offset = lseek(fd_target, 0, SEEK_CUR); // ottengo il file offset corrente (dopo avere scritto 44 bytes)

	printf("file offset corrente: %ld\n", current_file_offset);

    // supponiamo di leggere i dati dal file (che abbiamo appena scritto)

	// 1 - riposizionare il file offset all'inizio del file

	current_file_offset = lseek(fd_target, 0, SEEK_SET); // impostiamo il file offset all'inizio del file

	printf("nuovo file offset corrente: %ld\n", current_file_offset);

	// ok siamo pronti a leggere il vettore

	// 1 - leggo la dimensione del vettore

	int dimensione_array_da_leggere;
	res = read(fd_target, &dimensione_array_da_leggere, sizeof(int));

	if (res != sizeof(int)) {
		printf("mi aspettavo sizeof(int) bytes...\n");
		exit(1);
	}

	if (dimensione_array_da_leggere < 0) {
		printf("problema: dimensione_array_da_leggere < 0\n");
		exit(1);
	}

	int * array_da_leggere;

	array_da_leggere = malloc(dimensione_array_da_leggere * sizeof(int));
	if (array_da_leggere == NULL) {
		perror("malloc()");
		exit(1);
	}

	for (int i = 0; i < dimensione_array_da_leggere; i++) {

		res = read(fd_target, &array_da_leggere[i], sizeof(int));

		if (res != sizeof(int)) {
			printf("mi aspettavo sizeof(int) bytes...\n");
			exit(1);
		}

	}

	printf("ho letto tutto il vettore dal file. dimensione del vettore: %d\n", dimensione_array_da_leggere);

	// per visualizzare i contenuti del file:

	// od -t x1 prova.txt


	return 0;

}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>



int main(int argc, char * argv[]) {


	int fd_target;
	char * filename_target = "prova.txt";
	int res;
	int strlen;

	fd_target = open(filename_target, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd_target == -1) {
	  perror("open()");
	  exit(1);
	}


	// dato un array di int, salvarlo su file in questo modo:
	// scrivere tutti i numeri in formato "testo"
	// e come prima cosa scrivere il numero di celle
	// poi a seguire i dati dell'array

	int dimensione_array = 10;
	int esempio_array[10] = {1,2,3,4,3475687, INT_MAX, INT_MIN};

	char buffer[32];

	// sprintf restituisce il numero di caratteri che ha prodotto
	// ipotesi: ogni numero scritto in formato testo occupa un numero costante di caratteri (11 + 1 per \n)
	strlen = sprintf(buffer, "%11d\n", dimensione_array);

	res = write(fd_target, buffer, strlen);
	if (res < strlen) {
		printf("problema con write()\n");
	}

	for (int i = 0; i < dimensione_array; i++) {
		strlen = sprintf(buffer, "%11d\n", esempio_array[i]);

		res = write(fd_target, buffer, strlen);
		if (res < strlen) {
			printf("problema con write()\n");
		}
	}

	// riposiziono il file offset

	off_t current_file_offset;


	// riposizionare il file offset all'inizio del file

	current_file_offset = lseek(fd_target, 0, SEEK_SET); // impostiamo il file offset all'inizio del file
	printf("nuovo file offset corrente: %ld\n", current_file_offset);

	// leggo i dati (che abbiamo appena scritto)

	// ogni riga del file ha dimensione fissa: 11+1
	// ogni riga è terminata da \n

	// se 'dim' è la dimensione dell'array di int da leggere,
	// dovremo leggere (dim+1) * 12 caratteri (nel caso peggiore)

	char * read_buffer;
	int read_buffer_len = 11+1;

	read_buffer = malloc(read_buffer_len);

	res = read(fd_target, read_buffer, read_buffer_len);
	if (res <= 0) {
		exit(1);
	}

	//read_buffer[read_buffer_len-1] = 0;

	//printf("ho letto questi caratteri: %s\n", read_buffer);

	int read_array_dim;

	res = sscanf(read_buffer,"%d\n", &read_array_dim);

	if (res != 1) {
		printf("non ho trovato la dimensione del vettore");
		exit(1);
	}

	printf("ho letto la dimensione del vettore: %d\n", read_array_dim);

	if (read_array_dim < 0) {
		printf("read_array_dim non può essere < 0\n");
		exit(1);
	}

	int * read_array;

	read_array = malloc(read_array_dim * sizeof(int));
	if (read_array == NULL) {
		exit(1);
	}

	// conosco la dimensione del vettore
	// leggo tutti gli elementi del vettore

	for (int i = 0; i < read_array_dim; i++) {

		res = read(fd_target, read_buffer, read_buffer_len);
		if (res <= 0) {
			exit(1);
		}

		res = sscanf(read_buffer,"%d\n", &read_array[i]);

		if (res != 1) {
			printf("non ho trovato il valore della cella i-ma\n");
			exit(1);
		}

	}

	// ho finito di leggere il vettore, scriviamo su stdout i dati che ho letto

	for (int i = 0; i < read_array_dim; i++) {

		printf("read_array[%d] = %d\n", i, read_array[i]);

	}


	return 0;
}

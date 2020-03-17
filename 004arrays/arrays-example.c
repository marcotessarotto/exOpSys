#include <stdio.h>
#include <stdlib.h>

/*
 * chiede all'utente di scrivere 10 numeri interi.
 * una volta letti i dati, li scrive su standard output
 */
void read_static_array() {

	// static array
	// ogni elemento è inizializzato a 0
	int numbers[10] = { 0 };

	//printf("dimensione totale dell'array numbers: %ld\n", sizeof(numbers));

	printf("scrivi 10 numeri interi: ");

	for (int i=0; i < 10; i++) {

		scanf("%d", &numbers[i]);

	}

	printf("\n");

	for (int i=0; i < 10; i++) {
		printf("numbers[%u]=%d\n", i, numbers[i]);
	}

}

void read_dynamic_array(unsigned int array_size) {

	int * numbers;

	numbers = calloc(array_size, sizeof(int));

	// esercizio 1: scrivi la dimensione in bytes dell'array dinamico

	// esercizio 2: completare:

	// a - leggi i valori da stdin

	// b - scrivi i valori letti

	free(numbers);

}

int main() {

	int first_array[3] = { 0x123, 0x1234, 0x12345 };
	// ogni cella dell'array ha dimensione (in bytes) ...
	// gli indici validi di first_array sono: 0, 1, 2
	// dimensione totale dell'array in bytes: ....

	/*

                        Big endian (Intel, AMD)
       +----+----+----+----+----+----+----+----+----+----+----+----+
       |0x00|    |    |    |    |    |    |    |    |    |    |    |
       +----+----+----+----+----+----+----+----+----+----+----+----+
 byte     0    1    2    3   4     5    6    7    8    9   10   11
 address
						Little endian (ARM)
       +----+----+----+----+----+----+----+----+----+----+----+----+
       |    |    |    |    |    |    |    |    |    |    |    |    |
       +----+----+----+----+----+----+----+----+----+----+----+----+
 byte     0    1    2    3   4     5    6    7    8    9   10   11
 address


	 */

	int second_array[5] = {2, 1, 0};
	// ogni cella dell'array ha dimensione (in bytes) ...
	// gli indici validi di second_array sono: 0, 1, ....
	// dimensione totale dell'array in bytes: ....

	short int third_array[10];
	// ogni cella dell'array ha dimensione (in bytes) ...
	// gli indici validi di second_array sono: ....
	// dimensione totale dell'array in bytes: ....

	char fourth_array[1024];
	// ogni cella dell'array ha dimensione (in bytes) ...
	// gli indici validi di second_array sono: ....
	// dimensione totale dell'array in bytes: ....


	first_array[0] = -1;

	// non viene fatto un controllo sugli indici quando accedo all'array,
	// dipende dal programma

	// provare!!! se messaggio di errore riporta un signal,
	// andare a vedere: man 7 signal
	//first_array[100000] = 0XDEADBEEF; // errore! 1000 è un indice NON valido

	int z = first_array[2] + first_array[0];
	// quanto vale z? ....

	if (1) {

		read_static_array();
	}

	if (0) {
		read_dynamic_array(5);
	}

	if (0) {
		unsigned int n;

		scanf("quanti valori sono da leggere? %d", &n);


		read_dynamic_array(n);
	}


	return 0;
}

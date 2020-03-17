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

	printf("dimensione totale dell'array numbers: %ld\n", sizeof(numbers));

	printf("scrivi 10 numeri interi: ");

	for (int i=0; i < 10; i++) {

		scanf("%d", &numbers[i]);

	}

	printf("\n");

	for (unsigned int i=0; i < 10; i++) {
		printf("numbers[%u]=%d\n", i, numbers[i]);
	}

}

void read_dynamic_array(unsigned int array_size) {

	int * numbers;

	numbers = calloc(array_size, sizeof(int));
	// prova ad allocare una zona di memoria di dimensione totale (bytes)
	// array_size * sizeof(int)
	// e la inizializza a zero

	// in Java:
	// int [] numbers = new int[array_size];



	// ESERCIZIO 1: scrivi la dimensione in bytes dell'array dinamico
	//

	// ESERCIZIO 2: completare:

	// a - leggi i valori da stdin
	// ......

	// b - scrivi i valori letti
	// ......

	free(numbers);

}

int main() {

	int first_array[3] = { 0x123, 0x1234, 0x12345 };
	// ogni cella dell'array ha dimensione (in bytes) : 4 bytes
	// gli indici validi di first_array sono: 0, 1, 2
	// dimensione totale dell'array in bytes: 12 bytes

	int var1 = 0x00000123;

	/*
	 * 00 00 01 23
	 *
	   ESERCIZIO: completare la rappresentazione in memoria degli array

                        Big endian (Intel, AMD)
       +----+----+----+----+----+----+----+----+----+----+----+----+
       |0x00|0x00|0x01|0x23|0x00|0x00|0x12|0x34|0x00|0x01|0x23|0x45|
       +----+----+----+----+----+----+----+----+----+----+----+----+
 byte     0    1    2    3   4     5    6    7    8    9   10   11
 address
						Little endian (ARM)
       +----+----+----+----+----+----+----+----+----+----+----+----+
       |0x23|0x01|0x00|0x00|0x34|0x12|0x00|0x00|0x45|0x23|0x01|0x00|
       +----+----+----+----+----+----+----+----+----+----+----+----+
 byte     0    1    2    3   4     5    6    7    8    9   10   11
 address


	 */

	int second_array[5] = {2, 1, 0}; // equivalente a {2,1,0,0,0}
	// ogni cella dell'array ha dimensione (in bytes) 4 bytes
	// gli indici validi di second_array sono: 0, 1, 2, 3, 4
	// dimensione totale dell'array in bytes: 4 * 5 = 20 bytes

	second_array[4] = 123123; // le celle dell'array sono pronte per l'uso

	short int third_array[10]; // le celle dell'array hanno valori casuali
	// ogni cella dell'array ha dimensione (in bytes) : 2 bytes
	// gli indici validi di third_array sono: [0, 9]
	// dimensione totale dell'array in bytes: 20 bytes

	char fourth_array[1024];
	// ogni cella dell'array ha dimensione (in bytes) 1 byte
	// gli indici validi di fourth_array sono: [0, 1023]
	// dimensione totale dell'array in bytes: 1024 bytes


	first_array[0] = -1;

	// non viene fatto un controllo sugli indici quando accedo all'array,
	// dipende dal programmatore

	// provare!!! se messaggio di errore riporta un signal,
	// andare a vedere: man 7 signal
	// first_array[100000] = 0XDEADBEEF; // errore! 100000 è un indice NON valido

	int z = first_array[2] + first_array[0];
	// quanto vale z? 0x12344

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

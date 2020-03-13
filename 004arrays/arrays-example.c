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

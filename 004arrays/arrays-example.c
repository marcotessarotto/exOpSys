#include <stdio.h>
#include <stdlib.h>

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

void read_dynamic_array(int array_size) {

	int * numbers;

	numbers = calloc(array_size, sizeof(int));

	// esercizio: scrivi la dimensione in bytes dell'array dinamico

	// esercizio: completare
	// ..... leggi i valori da stdin

	// scrivi i valori letti

	free(numbers);

}

int main() {

	read_static_array();

	int n;

	//scanf("valore di array_size: %d", &n);

	//read_dynamic_array(n);

	return 0;
}

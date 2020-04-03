#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {

	// ESERCIZIO 2:

	// scrivere il seguente programma:

	// legge e salva (in memoria) tutti i dati provenienti da stdin fino a EOF

	int ch;
	char * array;
	unsigned int counter = 0;

	array = malloc(sizeof(char));

	if (array == NULL) {
		perror("malloc()");
		exit(EXIT_FAILURE);
	}

	while ((ch = getchar()) != EOF) {

		array[counter] = ch;

		counter++;

		//printf("counter=%d\n", counter);
		array = realloc(array, (counter+1) * sizeof(char));

		if (array == NULL) {
			perror("realloc()");
			exit(EXIT_FAILURE);
		}

	}

	printf("caratteri letti=%u\n", counter);

	// poi scrive su stdout i dati accumulati ma all'incontrario (dall'ultimo al primo)

	printf("ecco i contenuti all'incontrario:\n");

	for (int i = counter-1; i >= 0; i--) {
		//printf("i=%d\n", i);
		putchar(array[i]);
	}

	free(array); // non necessario

	return 0;
}

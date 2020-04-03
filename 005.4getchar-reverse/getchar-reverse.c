#include <stdio.h>
#include <stdlib.h>

#include "stretchy_buffer.h"

void alternative_solution() {


	//       SomeStruct *arr = NULL;
	//       while (something)
	//       {
	//          SomeStruct new_one;
	//          new_one.whatever = whatever;
	//          new_one.whatup   = whatup;
	//          new_one.foobar   = barfoo;
	//          sb_push(arr, new_one);
	//       }

	int ch;
	char *arr = NULL;
	unsigned int counter = 0;

	while ((ch = getchar()) != EOF) {

		sb_push(arr, ch);

		counter++;
	}


	printf("caratteri letti=%u\n", counter);

	// poi scrive su stdout i dati accumulati ma all'incontrario (dall'ultimo al primo)

	printf("ecco i contenuti all'incontrario:\n");

	for (int i = counter-1; i >= 0; i--) {
		//printf("i=%d\n", i);
		putchar(arr[i]);
	}

	sb_free(arr);


}


int main(int argc, char *argv[]) {

	if (1) {

		// utilizza:
		// stretchy_buffer.h - v1.04 - public domain - nothings.org/stb
		// a vector<>-like dynamic array for C

		// https://github.com/nothings/stb/blob/master/stretchy_buffer.h

		alternative_solution();

		return 0;
	}

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>


// esempio:
// una funzione restituisce un array (e la sua dimensione)


// typedef: Kernighan&Ritchie pag. 146
// struct: Kernighan&Ritchie pag. 127

typedef struct {
	char * data;
	unsigned int length;
} array_result;


array_result example_function() {

	array_result result;

	result.data = calloc(10, sizeof(int));

	for (int i = 0; i < 10; i++)
		result.data[i] = i;

	result.length = 10;

	return result;
}



int main(int argc, char * argv[]) {


	array_result ar = example_function();

	printf("example_function ha restituito un array di lunghezza %u\n", ar.length);

	// lavoriamo con l'array ar.data

	printf("utilizziamo l'array ricevuto:\n");
	// ad esempio, lo stampiamo...

	for (int i = 0; i < ar.length; i++) {
		printf("ar.data[%i] = %i\n", i, ar.data[i]);
	}

	// una volta finito di utilizzare l'array, liberiamo la memoria
	free(ar.data);

	// tutte le allocazioni di memoria ancora attive vengono liberate automaticamente alla fine del programma
	return EXIT_SUCCESS;
}

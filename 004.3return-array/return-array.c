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

/*
 * una struttura (struct) è una collezione di una o più variabili, anche di tipo diverso,
 * raggruppate assieme sotto un nome unico (il nome della 'struct')
 *
 *
 * nell'esempio sotto, la struttura è anonima ma
 * viene definito un nuovo tipo dati ('char_array_type') attraverso il comando typedef
 */

typedef
	struct {
		char * data;
		unsigned int length;
	} char_array_type;


char_array_type example_function() {

	char_array_type result;

	result.data = calloc(10, sizeof(char));

	for (int i = 0; i < 10; i++)
		result.data[i] = i; // conversione implicita da int a char

//	for (char i = 0; i < 10; i++)
//			result.data[i] = i; // nessuna conversione: char è assegnato a char

	// result.data[9] = 9;
	// rappresentazione in memoria di (int) 9 è:
	// 0x09 00 00 00

	result.length = 10;

	return result;
}



int main(int argc, char * argv[]) {

	char_array_type ar = example_function();

	printf("example_function ha restituito un array di lunghezza %u\n\n", ar.length);

	// lavoriamo con l'array ar.data

	printf("utilizziamo l'array ricevuto:\n");
	// ad esempio, lo stampiamo...

	for (int i = 0; i < ar.length; i++) {
		printf("ar.data[%i] = %i\n", i, ar.data[i]);
	}

	// una volta finito di utilizzare l'array, liberiamo la memoria
	free(ar.data);

	// ma non possiamo liberare ar, perchè ar è una variabile locale e verrà distrutta automaticamente alla fine della funzione

	// tutte le allocazioni di memoria ancora attive vengono liberate automaticamente alla fine del programma
	return EXIT_SUCCESS;
}

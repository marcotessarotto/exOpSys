#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// allocare (creare) un array
// liberare (distruggere) un array
// ridimensionare un array
// copiare un array
// accedere ad array

// concatenare un array

// reverse array

// sort di array

// sottoinsieme di array
// cercare in un array

// array di array

void * make_copy_of_array(void * src_array, unsigned int array_length);

int main(int argc, char *argv[]) {

	int size = 10;

	char * char_array; // char_array è un puntatore a char
	// Kernighan&Ritchie pag. 93
	// un puntatore è una variabile che contiene un indirizzo di (un'altra) variabile


	char_array = NULL;

	// allocare (creare) un array
	// ogni cella ha dimensione sizeof(char)
	// l'array ha 'size' celle
	char_array = calloc(size, sizeof(char)); // inizializza a zero la memoria allocata
	// oppure
	// char_array = malloc(size * sizeof(char));

	// controllare se calloc ha funzionato:
	if (char_array == NULL) {
		perror("calloc error!");
		exit(EXIT_FAILURE);
	}

	printf("indirizzo contenuto in char_array: %p\n", char_array);

	// liberare (distruggere) un array
	free(char_array);

	// attenzione! char_array contiene lo stesso valore di prima ma
	// non punta più ad una zona di memoria allocata!

	/////////////////////////////////////////

	int * int_array;
	// allocare (creare) un array
	int_array = malloc(size * sizeof(int)); // dimensione totale in bytes dell'array?

	size = size * 100;

	// ridimensionare un array
	int_array = realloc(int_array, size * sizeof(int));

	if (int_array == NULL) {
		perror("realloc error!");
		exit(EXIT_FAILURE);
	}

	free(int_array);

	/////////////////////////////////////////

	long * array_a;
	long * copia_array;
	size = 20;

	array_a = calloc(size, sizeof(char)); // array_a non viene inizializzato da calloc

	// che valori ha array_a?
	memset(array_a, 0, size * sizeof(char)); // inizializziamo array_a (ogni cella avrà il valore 0)

	copia_array = make_copy_of_array(array_a, size * sizeof(char));

	//
	free(copia_array);

	// accediamo agli elementi dell'array:

	array_a[0] = 1;
	array_a[1] = 0xFFFF;
	// ...





	return EXIT_SUCCESS;
}

/*
 * crea una copia dell'array
 * parametri: indirizzo dell'array, dimensione totale in bytes dell'array
 * restituisce l'indirizzo della copia
 */
// il tipo dati 'void *' indica un puntatore 'generico'
void * make_copy_of_array(void * src_array, unsigned int array_total_size) {

	if (src_array == NULL || array_total_size < 0)
		return NULL;

	void * result;

	// allochiamo la memoria della copia
	result = malloc(array_total_size);

	memcpy(result, src_array, array_total_size);

	// invece di memcpy potrei fare così:
//	char * dest = result;
//	char * src = src_array;
//	for (unsigned int i = 0; i < array_total_size; i++) {
//		dest[i] = src[i];
//	}

	// chi riceve il risultato, dovrà occuparsi di liberare la memoria allocata per la copia dell'array
	return result;
}



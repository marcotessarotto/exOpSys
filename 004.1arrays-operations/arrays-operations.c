#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// allocare (creare) un array (OK)
// liberare (distruggere) un array (OK)
// ridimensionare un array (OK)
// copiare un array (OK)
// accedere ad array (OK)

// concatenare un array (ESERCIZIO)

// reverse array (OK)

// sort di array (ESERCIZIO)

// sottoinsieme di array (TODO)
// cercare in un array (TODO)

// array di array (TODO)

// dichiarazione di funzione definita dopo main()
long * make_copy_of_array(long * src_array, unsigned int array_dimension);

void * make_copy_of_array_generic(void * src_array, unsigned int array_total_size);

void swap(char *x, char *y);
char * reverse(char * array, unsigned int i, unsigned int j);
char * complete_reverse(char * array, unsigned int array_len);

char * concat_arrays(char * array1, int array1_dimension, char * array2, int array2_dimension);


int main(int argc, char *argv[]) {

	int size = 10;

	char * char_array; // char_array è un puntatore a char
	// Kernighan&Ritchie pag. 93
	// un puntatore è una variabile che contiene un indirizzo di (un'altra) variabile


	char_array = NULL;

	// cosa succede?
	//*char_array = 'A'; // WRONG! perchè? (in quale indirizzo di memoria va a scrivere?)

	//char_array = 'A'; // WRONG! perchè?

	// allocare (creare) un array
	// ogni cella ha dimensione sizeof(char)
	// l'array ha 'size' celle
	char_array = calloc(size, sizeof(char)); // inizializza a zero la memoria allocata
	// oppure
	//char_array = malloc(size * sizeof(char)); // NON inizializza a zero la memoria allocata

	// NOTA: malloc, calloc e free richiedono:
	// #include <stdlib.h>

	// è necessario controllare se calloc ha funzionato:
	if (char_array == NULL) {
		perror("calloc error!");
		exit(EXIT_FAILURE);
	}

	// esempio: allochiamo 100 GB di memoria:

	// vedere:
	// man 3 malloc

	// void *malloc(size_t size);
	// size_t è 'unsigned long'

	//printf("sizeof(size_t)= %lu\n", sizeof(size_t));

	// nota:
	// perchè scrivo "100 * 1024 * 1024 * 1024L" (con la L sull'ultimo 1024) ?
	// se scrivessi "100 * 1024 * 1024 * 1024" cosa cambia? (provate...)

	char * crash_test = malloc(100 * 1024 * 1024 * 1024L);

	if (crash_test == NULL) {
		printf("richiesta malloc(100 GB): fallita!\n");
	} else {
		printf("malloc(100 GB) ok!\n");
	}

	free(crash_test); // se passo NULL a free è ok, non succede nulla (vedere man free)


	printf("indirizzo contenuto in char_array: %p\n", char_array);

	// liberare (distruggere) un array
	free(char_array);
	//char_array = NULL;

	// attenzione! char_array contiene lo stesso valore di prima ma
	// non punta più ad una zona di memoria allocata! non bisogna utilizzarla più!

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

	array_a = malloc(size * sizeof(long)); // array_a non viene inizializzato da malloc

	// che valori ha array_a?
	memset(array_a, 0, size * sizeof(long)); // inizializziamo array_a (ogni cella avrà il valore 0)

	copia_array = make_copy_of_array(array_a, size * sizeof(long)); // importante!!!

	//
	free(copia_array);

	// accediamo agli elementi dell'array:

	array_a[0] = 1;
	array_a[1] = 0xFFFF;
	// ...

	return EXIT_SUCCESS;
}


/*
 * crea una copia dell'array di long
 * parametri: indirizzo dell'array, dimensione dell'array (numero di celle)
 * restituisce l'indirizzo della copia
 */
long * make_copy_of_array(long * src_array, unsigned int array_dimension) {

	/*
	 * in Java, quando passo un array come argomento di una funzione, oltre al riferimento all'oggetto
	 * viene passato "automaticamente" anche la lunghezza dell'array.
	 *
	 * in C l'array non si porta dietro "automaticamente" la lunghezza di se stesso:
	 * bisogna passare l'indirizzo dell'array E la lunghezza dell'array, in due parametri distinti
	 */

	if (src_array == NULL || array_dimension < 0)
		return NULL;

	long * result;

	// allochiamo lo spazio di memoria per la copia
	result = malloc(array_dimension * sizeof(long));

	memcpy(result, src_array, array_dimension * sizeof(long));

	// invece di memcpy potrei fare così:
//	long * dest = result;
//	long * src = src_array;
//	for (unsigned int i = 0; i < array_dimension; i++) {
//		dest[i] = src[i];
//	}

	// IMPORTANTE: chi riceve il risultato, dovrà occuparsi di liberare la memoria allocata per la copia dell'array
	return result;
}



/*
 * crea una copia dell'array
 * parametri: indirizzo dell'array, dimensione totale in bytes dell'array
 * restituisce l'indirizzo della copia
 *
 * il tipo dati 'void *' indica un puntatore 'generico'
 */
void * make_copy_of_array_generic(void * src_array, unsigned int array_total_size) {

	/*
	 * in Java, quando passo un array come argomento di una funzione, oltre al riferimento all'oggetto
	 * viene passato "automaticamente" anche la lunghezza dell'array.
	 *
	 * in C l'array non si porta dietro "automaticamente" la lunghezza di se stesso:
	 * bisogna passare l'indirizzo dell'array E la lunghezza dell'array, in due parametri distinti
	 */

	if (src_array == NULL || array_total_size < 0)
		return NULL;

	void * result;

	// allochiamo la memoria della copia
	result = malloc(array_total_size);

	memcpy(result, src_array, array_total_size);

	// chi riceve il risultato, dovrà occuparsi di liberare la memoria allocata per la copia dell'array
	return result;
}


// function to swap two char
void swap(char *x, char *y) {
	char t = *x;
	*x = *y;
	*y = t;
}

// function to reverse array[i..j]
char * reverse(char * array, unsigned int i, unsigned int j)
{
	while (i < j)
		swap(&array[i++], &array[j--]);

	return array;
}

char * complete_reverse(char * array, unsigned int array_len) {
	return reverse(array, 0, array_len);
}


char * concat_arrays(char * array1, int array1_dimension, char * array2, int array2_dimension) {

	// ESERCIZIO:


	// allocare lo spazio per un new_array, numero di celle = array1_dimension + array2_dimension



	// copiare array1 in new_array, a partire da nuovo_array[0]



	// copiare array2 in new_array, a partire da nuovo_array[array1_dimension]


	// restituire new_array

}


/*
 * la funzione bubble_sort ordina l'array passato come parametro
 * array_dimension: numero di celle dell'array
 */
void bubble_sort(int * array, int array_dimension) {

	// ESERCIZIO: implementare bubble sort (pseudocodice riportato sotto

/*

procedure bubbleSort(A : list of sortable items)
    n := length(A)
    repeat
        newn := 0
        for i := 1 to n - 1 inclusive do
            if A[i - 1] > A[i] then
                swap(A[i - 1], A[i])
                newn := i
            end if
        end for
        n := newn
    until n ≤ 1
end procedure
*/

}

// insertion sort (TBC)



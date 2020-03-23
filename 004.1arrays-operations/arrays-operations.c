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


/*

 lo standard C99 prevede 'variable-length array' (VLA) , ad esempio:

float read_and_process(int n)
{
    float vals[n];

    for (int i = 0; i < n; ++i)
        vals[i] = read_val();

    return process(n, vals);
}

 considerare però:

 "The Linux Kernel Is Now VLA-Free: A Win For Security, Less Overhead & Better For Clang"
 https://www.phoronix.com/scan.php?page=news_item&px=Linux-Kills-The-VLA

 da C11, 'variable-length array' diventa opzionale

 ***
 Linux Kernel viene tipicamente compilato con gcc.

 gcc (GNU Compiler Collection) include il compilatore C, GNU C library (più altre cose);
 è sviluppato dal Progetto GNU promosso dalla Free Software Foundation. Open source.

 clang include il compilatore C. Pensato come rimpiazzo di gcc, promosso da Apple, Microsoft ed altri. Open source.


 */

#define ARRAY_SIZE 10

// dichiarazioni di funzione definite dopo main()
long * make_copy_of_array(long src_array [], unsigned int array_dimension);
void * make_copy_of_array_generic(void * src_array, unsigned int array_total_size);

void swap(char *x, char *y);
char * reverse(char * array, unsigned int i, unsigned int j);
char * complete_reverse(char * array, unsigned int array_len);

int * complete_reverse_int(int * array, unsigned int array_len); // ESERCIZIO

char * concat_arrays(char * array1, int array1_dimension, char * array2, int array2_dimension);

void bubble_sort(int * array, int array_dimension);


int main(int argc, char *argv[]) {

	int size = 10;

	char * char_array; // char_array è un puntatore a char
	// Kernighan&Ritchie pag. 93
	// un puntatore è una variabile che contiene un indirizzo di (un'altra) variabile


	char_array = NULL;

	// ESEMPIO: allocare array con calloc (o malloc),
	// controllare il risultati di calloc/malloc
	// utilizzare l'array,
	// liberare/distruggere l'array

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

	// utilizzo di char_array ...

	printf("indirizzo contenuto in char_array: %p\n", char_array);

	// liberare (distruggere) un array
	free(char_array);
	//char_array = NULL;
	// attenzione! char_array contiene lo stesso valore di prima ma
	// non punta più ad una zona di memoria allocata! non bisogna utilizzarla più!


	/////////////////////////////////////////

	//alloc_test(); // spostato in progetto 004.7alloca-test

	/////////////////////////////////////////

	// ESEMPIO: allocazione array,
	// ridimensionamento array,
	// distruggere array

	size = 10;

	int * int_array;
	// allocare (creare) un array
	int_array = malloc(size * sizeof(int)); // dimensione totale in bytes dell'array?

	int_array[0] = 1024;

	size = size * 100; // la nuova dimensione di int_array sarà 1000

	// ridimensionare un array
	int_array = realloc(int_array, size * sizeof(int));

	if (int_array == NULL) {
		perror("realloc error!");
		exit(EXIT_FAILURE);
	}

	// quanto vale int_array[0] dopo il ridimensionamento?
	printf("int_array[0] = %d\n", int_array[0]);

	free(int_array);

	/////////////////////////////////////////

	// ESEMPIO: allocare array, fare una copia dell'array

	long * array_a;
	long * copia_array;
	size = 20;

	array_a = malloc(size * sizeof(long)); // array_a non viene inizializzato da malloc
	// dovrei verificare che sia non NULL ...

	long l = array_a[0]; // cosa trovo? c'è sicuramente un valore, ma è random

	// che valori ha array_a dopo memset?
	// man 3 memset
	memset(array_a, 0, size * sizeof(long)); // inizializziamo array_a (ogni cella avrà il valore ...)

	copia_array = make_copy_of_array(array_a, size * sizeof(long)); // importante!!!

	// operiamo su copia_array
	// quando abbiamo finito, la liberiamo

	free(copia_array);

	// accediamo agli elementi dell'array:

	array_a[0] = 1;
	array_a[1] = 0xFFFF;
	// ...
	free(array_a);


	/////////////////////////////////////////
	// ESEMPIO: reverse array (di array di char)

	// reverse array (inversione dell'array) di un array di lunghezza n (= numero di celle)
	// la cella 0 viene scambiata con la cella n-1
	// la cella 1 viene scambiata con la cella n-2
	// la cella 2 viene scambiata con la cella n-3
	// ...


	int i;

	char_array = calloc(ARRAY_SIZE, sizeof(char));

	if (char_array == NULL) {
		perror("calloc error");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < ARRAY_SIZE; i++) {
		char_array[i] = 'A' + i;
	}


	printf("contenuto dell'array char_array:\n");
	for (i = 0; i < ARRAY_SIZE; i++) {
		printf("'%c'(%d) ", char_array[i], char_array[i]);
	}
	printf("\n");

	complete_reverse(char_array, ARRAY_SIZE);

	printf("contenuto dell'array char_array dopo complete_reverse():\n");
	for (i = 0; i < ARRAY_SIZE; i++) {
		printf("'%c'(%d) ", char_array[i], char_array[i]);
	}
	printf("\n");

	free(char_array);

	////////
	// ESERCIZIO: scrivere una funzione 'complete_reverse_int' che
	// inverte le celle di un array di int
	// la funzione va definita più sotto, dopo complete_reverse


	int int_array_2[ARRAY_SIZE] = { 1, 2, 3, 4, 5 };

	printf("contenuto dell'array int_array_2:\n");
	for (i = 0; i < ARRAY_SIZE; i++) {
		printf("%d ", int_array_2[i]);
	}
	printf("\n");

	// ESERCIZIO: scrivere la funzione (definizione più sotto)
	complete_reverse_int(int_array_2, ARRAY_SIZE);

	for (i = 0; i < ARRAY_SIZE; i++) {
		printf("%x ", int_array_2[i]);
	}
	printf("\n");


	// ESERCIZIO: completare la funzione concat_arrays
	// esempio con concat di due array

	char char_array_1 [] = {'p', 'r', 'o', 'v', 'a'};
	char char_array_2 [] = { 'O', 'K' , '\0'};

	char * prova_stringa = "provaOK";

    char * concat_char_array;

    // restituisce una concatenazione dei due array
    concat_char_array = concat_arrays(char_array_1, sizeof(char_array_1),
    		char_array_2, sizeof(char_array_2)
    		);

    printf("risultato di concat_arrays:\n%s\n", concat_char_array);
    printf("scrivo prova_stringa:\n%s\n", prova_stringa);



    // ESERCIZIO PER CASA:
	// esempio con bubble sort

    // https://www.random.org/integer-sets/?sets=1&num=100&min=-2000&max=2000&seqnos=on&commas=on&order=index&format=html&rnd=new

    int rnd_array_int [] = {
    		-1958, 140, 607, 1447, -22, -1400, -1748, -1150, 1623, -971, 891, -94, 1805, 105, 814, 1286, -1791, 359, -1277, -520, -1535, 663, -533, -383, -979, -694, -748, 347, 495, -169, -157, -751, 1302, -1307, -405, 311, 595, 1344, -604, -262, 777, 779, 1491, 13, 1333, 357, -240, -197, 685, 1309, 195, -207, -1545, 601, -1227, 1411, 172, 1487, 572, -1268, 1719, 1276, -1943, -352, -1366, -263, -1955, 118, -1407, -582, -1229, 193, 1818, -188, -594, 688, 1534, -1458, 1455, -1623, -1167, 1208, -1383, 1022, -1465, -1586, -300, 851, 515, 1959, 659, 486, 1039, 115, -1063, -591, 566, -1609, 655, -1410
    };

    // sizeof(rnd_array_int) restituisce la dimensione totale dell'array,
    // lo può fare perchè il numero di celle dell'array è noto

    int rnd_array_int_len = sizeof(rnd_array_int) / sizeof(int); // numero di celle dell'array

    // SCRIVERE bubble_sort (definizione più sotto)
    bubble_sort(rnd_array_int, rnd_array_int_len);

    printf("risultato di bubble_sort():\n");

    for (int i = 0; i < rnd_array_int_len; i++) {
    	printf("%d ", rnd_array_int[i]);
    }

    printf("\n");

	// quando il programma termina, tutte le allocazioni di memoria sono automaticamente liberate
	return EXIT_SUCCESS;
}


/*
 * crea una copia dell'array di long
 * parametri: indirizzo dell'array, dimensione dell'array (numero di celle)
 * restituisce l'indirizzo della copia
 */
long * make_copy_of_array(long src_array [] /* long * src_array */, unsigned int array_dimension) {

	/*
	 * in Java, quando passo un array come argomento di una funzione, oltre al riferimento all'oggetto
	 * viene passato "automaticamente" anche la lunghezza dell'array.
	 *
	 * in C l'array non si porta dietro "automaticamente" la lunghezza di se stesso:
	 * bisogna passare l'indirizzo dell'array E la lunghezza dell'array, in due parametri distinti
	 */

	if (src_array == NULL)
		return NULL;

	long * result;

	// allochiamo lo spazio di memoria per la copia
	result = malloc(array_dimension * sizeof(long));

	if (result == NULL) {
		return NULL;
	}

	memcpy(result, src_array, array_dimension * sizeof(long));

	// invece di memcpy potrei fare così:
//	long * destination = result;
//	long * source = src_array;
//	for (unsigned int i = 0; i < array_dimension; i++) {
//		destination[i] = source[i];
//	}

	// IMPORTANTE: chi riceve il risultato, dovrà occuparsi di liberare
	// la memoria allocata per la copia dell'array
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
	// nota bene: le aree di memoria sorgente e destinazione
	// NON si devono sovrapporre.

	// in caso di sovrapposizione di memoria sorgente e destinazione,
	// usare memmove

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
//	while (i < j)
//		swap(&array[i++], &array[--j]);

	while (i < j) {
		j--;
		swap(&array[i], &array[j]);
		i++;
	}

	return array;
}

char * complete_reverse(char * array, unsigned int array_len) {
	return reverse(array, 0, array_len);
}

void swap_int(int *x, int *y) {
	int t = *x;
	*x = *y;
	*y = t;
}

int * reverse_int(int * array, unsigned int i, unsigned int j)
{
	while (i < j)
		swap_int(&array[i++], &array[--j]);

	return array;
}

int * complete_reverse_int(int * array, unsigned int array_len) {
	return reverse_int(array, 0, array_len);
}

/*
 * concat_arrays prende in ingresso due array (array1 e array2)
 * produce un nuovo array che è il risultato della concatenazione tra i due array passati
 *
 * concatenare array
 * { 0, 1, 2} e { 10, 11, 12, 13}
 * concat di array è { 0, 1, 2, 10, 11, 12, 13}
 *
 */
//char * concat_arrays(char * array1, int array1_dimension, char * array2, int array2_dimension) {
//
//	// ESERCIZIO: seguire le specifiche e completare la funzione
//
//
//	// allocare lo spazio per un new_array, numero di celle = array1_dimension + array2_dimension
//
//
//
//	// copiare array1 in new_array, a partire da nuovo_array[0]
//    // for qui
//
//
//	// copiare array2 in new_array, a partire da nuovo_array[array1_dimension]
//
//
//	// restituire new_array
//
//}

char *concat_arrays(char *array1, int array1_dim, char *array2, int array2_dim) {

	char *new_array;

	int tot_len = array1_dim + array2_dim;

	new_array = malloc(sizeof(char)*tot_len);

	if (new_array == NULL) {
		perror("malloc ha restituito errore");
		return NULL;
	}

	for (int i=0; i<array1_dim; i++) {
		new_array[i] = array1[i];
	}

	for (int j=array1_dim; j<tot_len; j++) {
		new_array[j] = array2[j - array1_dim];
	}

	return new_array;
}


/*
 * la funzione bubble_sort ordina l'array passato come parametro
 * array_dimension: numero di celle dell'array
 */
void bubble_sort(int * array, int array_dimension) {

	// ESERCIZIO: implementare bubble sort (pseudocodice riportato sotto)




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



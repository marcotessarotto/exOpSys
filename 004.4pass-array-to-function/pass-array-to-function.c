#include <stdio.h>
#include <stdlib.h>

int calculate_average_status = 0; // variabile globale
// è accessibile da tutte le funzioni
// se vale 0 al ritorno da calculate_average => tutto OK
// se diverso da 0 => ERRORE

double calculate_average(int * data, int data_len) {
	/*
	 * in Java, quando passo un array come argomento di una funzione, oltre al riferimento all'oggetto
	 * viene passato "automaticamente" anche la lunghezza dell'array.
	 *
	 * in C l'array non si porta dietro "automaticamente" la lunghezza di se stesso:
	 * bisogna passare l'indirizzo dell'array E la lunghezza dell'array, in due parametri distinti
	 */

	double d = 0.0;

	if (data == NULL) { // condizione di errore
		printf("data is NULL!!!\n");

		calculate_average_status = 1;

		// come faccio a comunicare al chiamante lo stato di errore?
		// in C non ci sono le eccezioni...
		// 0 potrebbe essere un risultato valido
		return 0;
	}

	if (data_len <= 0) { // condizione di errore

		calculate_average_status = 1;

		// cosa restituisco?? devo comunicare che non posso calcolare la media
		return 0;
	}

	for (int i = 0; i < data_len; i++) {
		d = d + data[i]; // conversione implicita da int a double:
		//d = d + (double)data[i];
	}

	// precedenza operatori C:
	// https://en.cppreference.com/w/c/language/operator_precedence

	d = d / data_len;

	calculate_average_status = 0;

	return d;
}


double calculate_average2(int * data, int data_len) {
	// utilizza incremental average:
	// m(n) = m(n-1) + (a(n) - m(n-1)) / n

	// TODO: aggiungere controlli su parametri

	double d = 0.0;

	for (int i = 0; i < data_len; i++) {
		d = d + (data[i] - d) / (i+1);
	}

	return d;
}


///////////////////////////////////////////////////////////
// vedere prima progetto 004.3return-array

// typedef: Kernighan&Ritchie pag. 146
// struct: Kernighan&Ritchie pag. 127

typedef struct {
	int * data;
	unsigned int length;
} int_array_type;

double calculate_average3(int_array_type ar) {

	double result;

	result = calculate_average(ar.data, ar.length);

	return result;
}


double calculate_average4(int_array_type * ar) {

	// ar->length: è equivalente a:

//	int_array_type copy;
//	copy = *ar;
//	int len = copy.length;

	return calculate_average(ar->data, ar->length);
}

///////////////////////////////////////////////////////////


int main(int argc, char * argv[]) {

	int * array;

	int dimension = 1024;

	int test_array[1] = { -1 }; // media: -1

	printf("media aritmetica di test_array: %f\n",  calculate_average(test_array, 1));
	// test_array è equivalente a:
	// &test_array[0]

	// NOTA: malloc, calloc e free richiedono:
	// #include <stdlib.h>

	array = calloc(dimension, sizeof(int));

	// TODO: check calloc return value

	//
	for (int i = 0; i < dimension; i++) {
		array[i] = i;
	}

	double average = calculate_average(array, dimension);

	printf("la media aritmetica dell'array è %f\n", average);

	average = calculate_average2(array, dimension);

	printf("la media aritmetica (algoritmo 'incremental average') dell'array è %f\n", average);

	///
	int_array_type param;

	param.data = array;
	param.length = dimension;

	average = calculate_average3(param); // viene fatta una copia di param

	printf("la media aritmetica dell'array calcolata da calculate_average3 è %f\n", average);

	average = calculate_average4(&param); // viene passato l'indirizzo di param

	printf("la media aritmetica dell'array calcolata da calculate_average4 è %f\n", average);

	///


	return EXIT_SUCCESS;
}

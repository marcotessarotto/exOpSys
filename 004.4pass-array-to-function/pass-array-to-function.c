#include <stdio.h>
#include <stdlib.h>



double calculate_average(int * data, int data_len) {
	/*
	 * in Java, quando passo un array come argomento di una funzione, oltre al riferimento all'oggetto
	 * viene passato "automaticamente" anche la lunghezza dell'array.
	 *
	 * in C l'array non si porta dietro "automaticamente" la lunghezza di se stesso:
	 * bisogna passare l'indirizzo dell'array E la lunghezza dell'array, in due parametri distinti
	 */

	double d = 0.0;

	if (data == NULL) {
		printf("data is NULL!!!\n");
		// come faccio a comunicare al chiamante lo stato di errore?
		// in C non ci sono le eccezioni...
		// 0 potrebbe essere un risultato valido
		return 0;
	}

	if (data_len == 0) {
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


int main(int argc, char * argv[]) {

	int * array;

	int dimension = 1024;

	// NOTA: malloc, calloc e free richiedono:
	// #include <stdlib.h>

	array = calloc(dimension, sizeof(int));

	//
	for (int i = 0; i < dimension; i++) {
		array[i] = i;
	}

	double average = calculate_average(array, dimension);

	printf("la media aritmetica dell'array è %f\n", average);

	average = calculate_average2(array, dimension);

	printf("la media aritmetica (algoritmo 'incremental average') dell'array è %f\n", average);


	return EXIT_SUCCESS;
}

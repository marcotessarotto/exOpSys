#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0

typedef struct INT {
	void * data;
	int len;
	char data_type; // ‘i’, ‘f’
} INPUT_PROCESSATO;


INPUT_PROCESSATO leggi_riga_input() {

	INPUT_PROCESSATO risultato = { NULL, 0, '\0' };

	while (1) {

		int res = scanf(" %c", &risultato.data_type); // lo spazio prima di %c, istruisce scanf a saltare gli spazi vuoti prima del carattere

		if (DEBUG)
			printf("ho letto: %d  %c\n", risultato.data_type, risultato.data_type);

		if (res == 0) {
			printf("leggi_riga_input: EOF\n");
			return risultato;
		}

		if (risultato.data_type == '\n') // riprovo
			continue;

		if (risultato.data_type != 'i' && risultato.data_type != 'f') {
			printf("leggi_riga_input: errore in lettura dati\n");
			return risultato;
		}

		// se sono qui, ho letto i oppure f
		break;
	}

	int size;

	if (risultato.data_type == 'i')
		size = sizeof(long int);
	else if (risultato.data_type == 'f')
		size = sizeof(double);

	if (DEBUG)
		printf("data_type: %c\n", risultato.data_type);

	scanf("%d", &risultato.len);

	// allochiamo l'array
	risultato.data = calloc(risultato.len, size);

	if (risultato.data_type == 'i') {

		long int * data_int = risultato.data;

		for (int i = 0; i < risultato.len; i++) {
			scanf("%ld", &data_int[i]);
			// oppure:
//			scanf("%d", &((int *)risultato.data)[i]);
		}

	} else if (risultato.data_type == 'f') {

		double * data_double = risultato.data;

		for (int i = 0; i < risultato.len; i++) {
			scanf("%lf", &data_double[i]);
			// oppure:
//			scanf("%lf", &((double *)risultato.data)[i]);

		}
	}

	return risultato;
}

void print_input(INPUT_PROCESSATO input) {

	if (input.data == NULL)
		return;

	printf("print_input: %c len=%d ", input.data_type, input.len);

	if (input.data_type == 'i') {
		for (int i = 0; i < input.len; i++) {
			printf("data[%d]=%ld ", i, ((long int *)input.data)[i]);
		}
	}
	else if (input.data_type == 'f')
		for (int i = 0; i < input.len; i++) {
			printf("data[%d]=%lf ", i, ((double *)input.data)[i]);
		}
	printf("\n");
}


int main(int argc, char *argv[]) {

	printf("esercizio 1 - 29/03/2019\n");

	printf("esempi di input:\n");

	printf("i 5 87676875675 -778176876 87686876 876123128 7687673434\n");
	printf("i 12 8787 -87 0 6876 5 54 511 -1213 65445 11 99 -1\n");
	printf("i 0\n");
	printf("f 3 1.23 5.565 7.3434\n");
	printf("f 5 0.1 0.12124 1.1212 345.43 6.789\n");
	printf("\n");

	while (1) {

		INPUT_PROCESSATO input = leggi_riga_input();

		if (input.data == NULL)
			break;

		print_input(input);

		free(input.data);
	}

	printf("bye!\n");

	return 0;
}


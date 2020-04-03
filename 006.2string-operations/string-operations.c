#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <errno.h>

/*
 * operazioni tra stringhe di caratteri:
 *
 * leggere una stringa da stdin: OK
 * scrivere una stringa su stdout: OK
 *
 * funzioni sscanf, sprintf
 *
 * lunghezza di una stringa di caratteri
 *
 * confronto tra stringhe di caratteri
 *
 * duplicare una stringa
 *
 * copiare una stringa
 *
 * concatenare due stringhe
 *
 * ***IN PROGRESS***
 *
 */


int main(int argc, char *argv[]) {

	// leggere una stringa di caratteri da stdin
	int res;
	char line[80+1];

	printf("scrivi una stringa di caratteri:\n");

	// problema di sicurezza:
	// line contiene al max 80 caratteri (più carattere \0 terminatore)
	// specifichiamo a scanf di leggere al massimo n caratteri
	res = scanf("%80s", line);

	// controllare il numero di valori letti da scanf
	if (res < 1) {
		printf("scanf returns %d\n", res);
	} else {
		printf("ok stringa di caratteri ricevuta.\n");
	}

	printf("\n");

	// scrivere una stringa di caratteri su stdout
	// lunghezza di una stringa di caratteri

	unsigned int string_length = strlen(line); // strlen è definita in string.h

	printf("line='%s'\nstrlen(line)=%u\n", line, string_length);

	printf("\n");

	// confronto tra stringhe di caratteri

	char * abc = "abc";
	char * ABC = "ABC";

	res = strcmp(abc, ABC);
	/* man strcmp
	 *
	 * It returns an integer less than, equal to, or greater than zero if s1 is found,
       respectively, to be less than, to match, or be greater than s2.
	 */

	printf("risultato strcmp(): %d\n", res);

	printf("\n");

	// duplica stringa
	// strdup()

	// copia stringa


	return 0;
}

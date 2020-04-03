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
 * lunghezza di una stringa di caratteri: OK
 *
 * confronto tra stringhe di caratteri: OK
 *
 * duplicare una stringa: OK
 *
 * copiare una stringa: OK
 *
 * concatenare due stringhe  => vedere progetto 013strings, string_concat()
 *
 *
 */


int main(int argc, char *argv[]) {

	char * zero_str = "0";
	// rappresentazione in memoria:
	// 2 byte: 48  0

	//char * zero_str;

	printf("zero_str = \"%s\"\n", zero_str);
	printf("zero_str[0]=%u\n", zero_str[0]);

	zero_str = "\0";
	// rappresentazione in memoria:
	// 2 byte: 0  0

	printf("zero_str = \"%s\"\n", zero_str);
	printf("zero_str[0]=%u\n", zero_str[0]);

	zero_str = "";
	// rappresentazione in memoria:
	// 1 byte: 0

	printf("zero_str = \"%s\"\n", zero_str);


	printf("\n");
	printf("\n");


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

	res = strcmp(abc, ABC); // confronto di stringhe di caratteri
	/* man strcmp
	 *
	 * It returns an integer less than, equal to, or greater than zero if s1 is found,
       respectively, to be less than, to match, or be greater than s2.
	 */

	/*
	if (abc == ABC) { // confronto indirizzi
			....
		}

	if (*abc == ABC) {
			....
		}

	if (abc[0] == ABC[0]) {
		....
	}
    */

	printf("risultato strcmp(\"abc\",\"ABC\"): %d\n", res);
	// 32: è dato da 'a' - 'A'

	res = strcmp("a","b");
	printf("risultato strcmp(\"a\",\"b\"): %d\n", res);
	// il risultato è 'a' - 'b'

	res = strcmp("b","a");
	printf("risultato strcmp(\"b\",\"a\"): %d\n", res);

	res = strcmp("aaa","aaa");
	printf("risultato strcmp(\"aaa\",\"aaa\"): %d\n", res);

	printf("\n");

	// duplicare una stringa

	char orig_str[] = "stringa da duplicare";

	char * dup_str;

	dup_str = strdup(orig_str); // duplica la stringa passata come parametro
	// ricordarsi di invocare free(dup_str) quando non ci serve più

	printf("risultato di strdup: %s\n", dup_str);

	printf("\n");

	// copia stringa

	char src_str[] = "stringa da copiare";
	char dest_str[100];

	// la stringa di destinazione deve essere già allocata
	// e sufficientemente grande
	strcpy(dest_str, src_str);
	// esiste anche strncpy:
	// char *strncpy(char *dest, const char *src, size_t n);

	printf("risultato di strcpy: %s\n", dest_str);

	printf("\n");


	return 0;
}

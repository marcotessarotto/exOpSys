#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>


/*
 * convertire un numero intero in una stringa di caratteri
 *
 */

char * itoa_printf(int number);


// esercizio: completare

// char * ltoa_printf(long number);
// char * dtoa_printf(double number);


int main(int argc, char * argv[]) {

	char * str;

	str = itoa_printf(INT_MAX);

	printf(str);
	printf("\n");

	free(str);

	str = itoa_printf(INT_MIN);

	printf(str);
	printf("\n");

	free(str);

	str = itoa_printf(1234);

	printf(str);
	printf("\n");

	return 0;
}

// minima lunghezza della stringa di caratteri per rappresentare number (in base 10)
int min_string_len(int number) {

	// man ceil -> smallest integral value not less than argument
	// man log10 -> logaritmo base 10
	// man labs -> valore assoluto

	int result =  (int)( ceil( log10( labs(number) )) + 1 ) * sizeof(char);

	if (number < 0)
		result++;

	return result;
}


char * itoa_printf(int number) {
	char * result;

	// include '\0'
	printf("minimum string len: %d\n",min_string_len(number));

	result = calloc(12, sizeof(char));

	snprintf(
			result, // zona di memoria in cui snprintf scriverà la stringa di caratteri
			12, // dimensione della zona di memoria
			"%d", // stringa di formattazione
			number); // numero intero da convertire in stringa

	return result;
}

#include <stdio.h>
#include <string.h>
#include <limits.h>

#include <float.h>

int main(int argc, char **argv) {

	char ch;

	short int var_si;
	unsigned short int var_usi;

	int var_i; // oppure signed int
	unsigned int var_ui;


	// char
	printf("***char***\n");
	printf("valore minimo di char: %d\n", CHAR_MIN);
	printf("valore massimo di char: %d\n", CHAR_MAX);
	printf("sizeof(char) = %ld byte\n", sizeof(char));

	// short int
	printf("\n***short int***\n");
	printf("valore minimo di short int: %d\n", SHRT_MIN);
	printf("valore massimo di short int: %d\n", SHRT_MAX);
	printf("sizeof(short int) = %ld byte\n", sizeof(short int));

	// esercizio: mostrare le stesse informazioni per unsigned short int


	// int
	printf("\n***int***\n");
	printf("valore minimo di int: %d\n", INT_MIN);
	printf("valore massimo di int: %d\n", INT_MAX);
	printf("sizeof(int) = %ld byte\n", sizeof(int));

	// esercizio: mostrare le stesse informazioni per unsigned int

	// long
	printf("\n***long***\n");
	printf("valore minimo di long: %ld\n", LONG_MIN);
	printf("valore massimo di long: %ld\n", LONG_MAX);
	printf("sizeof(long) = %ld byte\n", sizeof(long));

	// esercizio: mostrare le stesse informazioni per unsigned long


	// tipi dati a virgola mobile

	// https://www.tutorialspoint.com/c_standard_library/float_h.htm

	printf("\n***float***\n");
	printf("The maximum value of float = %.10e\n", FLT_MAX);
	printf("The minimum value of float = %.10e\n", FLT_MIN);
	printf("sizeof(float) = %ld byte\n", sizeof(float));

	// provare questo:
	//printf("The maximum value of float = %f\n", FLT_MAX);
	//printf("The minimum value of float = %f\n", FLT_MIN);

	printf("\n***double***\n");
	printf("The maximum value of double = %.10e\n", DBL_MAX);
	printf("The minimum value of double = %.10e\n", DBL_MIN);
	printf("sizeof(double) = %ld byte\n", sizeof(double));

	//printf("The maximum value of double = %f\n", DBL_MAX);
	//printf("The minimum value of double = %f\n", DBL_MIN);

	printf("\n***long double***\n");
	printf("The maximum value of long double = %.10Le\n", LDBL_MAX);
	printf("The minimum value of long double = %.10Le\n", LDBL_MIN);
	printf("sizeof(long double) = %ld byte\n", sizeof(long double));

	//printf("The maximum value of long double = %Lf\n", LDBL_MAX);
	//printf("The minimum value of long double = %Lf\n", LDBL_MIN);

	return 0;
}

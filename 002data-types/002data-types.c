#include <stdio.h>
#include <string.h>
#include <limits.h>

#include <float.h>

int main(int argc, char **argv) {

	// fare riferimento anche a Kernighan&Ritchie, pag 35-38

	// endianess: ordine dei byte per immagazzinare in memoria dati di dimensione superiore al byte
	// Intel, AMD -> big endian
	// ARM -> little endian

	/*

	   la costante int 291, in base 16, è 123

	   0x0123 costante intera in base 16, utilizza 2 bytes: 01 e 23


	   Little endian             Big endian
       +----+----+               +----+----+
       |0x23|0x01|              |0x01|0x23|
       +----+----+               +----+----+
 byte:    0    1                   0    1


 	   0x01234567 costante intera in base 16, utilizza 4 bytes: 01 23 45 67

           Little endian             Big endian
       +----+----+----+----+        +----+----+----+----+
       |0x67|0x45|0x23|0x01|    |0x01|0x23|0x45|0x67|
       +----+----+----+----+        +----+----+----+----+
 byte:    0    1    2    3         0    1    2    3


	 */


	// char
	printf("***char***\n");
	printf("valore minimo di char: %d\n", CHAR_MIN);
	printf("valore massimo di char: %d\n", CHAR_MAX);
	printf("sizeof(char) = %ld byte\n", sizeof(char));

	char ch;
	ch = 'Z'; // 'Z' è una costante carattere
	// ch = 90; // 90 è una costante di tipo int (4 byte), che viene convertita in char (1 byte)

	// ASCII chart: ad esempio, vedere http://www.asciitable.com/

	ch = '\012'; // valore 012 in base 8, o valore 10 in base 10
	ch = '\xa'; // valore a in base 16 o valore ... in base 10

	ch = '\x5A'; // esercizio: a che simbolo corrisponde?

	ch = '\0'; // carattere con valore 0


	// short int
	printf("\n***short int***\n");
	printf("valore minimo di short int: %d\n", SHRT_MIN);
	printf("valore massimo di short int: %d\n", SHRT_MAX);
	printf("sizeof(short int) = %ld byte\n", sizeof(short int));

	short int var_short_int;

	var_short_int = 0x0123;

	var_short_int = 10000;
	// 10000 è una costante di tipo int (4 byte), che viene convertita in short int (2 byte)
	// troncando i due byte più significativi (e con sign extension)


	//unsigned short int var_usi;
	// esercizio: mostrare le stesse informazioni per unsigned short int (usare USHRT_MAX)


	// int
	printf("\n***int***\n");
	printf("valore minimo di int: %d\n", INT_MIN);
	printf("valore massimo di int: %d\n", INT_MAX);
	printf("sizeof(int) = %ld byte\n", sizeof(int));

	int var_signed_int;
	var_signed_int = 0x01234567;

	printf("\n***unsigned int***\n");
	printf("valore minimo di unsigned int: %d\n", 0);
	printf("valore massimo di unsigned int: %u\n", UINT_MAX);
	printf("sizeof(unsigned int) = %ld byte\n", sizeof(unsigned int));

	unsigned int var_unsigned_int;
	var_unsigned_int = 123456789U; // suffisso U per costante unsigned

	// long (o long int)
	printf("\n***long***\n");
	printf("valore minimo di long: %ld\n", LONG_MIN);
	printf("valore massimo di long: %ld\n", LONG_MAX);
	printf("sizeof(long) = %ld byte\n", sizeof(long));

	long int var_signed_long_int = 1234567890L; // suffisso L


	// esercizio: mostrare le stesse informazioni per unsigned long


	unsigned long var_unsigned_long = 123456789123456789UL; // suffisso UL


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

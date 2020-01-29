#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MY_ARRAY_SIZE 100

// global variable
int a;

void increment_wrong(int a) {
	a = a + 1;
}

void increment_ok(int * a) {
	*a = *a + 1;
	// (*a)++; // come sopra
}

void mutable_string() {
	// N.B: in Java le istanze di String sono immutabili

	printf("***mutable_string()***\n"); // questa stringa è immutable (si trova nell'area del codice cioè read only)

	// SI!
	// local variable
	char string_variable[] = "this string is mutable.";
	// i caratteri della stringa sono modificabili perchè si trovano nell'area 'static data'
	string_variable[0] = 'T'; // ok è permesso
	// string_variable = "non si può fare"; // errore di compilazione

	printf("len=%ld string=%s\n", strlen(string_variable), string_variable);

	// NO!
	// causa un warning:
	char * puntatore_char;

	puntatore_char = "this string is immutable!"; // la stringa 'costante' "..." viene scritta nel segmento del codice => read only
	// puntatore_char[0] = '*'; // NO! compila ma crash del programma, provo a scrivere nell'area del codice

	puntatore_char = "altra stringa!"; // OK!
	// puntatore_char[0] = '*'; // NO! compila ma crash del programma, provo a scrivere nell'area del codice

	puntatore_char = string_variable; // OK!
	puntatore_char[0] = '*'; // OK! compila e funziona! provate a scrivere su stdout string_variable...

	// how string literals look in compiled code?
	// https://gcc.godbolt.org/#%7B%22version%22%3A3%2C%22filterAsm%22%3A%7B%22labels%22%3Atrue%2C%22directives%22%3Atrue%2C%22commentOnly%22%3Atrue%7D%2C%22compilers%22%3A%5B%7B%22sourcez%22%3A%22MQSwdgxgNgrgJgUwAQB4DOAXOID2A6ACwD4AoE8DJAWwENwAKASiQG8SkOkAHGDNegEQEEUKDiQB3HACcocADpgBjANwkAvkAAA%3D%22%2C%22compiler%22%3A%22g492%22%2C%22options%22%3A%22%22%7D%5D%7D

	printf("len=%ld string=%s\n", strlen(puntatore_char), puntatore_char);

	char ape1[] = "ape1"; // array di char di dimensione 4+1 (zero terminated)
	ape1[0] = 'A'; // OK
	*(ape1 + 0) = 'A'; // uguale a ape1[0] = 'A';
	printf("ape1=%s\n",ape1);

	ape1[1] = 'P';
	*(ape1 + 1) = 'P'; // uguale a ape1[1] = 'P';
	printf("ape1=%s\n",ape1);

	ape1[2] = 'E';
	printf("ape1=%s\n",ape1);
	// MA: ape1 non può "puntare" ad un'altra stringa
	// ape1 = "ciao!"; // NO!

	char * ape2 = "ape2"; // stringa non modificabile
	// ape2[0] = 'A'; // WRONG!

	char ape3[] = { 'a', 'p', 'e', '3', 0 }; // equivalente a: char ape3[] = "ape3";
	ape3[0] = 'A'; // OK
//	printf("%s\n",ape3);

//	char * ape4 = { 'a', 'p', 'e', '4', 0 }; // stringa non modificabile
//	ape4[0] = 'A'; // WRONG!
//	printf("%s\n",ape4);

	char * dynamic_char_array = malloc(MY_ARRAY_SIZE * sizeof(char)); // alloca array di cahr di dimensione 100

	for(int i = 0; i < MY_ARRAY_SIZE-1; i++)
		dynamic_char_array[i] = 32 + i % 64;
	dynamic_char_array[MY_ARRAY_SIZE-1] = 0; // dobbiamo terminare con 0

	printf("dynamic_char_array=%s\n", dynamic_char_array);

	strcpy(dynamic_char_array, "è una stringa di lunghezza minore di 100 caratteri. è ok!");

	printf("dynamic_char_array=%s\n", dynamic_char_array);

	// ora dobbiamo liberare manualmente la memoria richeista
	free(dynamic_char_array);

}

void int_array() {

	printf("***int_array()***\n");

	// dimensione fissa a 100 bytes; non posso fare resize
	int array_int[MY_ARRAY_SIZE]; // array di 100*4 = 400 bytes allocato sulla stack; automaticamente rilasciato alla fine della funzione

	for (int i = 0; i < MY_ARRAY_SIZE; i++)
		array_int[i] = i;

	int * int_ptr;

	int_ptr = array_int;

	int_ptr[10] = -1000;

	for (int i = 0; i < MY_ARRAY_SIZE; i++)
		printf("%d ", array_int[i]);
	printf("\n");

}

int main(int argc, char * argv[]) {

	mutable_string();

	int_array();

	//
	a = 200;
	increment_wrong(a);

	printf("a=%d\n",a);

	increment_ok(&a);

	printf("a=%d\n",a);


	int local_var = 100;
	increment_wrong(local_var);

	printf("local_var=%d\n",local_var);

	increment_ok(&local_var);

	printf("local_var=%d\n",local_var);

	////////////


	int valA = 100;
	int valB = 200;

	int * ptr;

	ptr = &valA;

	*ptr = 300; // valA = 300;

	ptr = &valB;

	*ptr = 400; // valB = 400;

	// quanto vale valA?

	// quanto vale valB?



	for (int i = 0; i < argc; i++) {
		printf("argv[%d]=%s\n", i, argv[i]);
	}

	exit(EXIT_SUCCESS);

}

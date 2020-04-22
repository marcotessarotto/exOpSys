#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void func(int arg [] /* oppure int * arg */, int arg2[100]) {
	// la dimensione dell'array arg è "persa";
	// se func ha bisogno di conoscerla, può essere passata come
	// argomento della funzione stessa

	printf("func\n");
	printf("size of arg: %d\n", sizeof(arg)); // size del puntatore, 8
	printf("size of arg2: %d\n", sizeof(arg2)); // idem, non è 100
	printf("\n");

	arg[0] = 0xFFFFFFFF;
	arg2[1] = -1;
	//printf("%d\n", sizeof(void)); // == 1
}

void func2(int size) {
	int array[size]; // allocata sulla stack

	// calculations.....
	array[size-1] = 4321;
}

void func3(int array[], int size) {
	// ....
}

int main(int argc, char * argv[]) {

	int my_array[100] = { 1,2,3,4 }; // inizializza i primi elementi ai valori
	// specificati, gli altri a zero

	//my_array[1024] = 0xDEADBEEF; // dove stiamo scrivendo?!? BUG!!!

	printf("sizeof(int) = %ld\n", sizeof(int));
	printf("sizeof(my_array) = %ld bytes\n", sizeof(my_array));
	printf("numero di elementi dell'array: %ld\n", sizeof(my_array) / sizeof(int) );
	printf("\n");

	func(my_array, my_array);

	func2(1024);

	for (int i = 0; i < sizeof(my_array) / sizeof(int); i++)
		printf("[%d]=%d ", i, my_array[i]);

	printf("\n\n");

	char messaggio[] = "hello!";

	printf("sizeof(char) = %lu\n", sizeof(char));
	printf("sizeof(messaggio) = %ld bytes\n", sizeof(messaggio));
	printf("numero di elementi dell'array: %ld\n", sizeof(messaggio) / sizeof(char) );
	printf("strlen(messaggio) = %ld\n", strlen(messaggio));
	printf("\n");

	char messaggio_con_spazio_aggiuntivo[100] = "hello!";

	printf("sizeof(char) = %lu\n", sizeof(char));
	printf("sizeof(messaggio_con_spazio_inutilizzato) = %ld bytes\n", sizeof(messaggio_con_spazio_aggiuntivo));
	printf("numero di elementi dell'array: %ld\n", sizeof(messaggio_con_spazio_aggiuntivo) / sizeof(char) );
	printf("strlen(messaggio_con_spazio_inutilizzato) = %ld\n", strlen(messaggio_con_spazio_aggiuntivo));
	printf("\n");


	exit(EXIT_SUCCESS);
}

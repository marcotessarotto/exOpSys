#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stddef.h> /* size_t */


void show_array(const void * array, unsigned int size) {



}


int main(int argc, char * argv[]) {

	int counter, counter2;

	unsigned int array [] = { 0x04030201, 0x08070605, 0x090a0b0c, 0xfd, 0xfe000000, 0xff };
	// il valore 0xff viene usato come 'segnalatore' di fine array

	unsigned int array_size = sizeof(array);
	unsigned int array_len = sizeof(array) / sizeof(int);

	unsigned char * cptr;
	unsigned int * iptr;

	printf("'array' è un array di unsigned int, numero di celle=%u, dimensione totale in bytes=%u\n\n",
			array_len, array_size);

	cptr = (unsigned char *)array;
	iptr = array;

	//////////////////////////////////////////////////////
	printf("1 - scorro 'array' come un array di unsigned char:\n");


	counter = 0;

	while (*cptr != 0xff) {
		printf("cptr[%2d] = 0x%02x \t\t &cptr[%d] = %p\n", counter, *cptr, counter, cptr);

		if (counter % 4 == 3)
			printf("\n");

		counter++;
		cptr++; // incremento il puntatore: l'indirizzo aumenta di 1 byte
	}

	printf("\n");

	//////////////////////////////////////////////////////
	printf("2 - scorro 'array' come un array di unsigned int:\n");
	counter = 0;

	while (*iptr != 0xff) {
		printf("iptr[%2d] = 0x%08x \t\t &iptr[%d] = %p\n", counter, *iptr, counter, iptr);

		counter++;
		iptr++; // incremento il puntatore: l'indirizzo aumenta di 4 byte
	}

	printf("\n\n");

	//////////////////////////////////////////////////////
	printf("3 - scorro 'array' come un array di unsigned char ma a gruppi di 2 byte:\n");

	cptr = (unsigned char *)array;
	counter = 0;

	while (*cptr != 0xff) {
		unsigned short int si = *(unsigned short int *)cptr;

		printf("si = 0x%04x \t\t &cptr[%d] = %p\n", si, counter, cptr);

		counter += 2;
		cptr += 2; // incremento il puntatore: l'indirizzo aumenta di 2 byte
	}

	printf("\n\n");

	//////////////////////////////////////////////////////
	printf("4 - scorro 'array' simultaneamente come un array di unsigned int e come un array di unsigned char:\n");

	cptr = (unsigned char *)array;
	iptr = array;
	counter = 0;
	counter2 = 0;

	while (*iptr != 0xff) {
		printf("iptr[%2d] = 0x%08x \t\t &iptr[%d] = %p\n", counter, *iptr, counter, iptr);

		counter++;
		iptr++; // incremento il puntatore: l'indirizzo aumenta di 4 byte

		for (int i = 0; i < 4; i++) {
			printf("\tcptr[%2d] = 0x%02x \t\t\t\t &cptr[%d] = %p\n", counter2, *cptr, counter2, cptr);
			cptr++;
			counter2++;
		}
	}


	return 0;
}

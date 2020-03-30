#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// https://stackoverflow.com/a/3974138/974287
// assumes little endian
void print_bits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }

}

void print_char_bits(unsigned char bb)
{
	print_bits(sizeof(unsigned char), &bb);
	printf("\n");
}


void print_short_int_bits(unsigned short int bb)
{
	print_bits(sizeof(unsigned short int), &bb);
	printf("\n");
}



int main(int argc, char * argv[]) {

	unsigned char b;
	int i;
	unsigned int * iptr;
	unsigned char * cptr;

	/////// esempio 1
	// puntatore a int, operatore indirizzo (&), operatore indirezione (*)

	unsigned int prova = 0xADD1CAFE;

	/*
           Little endian             Big endian
       +----+----+----+----+        +----+----+----+----+
       |0xFE|0xCA|0xD1|0xAD|        |0xAD|0xD1|0xCA|0xFE|
       +----+----+----+----+        +----+----+----+----+
 byte:    0    1    2    3            0    1    2    3

	 */

	printf("esempio 1\n");

	iptr = &prova;

	printf("1: prova = %08x\n", prova);

	printf("2: *iptr = %08x\n", *iptr); // accedo alla zona di memoria il cui indirizzo si trova in iptr

	printf("\n\n");

	/////// esempio 2
	// cast da puntatore a uint a puntatore a char, incremento puntatore a char

	printf("esempio 2:\n");

	cptr = (unsigned char *) &prova;

    printf("1: %x\n", *cptr);

    cptr++;
    printf("2: %x\n", *cptr);

    cptr++;
	printf("3: %x\n", *cptr);

	cptr++;
	printf("4: %x\n", *cptr);

	printf("\n\n");

	/////// esempio 3 (esercizio)
	//

	printf("esempio 3\n");

	unsigned char add1_cafe[] = {0xA, 0xD, 0xD, 0x1, 0xC, 0xA, 0xF, 0xE};

	unsigned int add1_cafe_final = 0;

	/* ESERCIZIO:
	 *
	 * a partire da add1_cafe, vogliamo arrivare a add1_cafe_final con questo valore:

    Little endian
+----+----+----+----+
|0xFE|0xCA|0xD1|0xAD|
+----+----+----+----+
  0    1    2    3

*/

	unsigned char bytes[4];

	// << è l'operatore "shift left"

	printf("var       = 0x%08x , binary: ", 0xF);
	print_short_int_bits(0xF);

	printf("var << 4  = 0x%08x , binary: ", 0xF << 4);
	print_short_int_bits(0xF << 4);

	printf("var << 8  = 0x%08x , binary: ", 0xF << 8);
	print_short_int_bits(0xF << 8);

	printf("var << 12 = 0x%08x , binary: ", 0xF << 12);
	print_short_int_bits(0xF << 12);

	printf("\n");
	// ...
	// ...

	// | è l'operatore aritmetico OR

	printf("0xA       = 0x%08x , binary: ", 0xA);
	print_short_int_bits(0xA);

	printf("0xB       = 0x%08x , binary: ", 0xB);
	print_short_int_bits(0xB);

	printf("0xA << 4  = 0x%08x , binary: ", 0xA << 4);
	print_short_int_bits(0xA << 4);

	printf("0xA0 | 0xB= 0x%08x , binary: ", 0xA << 4 | 0xB);
	print_short_int_bits(0xA << 4 | 0xB);

	printf("\n");


	// ESERCIZIO: COMPLETARE gli indici
	bytes[0] = add1_cafe[6] << 4 | add1_cafe[7];

	bytes[1] = 0; //add1_cafe[ ] << 4 | add1_cafe[ ]; // completare

	bytes[2] = 0; //add1_cafe[ ] << 4 | add1_cafe[ ]; // completare

	bytes[3] = 0; //add1_cafe[ ] << 4 | add1_cafe[ ]; // completare

	add1_cafe_final = *((unsigned *)bytes);

	// come output vogliamo: add1cafe
	printf("add1_cafe_final = 0x%08x\n", add1_cafe_final);

	printf("\n\n");


	return 0;
}

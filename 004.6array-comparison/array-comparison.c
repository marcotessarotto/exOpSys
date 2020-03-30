#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stddef.h> /* size_t */

// equivalente a memcmp
int my_memcmp(const void* s1, const void* s2,size_t n)
{
    const unsigned char *p1 = s1, *p2 = s2;

    while(n--)
        if( *p1 != *p2 ) {
            return *p1 - *p2;
        } else {
            p1++; // incremento di puntatore di tipo unsigned char * (+1 byte)
            p2++; // idem
        }
    return 0;
}

void print_array(int array[], unsigned int length) {

	printf("i[ ");

	for (unsigned i = 0; i < length; i++) {
		printf("%4d ", array[i]);
	}

	printf(" ] ");

	//printf(", len=%d\n", length);

}

void print_char_array(char * array, size_t n, int newline) {
	printf("c[ ");

	for (unsigned i = 0; i < n; i++) {
		printf("%2x ", array[i] & 0xFF);
		if (newline && (i+1) % 4 == 0) {
			printf("\n  ");
		}
	}

	printf(" ]");
}

#define ARRAY_LEN 10

void print_arrays(int * arr1, int * arr2, int len) {
	printf("confronto " );
	print_array(arr1, len);

	printf(" con ");
	print_array(arr2, len);

	printf(", ovvero:\n");

	// array rappresentato in base 16, char *
	printf("confronto " );
	print_char_array((char *)arr1, len * sizeof(int), 0);

	printf(" con ");
	print_char_array((char *)arr2, len * sizeof(int), 0);

	printf(":\n");
}

int main(int argc, char * argv[]) {

	int result;

    int array_a[ARRAY_LEN] = {
    		1, 2, 3, 4, 5, 6, 7
    };

    int array_b[ARRAY_LEN] = {
    		1, 2, 3, 4, 0, 0, 0
    };

    int array_c[ARRAY_LEN * 2] = {
    		-1, -1, -1, 0, 1, 2, 3, 4, 1000, 2000
    };


    printf("sizeof(array_a) = %lu bytes\n", sizeof(array_a));
    printf("sizeof(array_b) = %lu bytes\n", sizeof(array_b));
    printf("sizeof(array_c) = %lu bytes\n", sizeof(array_c));

    printf("\n");

    printf("sono dati tre array di int:\n");
    printf("array_a : ");
    print_array(array_a, ARRAY_LEN);
    printf("\n");

    printf("array_b : ");
    print_array(array_b, ARRAY_LEN);
    printf("\n");

    printf("array_c : ");
    print_array(array_c, ARRAY_LEN * 2);
    printf("\n\n");

    printf("array_a ha %lu celle, array_b ha %lu celle, array_c ha %lu celle\n\n",
    		sizeof(array_a) / sizeof(int),
			sizeof(array_b) / sizeof(int),
			sizeof(array_c) / sizeof(int)
			);


    printf("rappresentazione come array di char in base 16 di array_c\n");
    print_char_array((char *) array_c, ARRAY_LEN * 2 * sizeof(int), 1);

    printf("\n\n");

    /*
       man 3 memcmp

       The  memcmp()  function  returns  an integer less than, equal to,
       or greater than zero if the first n bytes of s1 is found, respectively,
       to be less than, to match, or be greater
       than the first n bytes of s2.

       For a nonzero return value, the sign is determined by the sign of the difference between the
       first pair of bytes (interpreted as unsigned char) that differ in s1 and s2.

       If n is zero, the return value is zero.
 */
    printf("\n");

    // NOTA: il terzo parametro di memcmp è il numero di bytes dei due array da confrontare

    result = memcmp(array_a, array_b, ARRAY_LEN * sizeof(int));
    print_arrays(array_a, array_b, ARRAY_LEN);
    printf("memcmp(array_a, array_b, 10 * sizeof(int)) = %d\n", result);
    printf("\n");

    result = memcmp(array_a, array_b, 4 * sizeof(int));
    print_arrays(array_a, array_b, 4);
    printf("memcmp(array_a, array_b, 4* sizeof(int)) = %d ***ARRAY UGUALI***\n", result);
    printf("\n");

    result = memcmp(array_a, array_c, 4 * sizeof(int));
    print_arrays(array_a, array_c, 4);
    printf("memcmp(array_a, array_c, 4* sizeof(int)) = %d\n", result);
    printf("\n");


    // -254 risulta da:
    // confronto tra (char *)array_a ed (char *)array_c alla posizione 0:
    // 0x01 - 0xFF
    // => (unsigned char) 1 - (unsigned char) 255
    // => (int) 1 - (int)255 = -254


    //////////////////////////////
    result = my_memcmp(array_a, array_b, ARRAY_LEN * sizeof(int));
    printf("my_memcmp(array_a, array_b, 10* sizeof(int)) = %d\n", result);

    result = my_memcmp(array_a, array_b, 4 * sizeof(int));
    printf("my_memcmp(array_a, array_b, 4* sizeof(int)) = %d\n", result);

    result = my_memcmp(array_a, array_c, 4 * sizeof(int));
    printf("my_memcmp(array_a, array_c, 4* sizeof(int)) = %d\n", result);

    printf("\n\n");
    //////////////////////////////

    printf("ricordiamo:\n");
    printf("array_c : ");
    print_array(array_c, ARRAY_LEN * 2);
    printf("\n");
    printf("array_b : ");
    print_array(array_b, ARRAY_LEN);
    printf("\n\n");

    for (int i = 0; i < 6; i++) {

    	result = memcmp(array_b, &array_c[i], 4 * sizeof(int));

    	printf("confronto array_b con il 'sotto-array' di array_c, "
    			"a partire dalla posizione %d, lunghezza %lu bytes (%d celle)\n",
				i, 4 * sizeof(int), 4);
    	//
    	printf("confronto " );
    	print_array(array_b, 4);

    	printf(" con ");
    	print_array(&array_c[i], 4);

    	printf(", ovvero:\n");

    	// array rappresentato in base 16, char *
    	printf("confronto " );
    	print_char_array((char *)array_b, 4 * sizeof(int), 0);

		printf(" con ");
		print_char_array((char *)&array_c[i], 4 * sizeof(int), 0);

		printf(":\n");

    	printf("memcmp(array_b, &array_c[%d], 16) = %d%s\n\n", i, result,
    			result == 0 ? " ***ARRAY UGUALI***" : "");

    }

}

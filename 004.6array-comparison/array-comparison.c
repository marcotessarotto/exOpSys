#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stddef.h> /* size_t */

int my_memcmp(const void* s1, const void* s2,size_t n)
{
    const unsigned char *p1 = s1, *p2 = s2;

    while(n--)
        if( *p1 != *p2 )
            return *p1 - *p2;
        else
            p1++,p2++;
    return 0;
}

#define ARRAY_LEN 10

int main(int argc, char * argv[]) {

	int result;

//    int rnd_array_int [] = {
//    		-1958, 140, 607, 1447, -22, -1400, -1748, -1150, 1623, -971, 891, -94, 1805, 105, 814, 1286, -1791, 359, -1277, -520, -1535, 663, -533, -383, -979, -694, -748, 347, 495, -169, -157, -751, 1302, -1307, -405, 311, 595, 1344, -604, -262, 777, 779, 1491, 13, 1333, 357, -240, -197, 685, 1309, 195, -207, -1545, 601, -1227, 1411, 172, 1487, 572, -1268, 1719, 1276, -1943, -352, -1366, -263, -1955, 118, -1407, -582, -1229, 193, 1818, -188, -594, 688, 1534, -1458, 1455, -1623, -1167, 1208, -1383, 1022, -1465, -1586, -300, 851, 515, 1959, 659, 486, 1039, 115, -1063, -591, 566, -1609, 655, -1410
//    };

    int array_a[ARRAY_LEN] = {
    		1, 2, 3, 4, 5, 6, 7
    };

    int array_b[ARRAY_LEN] = {
    		1, 2, 3, 4
    };

    int array_c[ARRAY_LEN * 2] = {
    		-1, -1, -1, 0, 1, 2, 3, 4, 1000, 2000 };

    printf("sizeof(array_a) = %lu\n", sizeof(array_a));
    printf("sizeof(array_b) = %lu\n", sizeof(array_b));

    printf("sizeof(array_c) = %lu\n", sizeof(array_c));

/*
 *
 *
 *     man 3 memcmp
 *
       The  memcmp()  function  returns  an integer less than, equal to,
       or greater than zero if the first n bytes of s1 is found, respectively,
       to be less than, to match, or be greater
       than the first n bytes of s2.

       For a nonzero return value, the sign is determined by the sign of the difference between the
       first pair of bytes (interpreted as unsigned char) that differ in s1 and s2.

       If n is zero, the return value is zero.
 *
 */

    result = memcmp(array_a, array_b, ARRAY_LEN * sizeof(int));
    printf("memcmp(array_a, array_b, ARRAY_LEN * sizeof(int)) = %d\n", result);

    result = memcmp(array_a, array_b, 4 * sizeof(int));
    printf("memcmp(array_a, array_b, 4* sizeof(int)) = %d\n", result);

    result = memcmp(array_a, array_c, 4 * sizeof(int));
    printf("memcmp(array_a, array_c, 4* sizeof(int)) = %d\n", result);




    result = my_memcmp(array_a, array_b, ARRAY_LEN * sizeof(int));
    printf("my_memcmp(array_a, array_b, ARRAY_LEN* sizeof(int)) = %d\n", result);

    result = my_memcmp(array_a, array_b, 4 * sizeof(int));
    printf("my_memcmp(array_a, array_b, 4* sizeof(int)) = %d\n", result);

    result = my_memcmp(array_a, array_c, 4 * sizeof(int));
    printf("my_memcmp(array_a, array_c, 4* sizeof(int)) = %d\n", result);



}

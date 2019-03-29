#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#include "strtoi_samples.h"

// tratto da
// https://en.cppreference.com/w/c/string/byte/strtoimax

void strtoi_samples(void)
{

/*
NAME
       strtol, strtoll, strtoq - convert a string to a long integer

SYNOPSIS
       #include <stdlib.h>

       long int strtol(const char *nptr, char **endptr, int base);

       long long int strtoll(const char *nptr, char **endptr, int base);


 * RETURN VALUE
       The strtol() function returns the result of the conversion, unless the value would underflow or overflow.  If an underflow occurs, strtol() returns LONG_MIN.  If an overflow  oc‐
       curs, strtol() returns LONG_MAX.  In both cases, errno is set to ERANGE.  Precisely the same holds for strtoll() (with LLONG_MIN and LLONG_MAX instead of LONG_MIN and LONG_MAX).

 *
 */

	printf("strtoi_samples()\n");


    char* endptr;


    printf("%ld\n", strtol(" -123junk",&endptr,10)); /* base 10                    */
    printf("%ld\n", strtol("11111111",&endptr,2));   /* base 2                     */
    printf("%ld\n", strtol("XyZ",&endptr,36));       /* base 36                    */
    printf("%ld\n", strtol("010",&endptr,0));        /* octal auto-detection       */
    printf("%ld\n", strtol("10",&endptr,0));         /* decimal auto-detection     */
    printf("%ld\n", strtol("0x10",&endptr,0));       /* hexadecimal auto-detection */

    printf("\n");
    printf("LONG_MAX = %ld\n", LONG_MAX);
    printf("LONG_MIN = %ld\n", LONG_MIN);

    printf("ULONG_MAX = %lu\n", ULONG_MAX); // ULONG_MIN varrebbe 0
    printf("\n");

    /* range error             */
    /* LONG_MAX+1 --> LONG_MAX */
    errno = 0;
    printf("input è 9223372036854775808, ma risultato è %ld (errore conversione)\n", strtol("9223372036854775808",&endptr,10));
    printf("%s\n", strerror(errno));


}

// strtoi_samples.

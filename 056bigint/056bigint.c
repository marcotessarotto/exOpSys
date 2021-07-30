#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// requisito:
// sudo apt install libgmp-dev

/*
 * calcola 100! utilizzando la libreria gmp ( https://gmplib.org/ )
 *
 * documentazione della libreria GMP:
 * https://gmplib.org/manual/
 *
 */

#define MY_PRINT(msg) write(1, msg, strlen(msg));

// alternative to mpz_fac_ui
// result must not have been initialized
void calc_fact(mpz_t result, unsigned long arg) {

    mpz_init(result);

    mpz_init_set_ui(result, arg);

    for (unsigned long i = arg-1; i > 1; i--) {
    	mpz_mul_ui(result, result, i);
    }

}

// print num to file descriptor 1 in base 10
void print_mpz(mpz_t num) {
	char * msg;

	msg = mpz_get_str(NULL, 10, num);

	write(1, msg, strlen(msg));

	free(msg);
}

int main(int argc, char** argv) {

    printf("calculation of 100!\n");

    printf("using calc_fact:\n");
    mpz_t num;
    calc_fact(num, 100);

    print_mpz(num);
    printf("\n");


    printf("using mpz_fac_ui:\n");
    mpz_t num2;

    mpz_init(num2);

    mpz_fac_ui(num2, 100);

    print_mpz(num2);
    printf("\n");


    mpz_clears(num, num2, NULL);

    // alternative to print_mpz, using streams:
    //mpz_out_str(stdout, 10, mynum);

    return 0;
}



#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// sudo apt install libgmp-dev

/*
 * calcola 100! utilizzando la libreria gmp ( https://gmplib.org/ )
 *
 * documentazione della libreria GMP:
 * https://gmplib.org/manual/
 *
 */

int main(int argc, char** argv) {
    mpz_t mynum;

    mpz_init(mynum);

    mpz_add_ui(mynum, mynum, 100);

    int i;

    for (i = 99; i > 1; i--) {

        mpz_mul_si(mynum, mynum, (long)i);

    }

    // char * mpz_get_str (char *str, int base, const mpz_t op)
    char * msg;

    msg = "100! = ";

    write(1, msg, strlen(msg));

    msg = mpz_get_str(NULL, 10, mynum);

    write(1, msg, strlen(msg));

    // msg va liberata con free()

    // in alternativa, per scrivere
    //mpz_out_str(stdout, 10, mynum);

    return 0;
}



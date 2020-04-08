#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

int main ()
{

/*
 * da man srand:
 *
 * The rand() function returns a pseudo-random integer in the range 0 to RAND_MAX inclusive
 * (i.e., the mathematical range [0, RAND_MAX]).
 *
 * The  srand() function sets its argument as the seed for a new sequence of
 * pseudo-random integers to be returned by rand().
 * These sequences are repeatable by calling srand() with the same seed value.
 *
 * If no seed value is provided, the rand() function is automatically seeded with a value of 1. *
 */

  // time() returns the time as the number of seconds
  // since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
  // time_t è un typedef di unsigned long
  time_t t = time(NULL); // sizeof(time_t) = 8

  printf("numero di secondi trascorsi da EPOCH [1970-01-01 00:00:00 +0000 (UTC)]: %lu\n", t);

  // inizializza il generatore di numeri casuali
  // se non lo usiamo srand, il generatore di rand() è inizializzato con valore 1
  srand(t); // utilizza i 4 byte meno significativi di t

  // The rand() function returns a pseudo-random integer in the range 0 to RAND_MAX inclusive
  // (i.e., the mathematical range [0, RAND_MAX]).

  int random_number = rand();

  printf("random_number=%d\n", random_number);

#define A 1
#define B 10
  // se vogliamo un numero intero casuale appartenente a [A, B]:

  random_number = rand() % B + A;

  printf("random_number=%d\n", random_number);

  return 0;
}

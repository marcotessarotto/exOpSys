#include <stdio.h>


int main(int argc, char **argv) {

	int a,b;
	int sum;
	int res;

	res = scanf("%d %d", &a, &b);

	printf("scanf ha restituito: %d\n", res);

	sum = a + b;

	printf("sum = %d\n", sum);

	/*
	 * esercizio:
	 *
	 * chiedere all'utente tre input:
	 * l'operazione da fare (un char tra: + - /) e due numeri
	 * eseguire l'operazione richiesta e scrivere il risultato
	 *
	 * esempio:
	 *
	 * input:
	 * + 10 20
	 *
	 * risultato:
	 * 30
	 *
	 */

	return 0;
}

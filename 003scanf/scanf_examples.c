#include <stdio.h>


void altri_esempi_scanf() {

	char ch;
	int a, b;
	unsigned int ua, ub;
	long la, lb;

	float c;
	double d;

	printf("scrivi un char: ");
	scanf("%c", &ch);
	printf("ho letto il seguente char: %c\n", ch);

	printf("scrivi un int: ");
	scanf("%d", &a);
	printf("ho letto il seguente int: %d\n", a);


}


int main(int argc, char **argv) {

	int a,b;
	int sum;
	int res;

	//altri_esempi_scanf();

	//return 0;

	printf("scrivi due numeri interi: ");

	res = scanf("%d %d", &a, &b);

	printf("scanf ha restituito: %d (numero di valori letti)\n", res);

	if (res == EOF) {
		printf("EOF! bye!");
		return 1;
	}

	sum = a + b;

	printf("sum = %d\n", sum);

	/*
	 * esercizio da fare:
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

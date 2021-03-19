#include <stdio.h>
#include <string.h>

/*
 * restituisce 0 se calculator ha avuto successo, altrimenti 1 in caso di errore
 * scrive su stdout il risultato
 */
int calculator(void) {
	char op;
	int n1 = 0;
	int n2 = 0;
	int res = 0;
	int control;


	control = scanf(" %c %d %d ", &op, &n1, &n2);

	if (control == 3) {

		switch(op) {
				case '+':
					res = n1 + n2;
					break;
				case '-':
					res = n1 - n2;
					break;
				case '*':
					res = n1 * n2;
					break;
				case '/':
					res = n1 / n2;
					break;
				default:
					fprintf(stderr,"wrong operation\n");
					return 1;
		}

		// + 1 2 => 3
		printf("%c %d %d => %d\n", op, n1, n2, res);

		return 0;

	} else if (control == -1) {
		// siamo a End Of File
		printf("EOF!!!!\n");

		return -1;

	} else {

		fprintf(stderr,"wrong parameters, control=%d\n", control);

		return 1;
	}
}

int main(int argc, char **argv) {


	while (1) {

		int res = calculator();

		if (res != 0)
			break;


	}

	return 0;
}

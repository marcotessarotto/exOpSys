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


	control = scanf("%c %d %d", &op, &n1, &n2);

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

		printf("%d\n", res);

		return 0;

	} else {

		fprintf(stderr,"wrong parameters\n");

		return 1;
	}
}

int main(int argc, char **argv) {

	// esempio con "pipes":
	// echo "+ 1 1" | ./003.1calculator

	if (argc > 1 && !strcmp(argv[1], "-h")) {
		printf("usage: on stdin, provide:\n<op> int_number int_number\n");
		printf("where <op> is one of: + - * /\n");

		return 0;
	}

	int res = calculator();

	return res;
}

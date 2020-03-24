#include <stdio.h>



int main(int argc, char *argv[]) {

	int c;
	unsigned long counter = 0;

	// in Unix (e Linux) ogni processo, quando viene avviato, ha tre file descriptors (descrittori di file) pronti per l'uso:

	// standard input (0, stdin) da cui leggere
	// standard output (1, stdout) su cui scrivere
	// standard error (2, stderr) su cui scrivere messaggi di errore o diagnostici

	// tipicamente sono "attaccati" al terminale da cui viene avviato il processo
	// quindi: input da tastiera, output (1 e 2) su schermo

	// DA EMULATORE TERMINALE:
	// ctrl+d per inviare EOF

	if (1) {

		c = getchar();

		while (c != EOF) {

			counter++;

			// utilizza c
			// ....

			c = getchar();
		}

	} else {

		// equivalente a:

		while ((c = getchar()) != EOF) {

			counter++;

			// utilizza c
			// ....

		}

	}

	printf("\nho letto %lu caratteri\n", counter);

	/*
	 * seconda parte dell'esercizio:
	 *
	 * reindirizzare stdin:
	 *
	 * 005.1getchar < prova.txt
	 *
	 */

	return 0;
}

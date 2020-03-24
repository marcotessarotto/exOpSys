#include <stdio.h>


int main(int argc, char *argv[]) {

	int c;

	// DA EMULATORE TERMINALE:
	// ctrl+d per inviare EOF

	if (1) {

		c = getchar();

		while (c != EOF) {

			putchar(c);

			c = getchar();
		}

	} else {

		// equivalente a:

		while ((c = getchar()) != EOF) {
			putchar(c);
		}

	}

}

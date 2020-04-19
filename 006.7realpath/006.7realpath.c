#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

/*

risolve percorsi di file in percorsi assoluti senza . .. link simbolici

passare i percorsi da elaborare come argomenti del programma

"." -> "/home/utente/git/exOpSys/006.7realpath"
".." -> "/home/utente/git/exOpSys"
"./../.." -> "/home/utente/git"

 */

int main(int argc, char * argv[]) {

	char * cwd;

	cwd = getcwd(NULL, 0); // get current working directory of process

	printf("process current working directory: %s\n", cwd);

	free(cwd);

	if (argc == 1) {

		cwd = realpath(".", NULL);

		if (cwd == NULL) {
			perror("realpath()");
			exit(EXIT_FAILURE);
		}

		printf("\"%s\" -> \"%s\"\n", ".", cwd);

		free(cwd);
	} else {

		for (int i = 1; i < argc; i++) {
			cwd = realpath(argv[i], NULL);

			if (cwd == NULL) {
				perror("realpath()");
				exit(EXIT_FAILURE);
			}

			printf("\"%s\" -> \"%s\"\n", argv[i], cwd);

			free(cwd);
		}

	}


	return 0;
}


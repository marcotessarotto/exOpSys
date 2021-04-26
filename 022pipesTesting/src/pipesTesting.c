
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// https://stackoverflow.com/questions/1312922/detect-if-stdin-is-a-terminal-or-pipe?noredirect=1&lq=1
// https://stackoverflow.com/questions/16305971/how-to-read-piped-content-in-c


int main(void) {
	// https://stackoverflow.com/questions/4978056/char-array-declaration-and-initialization-in-c

	char input_buffer[256];
	char * name;

	// fileno(stdin)) == STDIN_FILENO

	if (isatty(STDIN_FILENO)) {
	    printf( "STDIN_FILENO is a terminal\n" );

	    name = ttyname(STDIN_FILENO);

	    if (name != NULL) {
	    	printf("%s\n", name);
	    }

		while (1) {
			int res;

			// operazione sincrona
			res = scanf("%s",input_buffer);

			if (res < 1)
				break;

			printf("incoming data from STDIN_FILENO: %s\n", input_buffer);

			// architettura ad eventi? chiedo di leggere dalla stdin, quando dati pronti

		}


	} else {

		printf( "STDIN_FILENO is a file or a pipe\n");

		// leggiamo i dati nei modi che conosciamo già


//		int i = 0;
//		char pipe[65536];
//
//		char ch;
//
//		while ((ch = getc(stdin)) != -1) {
//			pipe[i++] = ch;
//
////			putc(ch, stdout);
//
//			if (ch == 10) {
//				pipe[i] = '\0';
//				fprintf(stdout,"piped content: >>%s<<\n", pipe);
//				i = 0;
//				fflush(stdout);
//			}
//		}
//
//
//
//		// let's count carriage returns
//
//		int countLines = 0;
//
//		for (int h = 0; pipe[h] != 0; h++) {
//			if (pipe[h] == 10)
//				countLines++;
//		}
//
//		fprintf(stdout,"newlines: %i\n", countLines);


	}

/*
marco@asusi7:~/eclipse-workspace-native/pipesTesting/Debug$ ls | ./pipesTesting
stdin is a file or a pipe
piped content: >>makefile
objects.mk
pipesTesting
sources.mk
src
<<
marco@asusi7:~/eclipse-workspace-native/pipesTesting/Debug$ echo "prova" | ./pipesTesting
stdin is a file or a pipe
piped content: >>prova
<<
marco@asusi7:~/eclipse-workspace-native/pipesTesting/Debug$ echo -n "prova" | ./pipesTesting
stdin is a file or a pipe
piped content: >>prova<<

 */

	printf("bye!\n");

	return EXIT_SUCCESS;
}

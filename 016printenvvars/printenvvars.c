#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;  /* Or define _GNU_SOURCE to get it from <unistd.h> */

int main(int argc, char *argv[])
{
	printf("program: %s\n", argv[0]);

	printf("environment:\n\n");

    char **ep;

    for (ep = environ; *ep != NULL; ep++)  {
        puts(*ep);

#ifdef SPLIT
        char * str = strdup(*ep);
        char s[] = "="; // caratteri separatori da utilizzare per la suddivisione
        char *token;

        // ottieni il primo token
        token = strtok(str, s);

        // "cammina" attraverso gli altri token
        while (token != NULL ) {
           printf( "+++%s\n", token );

           token = strtok(NULL, s); // le chiamate successive a strtok
        }
#endif


    }

    exit(EXIT_SUCCESS);
}

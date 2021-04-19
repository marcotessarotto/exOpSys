#include <stdio.h>
#include <stdlib.h>

extern char **environ;  /* Or define _GNU_SOURCE to get it from <unistd.h> */

int main(int argc, char *argv[])
{
	printf("program: %s\n", argv[0]);

	printf("environment:\n\n");

    char **ep;

    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);

    exit(EXIT_SUCCESS);
}

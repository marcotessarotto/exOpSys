#include <stdio.h>
#include <stdlib.h>

extern char **environ;  /* Or define _GNU_SOURCE to get it from <unistd.h> */

int main(int argc, char *argv[])
{
    char **ep;

    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);

    exit(EXIT_SUCCESS);
}

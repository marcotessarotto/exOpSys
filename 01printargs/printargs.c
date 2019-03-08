#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    int j;
    // scriviamo su stdout tutti gli argomenti a riga di comando del programma
    for (j = 0; j < argc; j++)
        printf("argv[%d] = %s\n", j, argv[j]);

    //
	pid_t pid;

	pid = getpid(); // il nostro process id

	printf("il mio pid è: %d\n", pid);


	//
    printf("hello world!\n"); // utilizza stdout

    fprintf(stdout,"stessa cosa\n");
    //

    int v = 10;

    fprintf(stdout,"v= %d altro valore %d \n", v, 20);

    exit(EXIT_SUCCESS);
}

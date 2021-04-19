#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * questo esempio esegue un altro programma all'interno del processo corrente
 *
 * il programma eseguito dal processo corrente verrà rimpiazzato
 * dal programma contenuto nel file specificato
 *
 * ricordare: processo = istanza del programma
 *
 * il processo rimane lo stesso, ma diventa istanza di un altro programma
 */

#define USE_MY_CUSTOM_ENV 1

// see example 016printenvvars
// array di stringhe di caratteri
// la conclusione dell'array è segnalata da NULL
extern char **environ;  /* Or define _GNU_SOURCE to get it from <unistd.h> */


int main(int argc, char * argv[]) {

	char * program_filename = "../016printenvvars/Debug/016printenvvars";


	// parametri per il programma che invocheremo con execve
	char * new_arguments[] = { program_filename , NULL };

	// variabili di environment per il programma che invocheremo con execve
	char * new_environment_variables[] = {
			"PROVA=ok",
			//"CIAO=",
			NULL };

	char * current_working_directory = getcwd(NULL,0);

	printf("cartella corrente: %s\n", current_working_directory);


	printf("ora proviamo ad eseguire il programma %s\n"
			"nel processo corrente (fate prima il build del progetto)\n\n\n", program_filename);


	/*
	 * execve() executes the  program referred to by pathname.
	 *
	 * This causes the program that is currently being run by
	 * the calling process to be replaced with a new program ...
	 *
	 */

	// l'esecuzione ripartirà dal main() del nuovo programma
	// utilizziamo le stesse variabili di environment del programma corrente

	char ** envv = USE_MY_CUSTOM_ENV ? new_environment_variables : environ;

//	if (USE_MY_CUSTOM_ENV)
//		envv = new_environment_variables;
//	else
//		envv = environ;

	// sudo apt install cdecl
	// cdecl explain "char *const __argv[]"

	execve(program_filename, new_arguments,
			envv);

	// On success, execve() does not return, on error -1 is returned, and errno is set appropriately.

	perror("execve");
	// The perror() function produces a message on standard error describing the last error
	// encountered during a call to a system or library function.

	// https://www.tldp.org/LDP/abs/html/exitcodes.html

	return 1;
}

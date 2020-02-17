#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * esempio tratto pari pari dalla documentazione di qsort(3)
 *
 * esempio di utilizzo:
26qsortstr zz mm aa
output:
aa
mm
zz

esiste anche il comando sort che: 1) legge le righe da stdin OPPURE 2) legge le righe di un file il cui nome è passato come parametro
e restituisce le righe ordinate.
 *
 */


static int
cmpstringp(const void *p1, const void *p2)
{
   /* The actual arguments to this function are "pointers to
	  pointers to char", but strcmp(3) arguments are "pointers
	  to char", hence the following cast plus dereference */

   return strcmp(* (char * const *) p1, * (char * const *) p2);
}

int
main(int argc, char *argv[])
{
   int j;

   if (argc < 2) {
	   fprintf(stderr, "Usage: %s <string>...\n", argv[0]);
	   exit(EXIT_FAILURE);
   }

   qsort(&argv[1], argc - 1, sizeof(char *), cmpstringp);

   for (j = 1; j < argc; j++)
	   puts(argv[j]);
   exit(EXIT_SUCCESS);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char *argv[]) {

    int fd;

    printf("questo esempio è equivalente ad eseguire il seguente comando da shell:\n");
    printf("ls -l > output.txt\n");

    // creiamo (troncando eventuale contenuto già esistente) il file "output.txt", nella cartella corrente del processo
    if ((fd = open("output.txt", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR)) == -1) {

    	perror("problema con open");

    	exit(EXIT_FAILURE);
    }

    printf("valore di fd: %d\n", fd);

    // close(1);
    // se facessi questo close, il file descriptor 1 non corrisponde ad alcun file aperto
    // si dice che fd=1 è inutilizzato
    // se è inutilizzato, lo posso riutilizzare

    // dup2(3, 1):
    // dup2 fa close(1) e poi riutilizza 1, come clone di fd=3

    // chiudiamo file descriptor 1 (associato a stdout sul terminale)
    // e lo riutilizziamo come clone del descrittore fd che "punta" al file "output.txt"

    if (dup2(fd, STDOUT_FILENO) == -1) {
    	perror("problema con dup2");

    	exit(EXIT_FAILURE);
    }
    // dopo dup2, i due file descriptor 3 ed 1 "puntano" allo stesso file aperto (output.txt)

    // fd=3 non mi serve più
    // attenzione che il file output.txt rimane aperto
    close(fd);

	char * newargv[] = { "ls", "-l" /*, "/home/utente"*/, NULL };

	char * newenviron[] = { NULL };

	/*
	 * il programma /bin/ls scrive il suo output su file descriptor 1
	 *
	 * /bin/ls verrà eseguito con i seguenti file descriptor aperti:
	 * 0 stdin
	 * 1 file "output.txt" (nella cartella corrente) [al posto di stdout]
	 * 2 stderr
	 *
	 * quindi, troveremo l'output di /bin/ls nel file output.txt
	 */

	execve("/bin/ls", newargv, newenviron);

	perror("problema con execve\n");

	exit(EXIT_FAILURE);


}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void writemsg2stdout(char * msg) {
	write(STDOUT_FILENO, msg, strlen(msg) );
}

#define BUF_SIZE 1024

// se questo define è abilitato, vengono lanciati i processi in questo modo:
// ls -l /home/utente | wc
#define USE_EXEC

int main(int argc, char * argv[]) {

	int pfd[2];
	char buf[BUF_SIZE];

	// file handlers già aperti:
	// 0: standard input
	// 1: standard output
	// 2: standard error

	if (pipe(pfd) == -1) {
		perror("problema con pipe");

		exit(EXIT_FAILURE);
	}

	// pipe crea una struttura di comunicazione dati unidirezionale gestita dal kernel
	// pipe è descritta da un file descriptor di sola lettura ed uno di sola scrittura (le due estremità del "tubo")

	// pfd[0]: file descriptor per LEGGERE dalla PIPE
	// pfd[1]: file descriptor per SCRIVERE nella PIPE

	printf("file handler pipe, estremità di lettura: %d\n", pfd[0]); // dovrebbe valere 3 (se i descrittori 0,1,2 sono utilizzati)
	printf("file handler pipe, estremità di scrittura: %d\n", pfd[1]); // dovrebbe valere 4

	switch (fork()) {
		case -1:
			perror("problema con fork");

			exit(EXIT_FAILURE);

		case 0: // processo FIGLIO: leggerà dalla PIPE

			close(pfd[1]); // chiudiamo l'estremità di scrittura della pipe, non ci serve

#ifdef USE_EXEC


			if (dup2(pfd[0], STDIN_FILENO) == -1) { // chiude fd 0; riapre fd 0 e lo rende identico a fd 3 (cioè fd 3 e fd 0 puntano allo stesso "file", in questo caso l'estremità di una pipe)
				perror("problema con dup2");
				exit(EXIT_FAILURE);
			}

			close(pfd[0]); // non ci serve più, perchè a wc passeremo fd 0 (che ora è l'estremità read della pipe)


			// al programma wc passeremo i seguenti file descriptor aperti:
			// 0: l'estremità di lettura della pipe (al posto di stdin)
			// 1: stdout
			// 2: stderr
			// wc lavorerà col fd 0 senza sapere che non è stdin, è completamente trasparente
			execlp("wc", "wc", (char * ) NULL); // execlp utilizza la system call execve

			// se la chiamata execlp funziona, non si passa più di qua; altrimenti si è verificato un problema
			perror("problema con execlp(1)");

			exit(EXIT_FAILURE);

#else

			while (1) {

				int numRead = read(pfd[0], buf, BUF_SIZE);

				if (numRead == -1) {
					perror("errore in read");
					exit(EXIT_FAILURE);
				}

				if (numRead == 0)
					break; // EOF: la pipe è stata chiusa dal lato di scrittura

				write(STDOUT_FILENO, buf, numRead);

			}

			close(pfd[0]); // chiudiamo l'estremità di lettura della pipe, ora il kernel libera tutta la struttura di pipe

			writemsg2stdout("*C* pipe EOF, prima di exit\n");

			//printf("ciao\n");

			exit(EXIT_SUCCESS); // fine del processo figlio
#endif

		default:
			// processo PADRE: scriverà dentro la PIPE

			close(pfd[0]); // chiudiamo l'estremità di lettura della pipe, non ci serve

#ifdef USE_EXEC

			if (dup2(pfd[1], STDOUT_FILENO) == -1) { // chiude fd 1; riapre fd 1 e lo rende identico a fd 4 (cioè fd 1 e fd 4 puntano allo stesso file...)
				perror("problema con dup2");
				exit(EXIT_FAILURE);
			}

			close(pfd[1]); // pfd[1] == 4


			execlp("ls", "ls", "-l", "/home/utente", (char *) NULL);

			// di qua non si passa più, a meno di errori

			perror("problema con execlp(2)");

			exit(EXIT_FAILURE);

#else


			char * msg = "messaggio per il processo figlio\n";

			int len = strlen(msg);

			write(pfd[1], msg, len); // scriviamo sull'estremità di scrittura della pipe

			sleep(2);

			write(pfd[1], msg, len);

			sleep(2);

			writemsg2stdout("*P* prima di close pipe[1]\n");

			close(pfd[1]); // chiudiamo l'estremità di scrittura della pipe; il processo figlio "riceverà" EOF

			writemsg2stdout("*P* prima di wait\n");

			wait(NULL); // aspettiamo la terminazione del processo figlio

			writemsg2stdout("*P* prima di exit\n");

			exit(EXIT_SUCCESS);
#endif

	}

}



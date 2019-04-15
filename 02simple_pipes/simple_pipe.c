#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



#define BUF_SIZE 1024

#define USE_EXEC

int main(int argc, char * argv[]) {

	int pfd[2];
	char buf[BUF_SIZE];

	if (pipe(pfd) == -1) {
		perror("problema con pipe");

		exit(EXIT_FAILURE);
	}

	switch (fork()) {
	case 0: // processo FIGLIO

		close(pfd[1]); // chiudiamo l'estremità di scrittura, non ci serve

#ifdef USE_EXEC
		if (pfd[0] != STDIN_FILENO) {

			dup2(pfd[0], STDIN_FILENO);

			close(pfd[0]);
		}

		execlp("wc", "wc", (char * ) NULL);

		// se la chiamata funziona, non si passa più di qua
#endif

		while (1) {

			int numRead = read(pfd[0], buf, BUF_SIZE);

			if (numRead == -1) {
				perror("errore in read");
				exit(EXIT_FAILURE);
			}

			if (numRead == 0)
				break;

			write(STDOUT_FILENO, buf, numRead);

		}

		close(pfd[0]);

		char * msg3 = "*C* pipe EOF\n";

		write(STDOUT_FILENO, msg3, strlen(msg3) );

		//printf("ciao\n");

		exit(EXIT_SUCCESS);

	default:
		// processo PADRE

		close(pfd[0]); // chiudiamo l'estremità di lettura della pipe, non ci serve

#ifdef USE_EXEC
		if (pfd[1] != STDOUT_FILENO) {

			dup2(pfd[1], STDOUT_FILENO);

			close(pfd[1]);
		}

		execlp("ls", "ls", "-l", "/home/utente", (char *) NULL);

		// di qua non si passa più, a meno di errori
#endif


		char * msg = "messaggio per il processo figlio\n";

		int len = strlen(msg);

		write(pfd[1], msg, len); // scriviamo sull'estremità di scrittura della pipe

		sleep(2);

		write(pfd[1], msg, len);

		sleep(2);

		close(pfd[1]);

		char * msg2 = "*P* prima di wait\n";

		write(STDOUT_FILENO, msg2, strlen(msg2) );

		wait(NULL); // aspettiamo la terminazione del processo figlio

		exit(EXIT_SUCCESS);

	}

}



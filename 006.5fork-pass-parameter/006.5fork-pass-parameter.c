#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * questo esempio mostra come passare uno o più parametri ad un processo figlio
 *
 */


int child_process(int a, int b) {

	printf("[child]sono il processo figlio %d, i parametri che ho ricevuto sono: %d %d\n", getpid(), a, b);


	exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[]) {

	int param1;

	int param2;

	// avviamo un processo figlio e gli passiamo dei parametri
	// il processo figlio (clone del processo padre) "vede" le stesse variabili del processo padre
	// ovvero vede una copia dei valori

	// quindi posso utilizzare delle variabili per passare dei parametri al processo figlio
	// in questo caso sono param1 e param2


	param1 = 100;
	param2 = 200;

	switch (fork()) {
		case 0:
			child_process(param1, param2);
			// child_process non ritorna

			break;
		case -1:

			break;
		default:
			;
	}

	sleep(3);

	printf("\n\n\n");

	////////

	for (int i = 0; i < 10; i++) {

		param1 = 100 * i;
		param2 = 200 * i;

		switch (fork()) {
			case 0:
				child_process(param1, param2);
				// child_process non ritorna

				break;
			case -1:
				printf("errore fork()!\n");
				break;
			default:
				;
		}
	}

	//

	int counter = 0;

	while (wait(NULL) != -1) { // aspetto tutti i processi figli e li conto
		counter++;
	}

	printf("\n\n\n");

	printf("[parent]counter=%d, bye!\n", counter);

	return 0;
}

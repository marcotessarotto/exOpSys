#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DEBUG 0

int coda[] = { -1, -1, -1 };

// pulisce la coda
void clean_coda() {
	for (int i = 0; i < 3; i++)
		coda[i] = -1;
}

// per debug
void print_coda() {
	for (int i = 0; i < 3; i++)
		printf("coda[%d]=%d ", i, coda[i]);
	printf("\n");
}

/**
 * scrive i caratteri validi presenti nella coda, poi svuota la coda
 */
void flush_coda() {

	if (DEBUG)
		print_coda();

	for (int i = 0; i < 3; i++)
		if (coda[i] != -1) {
			if (DEBUG)
				printf("F[%d]%c\n", i, coda[i]);
			else
				printf("%c", coda[i]);
		}

	clean_coda();
}

/*
 * rimuove l'ultimo elemento aggiunto alla coda
 */
void rollback_coda() {
	coda[2] = coda[1];
	coda[1] = coda[0];
	coda[0] = -1;
}

/*
 * stampa il carattere alla posizione 0 della coda (se è un char valido),
 * poi aggiunge ch alla coda
 */
void my_putchar(int ch) {

	if (DEBUG)
		printf("my_putchar %c\n", ch);

	if (coda[0] != -1) {
		if (DEBUG)
			printf("P[%d]%c\n", coda[0], coda[0]);
		else
			printf("%c", coda[0]);
	}

	coda[0] = coda[1];
	coda[1] = coda[2];
	coda[2] = ch;
}

/*
 * confronto la coda con la stringa
 */
int cmp_coda(char * str) {
	for (int i = 0; i < 3; i++)
		if (coda[i] != str[i])
			return 0;

	return -1;
}

// prova per regola 3
void regola3() {

	int ch;

	while ((ch = getchar()) != EOF) {

		my_putchar(ch);

		if (cmp_coda("xyz")) {
			clean_coda();
//			my_putchar('*');
//			my_putchar('*');
//			my_putchar('*');
			// oppure
			printf("***");

		}

	}


}


#define NON_RIPETIZIONI 0
#define RIPETEZIONI 1


/*
0 – non fare alcuna ipotesi sulla lunghezza dell’input
1 - carattere alfabeto maiuscolo -> carattere minuscolo
2 - carattere alfabeto ch che si ripetono n volte: scrivere, al posto della
sequenza di n caratteri ch, il numero intero n e poi il carattere ch.
3 - sostituire "xyz" con "***"
 */


void esercizio2(void) {

	printf("esercizio 2 - 29/03/2019\n");

	printf("esempi di input:\n");

	printf("000123456789aaaaaqwerty\n");

	int ch;
	int prev_ch = -1;
	int contatore_ripetizioni = 0;

	int stato = NON_RIPETIZIONI;


	while ((ch = getchar()) != EOF) {

		ch = tolower(ch);

		if (ch == prev_ch) {
			if (stato == NON_RIPETIZIONI) {
				contatore_ripetizioni = 2;
				stato = RIPETEZIONI;

				if (DEBUG)
					printf("\ncambio di stato: RIPETIZIONI\n");

				rollback_coda();
				flush_coda();
			} else {
				contatore_ripetizioni++;
			}
		} else {
			if (stato == RIPETEZIONI) {
				stato = NON_RIPETIZIONI;

				if (DEBUG)
					printf("\ncambio di stato: NON_RIPETIZIONI\n");

				if (prev_ch != '\n')
					printf("%d%c", contatore_ripetizioni, prev_ch);
				else
					printf("%d\\n", contatore_ripetizioni);

				if (DEBUG)
					printf("\n");

				my_putchar(ch);
			} else {
				my_putchar(ch);

				if (cmp_coda("xyz")) {
					clean_coda();

					printf("***");

				}

			}
		}

		prev_ch = ch;


	}


}



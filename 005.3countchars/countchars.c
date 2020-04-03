#include <stdio.h>


int main(int argc, char *argv[]) {

	// ESERCIZIO 1:
	// leggi da stdin fino a EOF e conta i caratteri: '\n', '\t' e ' ' (spazio)
	// dopo EOF, scrive su stdout il numero di occorrenze di ciascuno dei tre caratteri sopra

	int ch;

 	int char_occurrence[256] = { 0 };

	// attenzione! in C gli indici negativi per accedere agli array sono permessi
	// https://stackoverflow.com/q/3473675/974287
	// provare con 😸

 	/*
 	ch = -120;
 	printf("ch: %d %d %d\n", ch, (unsigned)ch, (unsigned)ch & 0xFF);
 	printf("ch: %x %x %x\n", ch, (unsigned)ch, (unsigned)ch & 0xFF);
 	*/

	while ((ch = getchar()) != -1) {
		char_occurrence[(unsigned)ch & 0xFF]++;
	}

	printf("count('\\n') = %d\n", char_occurrence['\n']);
	printf("count('\\t') = %d\n", char_occurrence['\t']);
	printf("count(' ') = %d\n", char_occurrence[' ']);

//	for (int i=0; i < sizeof(char_occurrence) / sizeof(int); i++)
//		if (char_occurrence[i] != 0)
//			printf("count(%d)=%d\n", i, char_occurrence[i]);

	return 0;
}


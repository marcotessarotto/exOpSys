#include <stdio.h>

#include "char_io.h"


void copy_input_to_output() {

	/*
	 * pseudo codice:
	 * leggi un carattere
	 * while (carattere non indica la fine del file)
	 * 		scrivi il carattere appena letto
	 * 		leggi un carattere
	 *
	 */

	printf("copy_input_to_output\n");

	int c;

	c = getchar();  // restituisce un "unsigned char" [0,255] convertito in int
	// perchè dobbiamo usare int e non char?
	// perchè getchar, oltre a restituire un unsigned char, può anche restituire EOF!
	// ma EOF è definito (di solito) come -1. Quindi, 8 bit non bastano...

	while (c != EOF) { // != significa "non uguale"
		// EOF è definito come:
		// #define EOF (-1)
		putchar(c);
		c = getchar();
	}

	// equivalente a:
	// c = getc(stdin);

}

// come copy_input_to_output ma codice più compatto
void copy_input_to_output_2() {
	int c;

	while ((c = getchar()) != EOF) // precedenze operatori: libro pag. 53
		putchar(c);


	// l'espressione nel while viene valutata così:
	// 1 - viene chiamato getchar(), che restituisce un valore (int)
	// 2 - questo valore viene scritto nella variabile c
	// 3 - viene fatto il confronto tra c ed EOF:   c != EOF
	// se c è diverso da EOF allora l'operatore binario "!=" restituisce 1,
	// se c è uguale a EOF allora l'operatore != restituisce 0.

	// perchè ci sono le parentesi intorno a "(c = getchar())"?
	// per via delle precedenze tra operatori.
	// L'operatore != ha priorità più alta rispetto a =.
	// Ovvero, se scrivessi:   c = getchar() != EOF è come se scrivessi:
	// c = (getchar() != EOF)
	// cioè scriverei nella variabile c il valore 0 o 1,
	// a seconda del risultato del confronto tra il valore restituito tra
	// getchar() ed EOF.

	// Esercizi:
	// verificare che getchar() != EOF restituisca 0 o 1
	// fare un programma che scriva il valore di EOF.



}


void count_lines_in_input() {
	// TODO

}

void word_count() {
	// TODO

}

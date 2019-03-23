#include <stdio.h>
#include <stdlib.h>

#include "char_io.h"
#include "common_settings.h"


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

void count_characters() {

	long number_of_chars = 0;

	while (getchar() != EOF)
		++number_of_chars;

	printf("numero di caratteri letti: %ld", number_of_chars);

}

void mini_esempio_con_puntatori() {


	int a = 0;
	int b;

	b = a++;
	/*
	 * b = a;
	 * a = a +1;
	 */

	printf("b vale %d\n",b);

	b = ++a;
	/*
	 * a = a + 1;
	 * b = a;
	 */

	printf("b vale %d\n",b);

}

void count_lines_in_input() {

	int c;
	int numero_linee = 0;

	while ((c = getchar()) != EOF)
		if (c == '\n')
			numero_linee++;

	char spazio_vuoto = ' ';

	// scrivere il risultato

}

enum STATO_PAROLA {
	IN, OUT
};


void word_count() {

	int c;
	int numero_linee = 0;
	int numero_parole = 0;
	int numero_caratteri = 0;

	enum STATO_PAROLA stato = OUT;


	printf("word_count()\n");

	if (is_debug_enabled())
		printf("prima del while\n");

	while ((c = my_getchar()) != EOF) {

		if (is_debug_enabled())
			printf("ho letto il carattere %d\n",c);

		numero_caratteri++;

		if (c == '\n')
			numero_linee++;

		if (c == ' ' || c == '\n' || c == '\t') {
			stato = OUT;
		} else if (stato == OUT) {
			stato = IN;
			numero_parole++;
		}
	}

	// scrivere i risultati
	printf("numero_caratteri = %d\n", numero_caratteri);
	printf("numero_parole = %d\n", numero_parole);
	printf("numero_linee = %d\n", numero_linee);
}

void count_to_array() {

	int c;
	int numero_white_space = 0;
	int numero_altri_caratteri = 0;

	int numero_cifre[10];

	for (int i = 0; i < 10; i++)
		numero_cifre[i] = 0;

	while ((c = getchar()) != EOF) {
		if (c >= '0' && c <= '9')
			numero_cifre[c - '0']++;
		else if (c == ' ' || c == '\n' || c == '\t')
			numero_white_space++;
		else
			numero_altri_caratteri++;
	}

	printf("risultati:\n");
	for (int i = 0; i < 10; i++)
		printf("contatore '%d' = %d\n", i, numero_cifre[i]);

	printf("numero_white_space = %d\n", numero_white_space);
	printf("numero_altri_caratteri = %d\n", numero_altri_caratteri);


}

// dichiarazione
//int power(int m, int n);


//int altra_funzione() {
//	power(a,b);
//}
//
//// definizione
//int power(int base, int n) {
//
//
//
//}





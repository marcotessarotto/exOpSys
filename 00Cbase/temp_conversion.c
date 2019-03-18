#include <stdio.h>

#include "temp_conversion.h"


/*
conversione da temperatura in gradi Fahrenheit a gradi Celsius:

C = 5/9 * (F-32)

l'output del programma sarà:

conversione da temperatura in gradi Fahrenheit a gradi Celsius:
C = 5/9 * (F-32)
0	-17
20	-6
40	4
60	15
80	26
100	37
120	48
140	60
160	71
180	82
200	93
220	104
240	115
260	126
280	137
300	148

 */

// questa è la dichiarazione di funzione
void temperature_conversion_int() {

	printf("conversione da temperatura in gradi Fahrenheit a gradi Celsius (usando int):\n");

	printf("C = 5/9 * (F-32)\n");

	int fahr, celsius; // DICHIARAZIONE di variabili LOCALI, visibili soltanto all'interno di questa funzione
	int lower, upper, step; // allocate sulla "stack"
	// int è un intero con segno, 32 bit (4 byte)

	// istruzioni di assegnazione di valore
	lower = 0;   /* lower limit of temperature table */
	upper = 300; /* upper limit */
	step = 20;  /* step size */

	// scriviamo una tabella di valori
	fahr = lower; // prendo il valore della variabile "lower" e lo copio nella variabile "fahr"

	// intestazione della tabella
	printf("\nF\tC\n");

	// "mentre" la condizione è "vera" (cioè l'espressione calcolata a runtime è diversa da zero) itera sul blocco di codice tra le graffe

	// "fahr <= upper" è una espressione, che valore viene calcolato?
    //printf("%d\n", fahr <= upper); // 0 se la condizione è falsa, 1 se è vera

	while (fahr <= upper) { // tutto il blocco di codice tra { e } viene eseguito

		// attenzione: "indentare" (cioè inserire i rientri) il codice in maniera opportuna!!
		// così il codice è più leggibile! IMPORTANTE!

		// sequenza delle operazioni (result è una variabile fittizia che serve solo a spiegare, non esiste in realtà):
		// 1: result = fahr-32
		// 2: result = 5 * result
		// 3: result = result / 9

		// l'operazione 5 / 9 "tronca" la parte frazionale, per questo motivo ho riorganizzato l'espressione

		celsius = 5 * (fahr-32) / 9; // perchè ho messo "/ 9" in fondo?

		// problema generale: questo algoritmo, quando usa numeri interi, è POCO PRECISO

		// printf è una funzione di GLIBC per produrre output formattato
		// l'output è determinato dalla stringa di formattazione, che prevede degli argomenti denotati dal
		// carattere '%'
		// ogni parametro % si aspetta un corrispondente parametro della funzione dopo la stringa di formattazione.
		printf("%d \t %d \n", fahr, celsius); // scrivo due numeri interi decimali, notare a \t e \n

		// l'output della tabella è un po' irregolare, per migliorare l'output possiamo usare:
		//printf("%3d \t %6d \n", fahr, celsius); // notare che gli spazi servono solo per rendere più leggibile la stringa di formattazione
		// %4d : 4 è la "width", cioè larghezza da utilizzare per formattare il numero, "giustificato a destra",
		// utilizzando quattro cifre.

		fahr = fahr + step;

	}

	printf("\n\n");

	/*

	printf("1 %d\n", 5 / 9); // cosa stamperebbe? attenzione che sto dividento due numeri interi

	printf("2 %f\n", 5 / 9); // cosa stamperebbe? printf si aspetta un valore float (floating point)

	printf("3 %f\n", 5 / 9.);

	printf("4 %f\n", 5. / 9);

	printf("5 %f\n", 5. / 9);

	printf("6 %d\n", 5. / 9);
	*/

}

////////

// introduciamo delle "costanti simboliche" per rendere più leggibile il codice
#define LOWER 0
#define UPPER 300
#define STEP 20


void temperature_conversion_float() {

	printf("conversione da temperatura in gradi Fahrenheit a gradi Celsius (usando float):\n");

	printf("C = 5/9 * (F-32)\n");

	float fahr, celsius;
	int lower, upper, step;
	// float è un intero con decimali dopo la virgola, 32 bit (4 byte)

	// istruzioni di assegnazione di valore
	lower = LOWER;   /* lower limit of temperature table */
	upper = UPPER; /* upper limit */
	step = STEP;  /* step size */

	// scriviamo una tabella di valori
	fahr = lower;

	// intestazione della tabella
	printf("\nF\tC\n");

	while (fahr <= upper) {

		celsius = (5.0 / 9.0) * (fahr-32.0);

		printf("%3.0f \t %6.1f \n", fahr, celsius);

		// %3.0f: il numero in virgola mobile deve essere largo almeno 3 caratteri, con zero decimali
		// %6.1f: il numero in virgola mobile deve essere largo almeno 6 caratteri, con una cifra decimale

		fahr = fahr + step; // fahr è di tipo float, step è int..... .float + int ?

	}

/*
%d		scrivi un intero decimale
%6d 	scrivi un intero decimale, largo almeno 6 caratteri
%f      scrivi un numero in virgola mobile
#6f     scrivi un numero in virgola mobile largo almeno 6 caratteri
%.2f    scrivi un numero in virgola mobile con due cifre decimali dopo la virgola
%6.2f   scrivi un numero in virgola mobile largo almeno 6 caratteri e con due cifre decimali
 */

/*
 * %o   intero in base 8
 * %x   intero in base 16
 * %c   carattere
 * %s   stringa di caratteri
 * %%   il simbolo %
 */
	/*
	 * ESERCIZIO:
	 * scrivere una funzione che calcoli la conversione da gradi Celsius in Fahrnenhei.
	 */



	printf("\n\n");

}


void temperature_conversion_float_for() {

	printf("conversione da temperatura in gradi Fahrenheit a gradi Celsius (usando float e for):\n");

	printf("\nF\tC\n");

	for (int fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP)
		printf("%3d \t %6.1f \n", fahr, (5.0 / 9.0) * (fahr-32));

	printf("\n\n");

}



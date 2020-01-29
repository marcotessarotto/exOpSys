#include <stdio.h>

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

// introduciamo delle "costanti simboliche" per rendere più leggibile il codice
// vengono elaborate dal preprocessore
#define LOWER 0
#define UPPER 300
#define STEP 20

/*
 * esercizio: scrivere la funzione:
 *
 * int fahr_to_celsius(int fahr);
 *
 */


//void convert_fahr_to_celsius(void) {
//
//	int celsius;
//	int lower, upper, step;
//
//	lower = LOWER;
//	upper = UPPER;
//	step = STEP;
//
//	for (int fahr = lower; fahr <= upper; fahr += step) { // variable fahr dichiarata qui
//
//		int celsius = 5 * (fahr - 32) / 9;
//
//		printf("%d\t%d\n", fahr, celsius); // stringa di formattazione, parametro1, parametro2
//	}
//
//}


void convert_fahr_to_celsius() {

	printf("conversione da temperatura in gradi Fahrenheit a gradi Celsius (usando int):\n");

	printf("C = 5/9 * (F-32)\n");

	int fahr, celsius; // DICHIARAZIONE di variabili LOCALI, visibili soltanto all'interno di questa funzione
	int lower, upper, step; // allocate sulla "stack"
	// su architettura x86-64: int è un intero con segno, 32 bit (4 byte)

	lower = LOWER;   /* lower limit of temperature table */
	upper = UPPER; /* upper limit */
	step = STEP;  /* step size */

	fahr = lower;

	// intestazione della tabella
	printf("\nF\tC\n");

	while (fahr <= upper) {

		celsius = 5 * (fahr-32) / 9;

		// celsius = 5 / 9 * (fahr-32); // che differenza c'è?

		// printf è una funzione di GLIBC per produrre output formattato
		// l'output è determinato dalla stringa di formattazione, che prevede degli argomenti denotati dal
		// carattere '%'
		// ogni parametro % si aspetta un corrispondente parametro della funzione dopo la stringa di formattazione.
		printf("%d \t %d \n", fahr, celsius); // scrivo due numeri interi decimali, notare l'uso di \t e \n

		// l'output della tabella è un po' irregolare, per migliorare l'output possiamo usare:
		//printf("%3d \t %6d \n", fahr, celsius); // gli spazi servono solo per rendere più leggibile la stringa di formattazione
		// %4d : 4 è la "width", cioè larghezza da utilizzare per formattare il numero, "giustificato a destra",
		// utilizzando quattro cifre.

		fahr = fahr + step;

	}

	// alternativa al costrutto while:
	//	for (int fahr = lower; fahr <= upper; fahr += step) { // variable fahr dichiarata qui
	//
	//		int celsius = 5 * (fahr - 32) / 9;
	//
	//		printf("%d\t%d\n", fahr, celsius); // stringa di formattazione, parametro1, parametro2
	//	}

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



int main(int argc, char **argv) {

	convert_fahr_to_celsius();

	return 0;
}

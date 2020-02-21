#include <stdio.h>

/*
conversione da temperatura in gradi Fahrenheit a gradi Celsius:

C = 5/9 * (F-32)

l'output del programma sarà:

conversione da temperatura in gradi Fahrenheit a gradi Celsius:
C = 5/9 * (F-32)
  0 	  -17.8
 20 	   -6.7
 40 	    4.4
 60 	   15.6
 80 	   26.7
100 	   37.8
120 	   48.9
140 	   60.0
160 	   71.1
180 	   82.2
200 	   93.3
220 	  104.4
240 	  115.6
260 	  126.7
280 	  137.8
300 	  148.9

 */

// introduciamo delle "costanti simboliche" per rendere più leggibile il codice
// vengono elaborate dal preprocessore
#define LOWER 0
#define UPPER 300
#define STEP 20

/*
 * esercizio: scrivere la funzione:
 *
 * float fahr_to_celsius(float fahr);
 *
 */



void convert_fahr_to_celsius() {

	printf("conversione da temperatura in gradi Fahrenheit a gradi Celsius (usando int):\n");

	printf("C = 5/9 * (F-32)\n");

	float fahr, celsius; // DICHIARAZIONE di variabili LOCALI, visibili soltanto all'interno di questa funzione
	int lower, upper, step; // allocate sulla "stack"
	// su architettura x86-64: int è un intero con segno, 32 bit (4 byte); sizeof(float) = 32

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
		printf("%3.0f \t %6.1f \n", fahr, celsius); // scrivo due numeri float notare l'uso di \t e \n

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


}



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

int main(int argc, char **argv) {

	convert_fahr_to_celsius();

	return 0;
}

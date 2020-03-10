#include <stdio.h>

#define LOWER 0
#define UPPER 300
#define STEP 20


void convert_fahr_to_celsius() {

	// da qua in poi, premere F6 (step over) per eseguire un comando alla volta

	printf("conversione da temperatura in gradi Fahrenheit a gradi Celsius (usando int):\n");

	printf("C = 5/9 * (F-32)\n");

	int fahr, celsius;
	int lower, upper, step;

	lower = LOWER;   /* lower limit of temperature table */
	upper = UPPER; /* upper limit */
	step = STEP;  /* step size */

	fahr = lower;

	printf("\nF\tC\n");

	// continuare con "step over" (F6); controllare i valori delle variabili
	while (fahr <= upper) {

		celsius = 5 * (fahr-32) / 9;

		printf("%d \t %d \n", fahr, celsius);

		fahr = fahr + step;
	}

	// per fare continuare l'esecuzione senza stop, usare il comando "Resume": F8

	printf("\n\n");


}

void test() {
	printf("123\n");

	printf("un altro printf\n");
}

int main(int argc, char **argv) {

	// cominciare la sessione di debug con F11

	test();

	// l'esecuzione si ferma qui:
	// premere F5 (step into) per "entrare" nella funzione
	convert_fahr_to_celsius();


	return 0;
}

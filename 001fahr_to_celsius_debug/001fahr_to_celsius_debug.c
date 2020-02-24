#include <stdio.h>

#define LOWER 0
#define UPPER 300
#define STEP 20


void convert_fahr_to_celsius() {

	printf("conversione da temperatura in gradi Fahrenheit a gradi Celsius (usando int):\n");

	printf("C = 5/9 * (F-32)\n");

	int fahr, celsius;
	int lower, upper, step;

	lower = LOWER;   /* lower limit of temperature table */
	upper = UPPER; /* upper limit */
	step = STEP;  /* step size */

	fahr = lower;

	printf("\nF\tC\n");

	while (fahr <= upper) {

		celsius = 5 * (fahr-32) / 9;

		printf("%d \t %d \n", fahr, celsius);

		fahr = fahr + step;
	}

	printf("\n\n");


}


int main(int argc, char **argv) {

	convert_fahr_to_celsius();

	return 0;
}

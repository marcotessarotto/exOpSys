#include <stdio.h>



int main() {

	// static array

	int numbers[10];

	printf("scrivi 10 numeri interi: ");

	for (int i=0; i < 10; i++) {

		scanf("%d", &numbers[i]);

	}

	printf("\n");

	for (int i=0; i < 10; i++) {
		printf("numbers[%u]=%d\n", i, numbers[i]);
	}


	return 0;
}

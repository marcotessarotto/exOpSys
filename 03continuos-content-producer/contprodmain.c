
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main(int argc, char *argv[]) {

	int counter = -1;

	if (argc > 1) {
		sscanf(argv[1], "%d", &counter);
	}

	printf("counter = %d\n", counter);

	while (-1) {

		time_t now = time(NULL);

		printf("%ld\n", now);
//		fflush(stdout);

		sleep(1); // http://man7.org/linux/man-pages/man3/sleep.3.html

		if (counter > 0 && --counter == 0)
			break;

	}

	return EXIT_SUCCESS;
}

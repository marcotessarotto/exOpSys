#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


#define NUMBER_OF_TESTS 10000000L

int main(int argc, char * argv[]) {

	struct timeval tv1, tv2, tv;

	gettimeofday(&tv1, NULL);

	for (int i = 0; i < NUMBER_OF_TESTS; i++) {
		gettimeofday(&tv, NULL);
	}

	gettimeofday(&tv2, NULL);


	printf("%ld %ld\n", tv1.tv_sec, tv1.tv_usec); // start time
	printf("%ld %ld\n", tv2.tv_sec, tv2.tv_usec); // end time

	// https://stackoverflow.com/a/12722972/974287
	long dt = ((tv2.tv_sec - tv1.tv_sec) * 1000000L + tv2.tv_usec) - tv1.tv_usec;

	printf("total dt : %ld microseconds\n", dt);

	printf("NUMBER_OF_TESTS: %ld\n", NUMBER_OF_TESTS);

	printf("dt/NUMBER_OF_TESTS : %ld nanoseconds\n", dt * 1000 / NUMBER_OF_TESTS);

	return 0;

}

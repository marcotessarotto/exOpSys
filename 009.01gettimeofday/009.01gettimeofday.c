#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

/*
 * misura il tempo di esecuzione delle system call gettimeofday(), clock_gettime()
 *
 */


#define NUMBER_OF_TESTS 10000000L

int main(int argc, char * argv[]) {

	struct timeval tv1, tv2, tv;

	gettimeofday(&tv1, NULL);

	for (int i = 0; i < NUMBER_OF_TESTS; i++) {
		gettimeofday(&tv, NULL);
	}

	gettimeofday(&tv2, NULL);


	//printf("%ld %ld\n", tv1.tv_sec, tv1.tv_usec); // start time
	//printf("%ld %ld\n", tv2.tv_sec, tv2.tv_usec); // end time

	// https://stackoverflow.com/a/12722972/974287
	long dt = ((tv2.tv_sec - tv1.tv_sec) * 1000000L + tv2.tv_usec) - tv1.tv_usec;

	printf("***gettimeofday()***\n");
	printf("total dt = %ld microseconds\n", dt);
	printf("NUMBER_OF_TESTS: %ld\n", NUMBER_OF_TESTS);
	printf("dt/NUMBER_OF_TESTS = %ld nanoseconds\n", dt * 1000 / NUMBER_OF_TESTS);
	printf("\n\n");

	//
	// int clock_getres(clockid_t clockid, struct timespec *res);
	// The  function clock_getres() finds the resolution (precision) of the specified clock clockid
	struct timespec ts1;

	gettimeofday(&tv1, NULL);
	for (int i = 0; i < NUMBER_OF_TESTS; i++) {
		clock_gettime(CLOCK_REALTIME, &ts1);
	}
	gettimeofday(&tv2, NULL);

	dt = ((tv2.tv_sec - tv1.tv_sec) * 1000000L + tv2.tv_usec) - tv1.tv_usec;

	printf("***clock_gettime(CLOCK_REALTIME)***\n");
	printf("total dt : %ld microseconds\n", dt);
	printf("NUMBER_OF_TESTS: %ld\n", NUMBER_OF_TESTS);
	printf("dt/NUMBER_OF_TESTS : %ld nanoseconds\n", dt * 1000 / NUMBER_OF_TESTS);
	printf("\n\n");

/*
       CLOCK_REALTIME
              A  settable system-wide clock that measures real (i.e., wall-clock) time.

       CLOCK_REALTIME_COARSE (since Linux 2.6.32; Linux-specific)
              A  faster but less precise version of CLOCK_REALTIME.  This clock is not settable.
              Use when you need very fast, but not fine-grained timestamps.
              Requires per-architecture support, and probably also architecture support for this flag in  the
              vdso(7).

       CLOCK_PROCESS_CPUTIME_ID (since Linux 2.6.12)
              This  is a clock that measures CPU time consumed by this process
              (i.e., CPU time consumed by all threads in the process).

       CLOCK_THREAD_CPUTIME_ID (since Linux 2.6.12)
              This is a clock that measures CPU time consumed by this thread.
 */

	gettimeofday(&tv1, NULL);
	for (int i = 0; i < NUMBER_OF_TESTS; i++) {
		clock_gettime(CLOCK_REALTIME_COARSE, &ts1);
	}
	gettimeofday(&tv2, NULL);

	dt = ((tv2.tv_sec - tv1.tv_sec) * 1000000L + tv2.tv_usec) - tv1.tv_usec;

	printf("***clock_gettime(CLOCK_REALTIME_COARSE)***\n");
	printf("total dt : %ld microseconds\n", dt);
	printf("NUMBER_OF_TESTS: %ld\n", NUMBER_OF_TESTS);
	printf("dt/NUMBER_OF_TESTS : %ld nanoseconds\n", dt * 1000 / NUMBER_OF_TESTS);
	printf("\n\n");



	gettimeofday(&tv1, NULL);
	for (int i = 0; i < NUMBER_OF_TESTS; i++) {
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts1);
	}
	gettimeofday(&tv2, NULL);

	dt = ((tv2.tv_sec - tv1.tv_sec) * 1000000L + tv2.tv_usec) - tv1.tv_usec;

	printf("***clock_gettime(CLOCK_PROCESS_CPUTIME_ID)***\n");
	printf("total dt : %ld microseconds\n", dt);
	printf("NUMBER_OF_TESTS: %ld\n", NUMBER_OF_TESTS);
	printf("dt/NUMBER_OF_TESTS : %ld nanoseconds\n", dt * 1000 / NUMBER_OF_TESTS);
	printf("\n\n");



	gettimeofday(&tv1, NULL);
	for (int i = 0; i < NUMBER_OF_TESTS; i++) {
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts1);
	}
	gettimeofday(&tv2, NULL);

	dt = ((tv2.tv_sec - tv1.tv_sec) * 1000000L + tv2.tv_usec) - tv1.tv_usec;

	printf("***clock_gettime(CLOCK_THREAD_CPUTIME_ID)***\n");
	printf("total dt : %ld microseconds\n", dt);
	printf("NUMBER_OF_TESTS: %ld\n", NUMBER_OF_TESTS);
	printf("dt/NUMBER_OF_TESTS : %ld nanoseconds\n", dt * 1000 / NUMBER_OF_TESTS);
	printf("\n\n");


	return 0;

}



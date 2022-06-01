#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <stdatomic.h> // C11
#include <stdalign.h> // C11
#include <stdint.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <sys/time.h>
#include <pthread.h>


// derived from example in: man 2 futex

// n pthreads implement and use a mutex using stdatomic.h facilities and the futex syscall

// requires C11 dialect (for stdatomic.h)
// https://en.cppreference.com/w/c/atomic

// https://en.cppreference.com/w/c/language/_Alignas
static alignas(4) uint32_t futex1, futex2;
static int nloops;

#define AVAILABLE 1
#define NOT_AVAILABLE 0

static int futex(uint32_t *uaddr, int futex_op, uint32_t val,
		const struct timespec *timeout, uint32_t *uaddr2, uint32_t val3) {
	return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}

/* Acquire the futex pointed to by 'futexp': wait for its value to
 become 1, and then set the value to 0. */

static void fwait(uint32_t *futexp) {
	long s;

	/* atomic_compare_exchange_strong(ptr, oldval, newval)
	 atomically performs the equivalent of:

	 if (*ptr == *oldval)
	 *ptr = newval;

	 It returns true if the test yielded true and *ptr was updated. */

	while (1) {

		//printf("futex value: %d\n", *futexp);

		/* Is the futex available? */
		uint32_t one = AVAILABLE;

		// https://en.cppreference.com/w/c/atomic
		// https://en.cppreference.com/w/c/atomic/atomic_compare_exchange
		if (atomic_compare_exchange_strong(futexp, &one, NOT_AVAILABLE))
			break; /* Yes */

		/* Futex is not available; wait */

		// FUTEX_PRIVATE_FLAG: This option bit can be employed with all futex operations.
		// It tells the kernel that the futex is process-private and not shared with another process (i.e.,  it  is  being
        // used for synchronization only between threads of the same process).
		// This allows the kernel to make some additional performance optimizations.

		// check that *futexp == NOT_AVAILABLE and if so, sleep waiting for a FUTEX_WAKE operation
		s = futex(futexp, FUTEX_WAIT_PRIVATE, NOT_AVAILABLE, NULL, NULL, 0);
		if (s == -1 && errno != EAGAIN) {
			perror("futex-FUTEX_WAIT");
			exit(EXIT_FAILURE);
		}
	}
}

/* Release the futex pointed to by 'futexp': if the futex currently
 has the value 0, set its value to 1 and the wake any futex waiters,
 so that if the peer is blocked in fwait(), it can proceed. */

static void fpost(uint32_t *futexp) {
	long s;

	/* atomic_compare_exchange_strong() was described
	 in comments above */

	// https://en.cppreference.com/w/c/atomic/atomic_fetch_add

//#define VERSION1

#ifdef VERSION1

	const uint32_t zero = NOT_AVAILABLE;
	if (atomic_compare_exchange_strong(futexp, &zero, 1)) {

		// from man 2 futex:
		// This  operation wakes at most val of the waiters that are waiting (e.g., inside FUTEX_WAIT) on the futex word at the address uaddr.  Most commonly, val is specified as ei‐
        // ther 1 (wake up a single waiter) or INT_MAX (wake up all waiters).  No guarantee is provided about which waiters are awoken (e.g., a waiter with a higher scheduling prior‐
        // ity is not guaranteed to be awoken in preference to a waiter with a lower priority).

		// if previous value of *futexp was zero, wake up one waiter (thread)
		s = futex(futexp, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, 0);
		if (s == -1) {
			perror("futex-FUTEX_WAKE");
			exit(EXIT_FAILURE);
		}
	}

#else

	// this makes futexp a semaphore (allows values > 1)
	if (atomic_fetch_add(futexp, 1) == 0) {

		// if previous value of *futexp was zero, wake up one waiter (thread)
		s = futex(futexp, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, 0);
		if (s == -1) {
			perror("futex-FUTEX_WAKE");
			exit(EXIT_FAILURE);
		}
	}

#endif

}


void * pthread_func(void * arg) {

	uint64_t param = (uint64_t) arg;

	uint32_t * futex_a, * futex_b;

	if ((param & 1) == 0) {
		futex_a = &futex1;
		futex_b = &futex2;
	} else {
		futex_a = &futex2;
		futex_b = &futex1;
	}

    pid_t tid = gettid(); // man 2 gettid

	printf("starting thread %jd\n", (intmax_t) tid);

	for (int j = 0; j < nloops; j++) {
		fwait(futex_a);
		printf("thread  (%jd) %d\n", (intmax_t) tid, j);
		fpost(futex_b);
	}

	return NULL;
}


int main(int argc, char *argv[]) {
	setbuf(stdout, NULL);

	nloops = (argc > 1) ? atoi(argv[1]) : 15;

	futex1 = NOT_AVAILABLE; /* State: unavailable */
	futex2 = AVAILABLE; /* State: available */

	long nthreads = 4;

	if ((nthreads & 1) != 0) {
		printf("nthreads number must be even, we will get deadlock otherwise\n");
		exit(EXIT_FAILURE);
	}

	pthread_t tid[nthreads];

	printf("starting threads\n");

	for (int i = 0; i < nthreads; i++) {
		if (pthread_create(&tid[i], NULL, pthread_func, (void *) (uint64_t)i) != 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < nthreads; i++) {
		if (pthread_join(tid[i], NULL) != 0) {
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}
	}

	printf("all threads have joined\n");


	exit(EXIT_SUCCESS);
}

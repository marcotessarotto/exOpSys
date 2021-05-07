#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> // errno
#include <signal.h> // signal

#define N 10


void process_signal_handler(int signum) {

	if (signum == SIGALRM) {
		// do something...

		printf("[signal handler] ALARM!\n");

		exit(0);
	}

}


int main() {


	if (signal(SIGALRM, process_signal_handler) == SIG_ERR) {
		perror("signal");
		exit(EXIT_FAILURE);
	}

	printf("a SIGALRM signal will be delivered in %d seconds....\n", N);

	// set an alarm in N seconds
	if (alarm(N) == -1) {
		perror("alarm");
		exit(1);
	}

	printf("calling pause()\n");
	if (pause() == -1) {
		perror("pause");
		exit(1);
	}

	return 0;
}

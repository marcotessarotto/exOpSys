#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

/*
 * parent process creates a child process;
 * parent process waits for child process to complete a task, using signals
 *
 * child process performs a task N times; each time a task is completed, it sends a signal to parent process
 *
 * child and parent processes use SIGUSR1 to synchronize
 */

#define NUM_CHILD_TASKS 1

int delivered_signals;


long get_timestamp() {
	struct timespec ts1;
	clock_gettime(CLOCK_MONOTONIC, &ts1);

	long result = ts1.tv_sec * 1000000000L + ts1.tv_nsec;

	return result;
}


// SIGCHLD handler
static void sigchld_handler(int signum) {
	// NOTE: printf is used here only for debugging; do not use printf inside signal handlers
	printf("[handler %lu] received SIGCHLD\n", get_timestamp());
}

// SIGUSR1 handler
static void handler(int signum) {
	delivered_signals++;

	// NOTE: printf is used here only for debugging; do not use printf inside signal handlers
	printf("[handler %lu] received SIGUSR1\n", get_timestamp());
}

sigset_t block_signal(int signum) {
	sigset_t block_mask, orig_mask;

	sigemptyset(&block_mask);
	sigaddset(&block_mask, signum); // block_mask contains signum signal

	// block delivery of signum signals
	if (sigprocmask(SIG_BLOCK, &block_mask, &orig_mask) == -1) {
		perror("sigprocmask");
		exit(1);
	}

	return orig_mask;
}

sigset_t unblock_signal(int signum) {

	printf("unblock_signal %d\n", signum);
	sigset_t unblock_mask, orig_mask;

	sigemptyset(&unblock_mask);
	sigaddset(&unblock_mask, signum); // unblock_mask contains signum signal

	// unblock delivery of signum signals
	if (sigprocmask(SIG_UNBLOCK, &unblock_mask, &orig_mask) == -1) {
		perror("sigprocmask");
		exit(1);
	}

	return orig_mask;
}

// set signal handler with SA_RESTART flag
void set_signal_handler(int signum, void (*handler)()) {
	struct sigaction sa;

	sigemptyset(&sa.sa_mask);

	sa.sa_flags = SA_RESTART; // auto restart after delivery and execution of signal handler
	sa.sa_handler = handler;

	if (sigaction(signum, &sa, NULL) == -1) {
		perror("sigprocmask");
		exit(1);
	}

}

void send_signal(pid_t pid, int signum) {
	// this syscall fails if target process is dead
	if (kill(pid, signum) == -1) {
		perror("kill");
		exit(1);
	}
}

void child_process() {

	printf("[child   %lu] child process has started, pid=%d\n", get_timestamp(), getpid());

	pid_t parent_pid = getppid();


	for (int i = 0; i < NUM_CHILD_TASKS; i++) {
		printf("[child   %lu] starting task %d...\n", get_timestamp(), i);

		// we do something...
		if (NUM_CHILD_TASKS > 1)
			sleep(1);

		printf("[child   %lu] task %d is complete, sending signal to parent process\n", get_timestamp() , i);

		// signals can be lost if they are too near
		send_signal(parent_pid, SIGUSR1);
	}


	//sleep(1);


	printf("[child   %lu] child process is terminating\n", get_timestamp());

	exit(0);
}


void print_blocked_mask(sigset_t * set) {

	printf("print_blocked_mask: ");

	if (sigismember(set, SIGUSR1))
		printf("SIGUSR1 ");

	printf("\n");
}

int main(int argc, char **argv) {

	setbuf(stdout, NULL); // disable buffering of stdout

	pid_t child_pid;
	sigset_t orig_mask;

	printf("NUM_CHILD_TASKS = %d\n", NUM_CHILD_TASKS);
	printf("[parent  %lu] setting up signal mask, signal handlers\n", get_timestamp());

	sigprocmask(0, NULL, &orig_mask); // get current blocked signal mask
	print_blocked_mask(&orig_mask);

	// When a child process stops or terminates, SIGCHLD is sent to the parent process.
	// The default response to the signal is to ignore it.

	// block delivery of SIGUSR1 signals
	orig_mask = block_signal(SIGUSR1);
	// orig_mask is the original block mask, which does not contain SIGUSR1

	// set signal handler for SIGUSR1 signals with auto restart flag
	set_signal_handler(SIGUSR1, handler);

	// set signal handler for SIGCHLD signals
	set_signal_handler(SIGCHLD, sigchld_handler);

	printf("[parent  %lu] before fork\n", get_timestamp());
	child_pid = fork();

	switch (child_pid) {
	case -1:
		perror("fork");
		exit(1);
	case 0:
		child_process();
	}

#define TWO


#ifndef TWO


	// enable SIGUSR1 and wait for termination of child process

	unblock_signal(SIGUSR1);

	int res;
	int wstatus = 0;

	res = waitpid(child_pid, &wstatus, 0);

	if (res == -1 && errno == ECHILD) {
		printf("[parent  %lu] child process has terminated\n", get_timestamp());
		//break;
	} else if (res == -1 && (errno == EINTR || errno == EAGAIN)) {

		printf("[parent  %lu] waitpid has been interrupted\n", get_timestamp());

	} else if (res == 0) { // child process exists (at the moment of return from waitpid)

		printf("[parent  %lu] after waitpid - child process exists\n", get_timestamp());

	} else if (res == child_pid) { // child process exists (at the moment of return from waitpid)
		if (WIFEXITED(wstatus) || WIFSIGNALED(wstatus)) {
			printf("[parent  %lu] after waitpid - child process has terminated(2)\n", get_timestamp());
			//break;
		} else if (WIFSTOPPED(wstatus)) {
			printf("[parent  %lu] after waitpid - child process has been stopped\n", get_timestamp());
		} else if (WIFCONTINUED(wstatus)) {
			printf("[parent  %lu] after waitpid - child process has been resumed\n", get_timestamp());
		}
	}


#else

	int res;
	int wstatus = 0;
	int prev_delivered_signals = 0;

	// condition for exiting while loop:
	// child process terminates
	while (1) {

		printf("[parent  %lu] before waitpid\n", get_timestamp());

		res = waitpid(child_pid, &wstatus, WNOHANG); // WNOHANG:  return immediately if no child has exited.

		if (res == -1 && errno == ECHILD) {
			printf("[parent  %lu] child process has terminated\n", get_timestamp());
			break;
		} else if (res == -1 && (errno == EINTR || errno == EAGAIN)) {

			printf("[parent  %lu] waitpid has been interrupted\n", get_timestamp());

		} else if (res == 0) { // child process exists (at the moment of return from waitpid)

			printf("[parent  %lu] after waitpid - child process exists\n", get_timestamp());

		} else if (res == child_pid) { // child process exists (at the moment of return from waitpid)
			if (WIFEXITED(wstatus) || WIFSIGNALED(wstatus)) {
				printf("[parent  %lu] after waitpid - child process has terminated(2)\n", get_timestamp());
				break;
			} else if (WIFSTOPPED(wstatus)) {
				printf("[parent  %lu] after waitpid - child process has been stopped\n", get_timestamp());
			} else if (WIFCONTINUED(wstatus)) {
				printf("[parent  %lu] after waitpid - child process has been resumed\n", get_timestamp());
			}
		}

		printf("[parent  %lu] before sigsuspend\n", get_timestamp());

		// orig_mask does not contain SIGUSR1: thus SIGUSR1 can be delivered to parent process (and signal handler can be executed)
		// sigsuspend can return also because process has received SIGCLHD signal
		if (sigsuspend(&orig_mask) == -1 && errno != EINTR) {
			perror("sigsuspend");
			exit(1);
		}
		/*
	    sigsuspend() temporarily replaces the signal mask of the calling thread with the mask given by mask
	    and then suspends the thread until delivery of a signal whose action is to invoke a signal handler or to terminate a process.

        If the signal terminates the process, then sigsuspend() does not return.  If the signal is caught, then sigsuspend() returns after the signal handler returns, and the signal mask
        is restored to the state before the call to sigsuspend().
		*/



		if (delivered_signals != prev_delivered_signals) {
			printf("[parent  %lu] after sigsuspend - delivered_signals = %d\n", get_timestamp(), delivered_signals);

			prev_delivered_signals = delivered_signals;

			// now do something...
			//sleep(1);

		}

	}

#endif

	printf("[parent  %lu] terminating, have received a total of %d signals (SIGUSR1) from child process\n", get_timestamp(), delivered_signals);

	return 0;
}


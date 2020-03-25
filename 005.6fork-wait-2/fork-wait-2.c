#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> // nanosleep
#include <errno.h> // errno
#include <signal.h> // signal

int counter = 0;
pid_t my_pid;

void signal_handler(int signum) {

	counter++;

	printf("[%u] segnale ricevuto!  counter=%d\n", my_pid, counter);

}


int main(int argc, char *argv[]) {

	pid_t child_pid;
	int wstatus;
	int modal_result = -1;

	if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
		perror("signal");
		exit(EXIT_FAILURE);
	}


	child_pid = fork();

	if (child_pid == 0) {
		// sono il processo figlio (child process)

		my_pid = getpid();

		printf("[child] sono il processo figlio, il mio PID è %u\n\n", my_pid);

		// faccio qualcosa... dormo per qualche secondo

		struct timespec t;

		struct timespec remaining;

		t.tv_sec = 10;  // seconds
		t.tv_nsec = 0; // nanoseconds

		while (nanosleep(&t, &remaining) == -1) {

			if (errno == EINTR) {

				printf("[child] nanosleep interrotto!\n");

				printf("tempo rimanente: tv_sec=%lu seconds, tv_nsec=%lu nanoseconds\n", remaining.tv_sec, remaining.tv_nsec);

				t = remaining;

			} else if (errno == EFAULT) {
				perror("nanosleep");
				break;
			} else if (errno == EINVAL) {
				perror("nanosleep");
				break;
			}
		}

		// https://www.tldp.org/LDP/abs/html/exitcodes.html

		int return_value = 10;

		printf("[child] sto per terminare restituendo il valore %u\n", return_value);

		// processo figlio termina
		exit(return_value);

	} else if (child_pid > 0) {
		// sono il processo padre

		my_pid = getpid();

		printf("[parent] sono il processo padre, il mio PID è %u\n\n", my_pid);

		// mando un segnale al processo figlio

		for (int i = 0; i < 3; i++) {
			sleep(1);
			kill(child_pid, SIGUSR1);
			// kill(0 , SIGUSR1); // il segnale viene inviato ad ogni processo del "process group"
		}


		//pid_t ws = waitpid(child_pid, &wstatus, WUNTRACED | WCONTINUED);

		do {
        	pid_t ws = waitpid(child_pid, &wstatus, WUNTRACED | WCONTINUED);
            if (ws == -1) {
                perror("[parent] waitpid");
                exit(EXIT_FAILURE);
            }

            if (WIFEXITED(wstatus)) {

            	modal_result = WEXITSTATUS(wstatus);

                printf("[parent] child process è terminato, ha restituito: %d\n", modal_result);
            } else if (WIFSIGNALED(wstatus)) {
                printf("[parent] child process killed by signal %d\n", WTERMSIG(wstatus));
            } else if (WIFSTOPPED(wstatus)) {
                printf("[parent] child process stopped by signal %d\n", WSTOPSIG(wstatus));
            } else if (WIFCONTINUED(wstatus)) {
                printf("[parent] child process continued\n");
            }
        } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

	} else {
		perror("fork()");

		exit(1);
	}


}


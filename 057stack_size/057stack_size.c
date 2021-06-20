#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

// https://stackoverflow.com/questions/2279052/increase-stack-size-in-linux-with-setrlimit
// https://stackoverflow.com/a/2279084/974287

/*
 * set process stack size to 64 MB
 *
 */

int main (int argc, char **argv)
{
    const rlim_t kStackSize = 64 * 1024 * 1024LU;   // min stack size = 64 Mb
    struct rlimit rl;
    int result;
    rlim_t prev_stack_size;

/*
   man 2 setrlimit

   RLIMIT_STACK
    The maximum size of the process stack, in bytes. Upon reaching this limit, a SIGSEGV signal is generated.
    To handle this signal, a process must employ an alternate signal stack (sigaltstack(2)).

    Since Linux 2.6.23, this limit also determines the amount of space used for the process's command-line arguments and
    environment variables; for details, see execve(2).
 */

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0) {
    	prev_stack_size = rl.rlim_cur;

    	printf("current stack size: %lu\n", prev_stack_size);

        if (rl.rlim_cur < kStackSize) {
        	rl.rlim_cur = kStackSize;

            result = setrlimit(RLIMIT_STACK, &rl);
            if (result == -1) {
            	perror("setrlimit");
            	exit(1);
                //fprintf(stderr, "setrlimit returned result = %d\n", result);
            } else {
            	printf("new stack size = %lu, old stack size = %lu\n", rl.rlim_cur, prev_stack_size);
            }
        }
    }


    return 0;
}




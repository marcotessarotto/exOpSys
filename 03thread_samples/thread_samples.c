#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include <pthread.h>


#define handle_error_en(en, msg) \
	   do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
	   do { perror(msg); exit(EXIT_FAILURE); } while (0)


void mostra_threads_del_processo() {

	char pid_str[11];
	pid_t pid;

	snprintf(pid_str, 11, "%d", pid = getpid());



	char * newargv[] = { "ps", "-T", "-p", pid_str, NULL };

	char * newenviron[] = { NULL };

	switch (fork()) {

		case -1:
			perror("errore con fork()");
			break;

		case 0:
			// ps -T -p %d
			printf("[fork]output di execve(\"/bin/ps -T -p %d\")\n", pid);

			execve("/bin/ps", newargv, newenviron);

			perror("[fork] se siamo qui, qualcosa è andato storto\n");

			exit(EXIT_FAILURE);
			break;
		default:
			return;

	}


}


static void * thread_start(void *arg)
{
	char * msg = arg;

	pthread_t self = pthread_self();

	int sleep_period = 10;

	printf("[thread] thread id: %ld\n", self);

	printf("[thread] argument: %s\n", msg);

	printf("[thread] sleep per %d secondi\n", sleep_period);
	for (int i = 0; i < sleep_period; i++) {
		printf("[thread]%i\n", i);
		sleep(1);
	}

	printf("[thread] fine\n");

	return strdup("ok!\n"); // fine del thread
}

//#define USE_THREAD_ATTR

int main(int argc, char * argv[]) {

	int s;

	pthread_t thread_id;

	void * res;

	pid_t pid;

	printf("[maint]PID=%d\n", pid = getpid());

#ifdef USE_THREAD_ATTR

	pthread_attr_t attr;

    s = pthread_attr_init(&attr);
    if (s != 0)
    	handle_error_en(s, "[maint]pthread_attr_init");

    printf("[maint]prima di pthread_create\n");
    s = pthread_create(&thread_id, &attr,
                       &thread_start, "hello!");
    if (s != 0)
        handle_error_en(s, "[maint]pthread_create");

    // a questo punto, il thread thread_id è stato avviato
    printf("[maint]dopo pthread_create\n");

    /* Destroy the thread attributes object, since it is no
       longer needed */

    s = pthread_attr_destroy(&attr);
    if (s != 0)
        handle_error_en(s, "[maint]pthread_attr_destroy");

    printf("[maint]prima di pthread_join\n");
    // aspettiamo la conclusione di thread_id
    s = pthread_join(thread_id, &res);
    if (s != 0)
        handle_error_en(s, "[maint]pthread_join");

    printf("[maint]Joined with thread %ld, return value: %s\n",
    		thread_id, (char *) res);
    free(res);      /* Free memory allocated by thread */

#else


    printf("[maint]prima di pthread_create; dimensione di default della stack del nuovo thread: 2MB (x86_64)\n");
    s = pthread_create(&thread_id, NULL,
                       &thread_start, "hello!");
    if (s != 0)
        handle_error_en(s, "[maint]pthread_create");
    // a questo punto, il thread thread_id è stato avviato
    printf("[maint]dopo pthread_create\n");

    // dovrebbero comparire due linee, del tipo:
    /*
  PID  SPID TTY          TIME CMD
 5654  5654 ?        00:00:00 03thread_sample
 5654  5659 ?        00:00:00 03thread_sample
     */

    mostra_threads_del_processo();

    printf("[maint]premi enter per continuare:\n");
    getchar();

    mostra_threads_del_processo();

    printf("[maint]prima di pthread_join\n");
    // aspettiamo la conclusione di thread_id
    s = pthread_join(thread_id, &res);
    if (s != 0)
        handle_error_en(s, "[maint]pthread_join");

    printf("[maint]Joined with thread %ld, return value from thread: %s\n",
    		thread_id, (char *) res);
    free(res);      /* Free memory allocated by thread */

#endif

    printf("[maint]bye!\n");
    sleep(1);

    exit(EXIT_SUCCESS);
}

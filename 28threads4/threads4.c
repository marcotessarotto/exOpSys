#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

void get_current_time(char * buffer)
{
    time_t timer;
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S\n", tm_info);
}


/*
 * processo principale composto da 4 threads.

definire un signal handler: ogni volta che riceve SIGINT, incrementa una variabile globale 'counter1'. il valore iniziale di counter1 è 0.

il thread principale legge da stdin un carattere alla volta fino a che non trova il carattere 'q', in tal caso scrive il valore di counter2 (vedi thread2) su stdout ed invoca exit.

thread 1:
assegna un numero random ad una variabile globale 'globvar'; sveglia i thread eventualmente in attesa; dorme un secondo; cicla.

thread2, quando è svegliato dal thread 1, legge globvar e lo scrive nella variabile locale 'loc';
se counter1 è pari, produce una stringa di caratteri di lunghezza pari a loc & 0x1F contenenti la stringa 'abc' ripetuta fino a riempire la stringa.
Se counter1 è dispari, produce una stringa contenente il tempo corrente (vedere esempio 23printtime).
Scrive la stringa su una pipe, all'altro estremo della pipe è in ascolto un processo 'zip -q /tmp/thread2.zip -' (vedere esempio 22two_procs)
thread2 incrementa il contatore counter2 (valore iniziale zero) con la lunghezza della stringa prodotta.
Infine thread2 torna a dormire, aspettando di essere svegliato.


thread3, quando è svegliato dal thread 1, prende le ultime due stringhe prodotte da thread2,
le concatena e le scrive tramite pipe ad un processo 'sort > /tmp/thread3.txt
 */

int counter1 = 0;
pthread_mutex_t counter1_mutex = PTHREAD_MUTEX_INITIALIZER;

int counter2 = 0;
pthread_mutex_t counter2_mutex = PTHREAD_MUTEX_INITIALIZER;

int globvar = 0;
pthread_mutex_t globvar_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t globvar_cond = PTHREAD_COND_INITIALIZER;

int pipefd_compressor[2];
int pipefd_sort[2];

char ultima_stringa_thread2[33];
char penultima_stringa_thread2[33];
pthread_mutex_t thread2_stringhe_mutex = PTHREAD_MUTEX_INITIALIZER;


// definire un signal handler: ogni volta che riceve SIGINT, incrementa una variabile globale 'counter1'. il valore iniziale di counter1 è 0.
void signal_handler(int sig) {
	if (sig == SIGINT) {

		pthread_mutex_lock(&counter1_mutex);
		counter1++;
		pthread_mutex_unlock(&counter1_mutex);

	}
}


void * thread_1(void * arg) {

	printf("[thread_1]start\n");

	// thread 1: assegna un numero random ad una variabile globale 'globvar'; sveglia i thread eventualmente in attesa; dorme un secondo; cicla.

	int loc;

	while (1) {

		pthread_mutex_lock(&globvar_mutex);
		globvar = random();

		loc = globvar;
		pthread_mutex_unlock(&globvar_mutex);

		// svegliamo tutti i thread in attesa su globvar_cond
		pthread_cond_broadcast(&globvar_cond);

		printf("[thread_1] globvar = %d\n", loc);

		sleep(1);
	}

	printf("[thread_1]fine thread\n");

	return NULL;
}

const char nl[] = "\n";

void * thread_2(void * arg) {

	printf("[thread_2]start\n");

/*
thread2, quando è svegliato dal thread principale, legge globvar e lo scrive nella variabile locale 'loc';
se counter1 è pari, produce una stringa di caratteri di lunghezza pari a loc & 0x1F contenenti la stringa 'abc' ripetuta fino a riempire la stringa.
Se counter1 è dispari, produce una stringa contenente il tempo corrente (vedere esempio 23printtime).
Scrive la stringa su una pipe, all'altro estremo della pipe è in ascolto un processo 'zip -q /tmp/thread2.zip -' (vedere esempio 22two_procs)
thread2 incrementa il contatore counter2 (valore iniziale zero) con la lunghezza della stringa prodotta.
Infine thread2 torna a dormire, aspettando di essere svegliato. (anche thread2 cicla)

 */
	int loc;
	const char * ptr = "abc";
	char buffer[32];
	int loc_counter1;

	pthread_mutex_lock(&globvar_mutex);
	loc = globvar;
	pthread_mutex_unlock(&globvar_mutex);

	while (1) {

		pthread_mutex_lock(&globvar_mutex);

		while (globvar == loc) {
			pthread_cond_wait(&globvar_cond, &globvar_mutex); // la chiamata a pthread_cond_wait richiede di avere già il lock sul mutex
		}

		loc = globvar;

		pthread_mutex_unlock(&globvar_mutex);

		printf("[thread_2]dopo pthread_cond_wait, globvar=%d\n", loc);


		pthread_mutex_lock(&counter1_mutex);
		loc_counter1 = counter1;
		pthread_mutex_unlock(&counter1_mutex);

		if (loc_counter1 % 2 == 0) {
			int size = loc & 0x1F; // 0x1F è 31 in base 10

			for (int i = 0; i < (size-1); i++) {
				buffer[i] = ptr[i % 3];
			}

			buffer[size-1] = '\0';
		} else {
			get_current_time(buffer);
		}

		int len = strlen(buffer);

		if (len == 0) {
			printf("[thread_2]len == 0, skip\n");
			continue;
		}

		printf("[thread_2]write to pipe: len=%d, data=%s\n", len, buffer);

		if (write(pipefd_compressor[1], buffer, len) == -1) {
			perror("write");
			break;
		}

		write(pipefd_compressor[1], nl, 1); // scriviamo newline nella pipe

		pthread_mutex_lock(&counter2_mutex);
		counter2 += len;
		pthread_mutex_unlock(&counter2_mutex);


		pthread_mutex_lock(&thread2_stringhe_mutex);

		strcpy(penultima_stringa_thread2, ultima_stringa_thread2);
		strcpy(ultima_stringa_thread2, buffer);

		pthread_mutex_unlock(&thread2_stringhe_mutex);

	}

	printf("[thread_2]fine thread\n");

	return NULL;
}


void * thread_3(void * arg) {
	printf("[thread_3]start\n");

/*
thread3, quando è svegliato dal thread principale, prende le ultime due stringhe prodotte da thread2,
le concatena e le scrive tramite pipe ad un processo 'sort > /tmp/thread3.txt
 */
	int loc;
	char buffer[32*2];

	pthread_mutex_lock(&globvar_mutex);
	loc = globvar;
	pthread_mutex_unlock(&globvar_mutex);


	while (1) {

		pthread_mutex_lock(&globvar_mutex);

		while (globvar == loc) {
			pthread_cond_wait(&globvar_cond, &globvar_mutex); // la chiamata a pthread_cond_wait richiede di avere già il lock sul mutex
		}

		loc = globvar;

		pthread_mutex_unlock(&globvar_mutex);

		printf("[thread_3]dopo pthread_cond_wait, globvar=%d\n", loc);


		pthread_mutex_lock(&thread2_stringhe_mutex);

		strcpy(buffer, ultima_stringa_thread2);
		strcat(buffer, penultima_stringa_thread2);

		pthread_mutex_unlock(&thread2_stringhe_mutex);

		int len = strlen(buffer);

		if (len == 0) {
			printf("[thread_3]len == 0, skip\n");
			continue;
		}

		printf("[thread_3]write to pipe: len=%d, data=%s\n", len, buffer);

		int s;
		if ((s = write(pipefd_sort[1], buffer, len)) == -1) {
			perror("write");
			break;
		} else
			printf("[thread_3]bytes written to pipe: %d\n", s);

		write(pipefd_sort[1], nl, 1); // scriviamo newline nella pipe

	}

	printf("[thread_3]fine thread\n");

	return NULL;
}


void compressor(void) {

	printf("[compressor]start pid=%d\n", getpid());

	// cambia il process group; lo facciamo per evitare di ricevere SIGINT quando digitiamo Ctrl-C nel terminale
	setpgrp();


	if (close(pipefd_compressor[1]) == -1) { // non ci serve il lato pipe in scrittura
		perror("close3");
	}

	// chiudiamo stdin e sostituiamo con pipefd[0]
	if (dup2(pipefd_compressor[0], STDIN_FILENO) == -1) { //
		perror("dup2");
	}

	char * newargv[] = { "zip", "-q", "/tmp/thread2.zip", "-", NULL };

	char * newenviron[] = { NULL };

	printf("[child]prima di execve /usr/bin/zip (con pipefd[0] al posto di stdin)\n");

	// ora lanciamo zip, che al posto di stdin si troverà l'estremità di lettura della pipe (ma per zip non cambia nulla, pensa di leggere da stdin)
	execve("/usr/bin/zip", newargv, newenviron);

	perror("[child]se siamo qui, qualcosa è andato storto\n");

	exit(EXIT_FAILURE);
}

void sorter(void) {
	printf("[sorter]start pid=%d\n", getpid());

	// cambia il process group; lo facciamo per evitare di ricevere SIGINT quando digitiamo Ctrl-C nel terminale
	setpgrp();

	if (close(pipefd_sort[1]) == -1) { // non ci serve il lato pipe in scrittura
		perror("close3");
	}


	printf("[child]prima di execve /usr/bin/sort (con pipefd[0] al posto di stdin)\n");

	// chiudiamo stdin e sostituiamo con pipefd[0]
	if (dup2(pipefd_sort[0], STDIN_FILENO) == -1) { //
		perror("dup2");
	}

	int fd;
	if ((fd = open("/tmp/thread3.txt", O_WRONLY
			 | O_CREAT | O_TRUNC, 0600 )) == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	if (dup2(fd, STDOUT_FILENO) == -1) { //
		perror("dup2");
	}

	close(fd);

	// sort > /tmp/thread3.txt
	char * newargv[] = { "sort" ,  NULL };

	char * newenviron[] = { NULL };


	// ora lanciamo zip, che al posto di stdin si troverà l'estremità di lettura della pipe (ma per zip non cambia nulla, pensa di leggere da stdin)
	execve("/usr/bin/sort", newargv, newenviron);

	perror("[child]se siamo qui, qualcosa è andato storto\n");

	exit(EXIT_FAILURE);



}

int main(int argc, char * argv[]) {

	if (signal(SIGINT, signal_handler) == SIG_ERR) {
		perror("signal");
	}

	// useremo una pipe per comunicare col processo figlio
	if (pipe(pipefd_compressor) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	if (pipe(pipefd_sort) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	if (fork() == 0) {
		close(pipefd_sort[0]);
		close(pipefd_sort[1]);

		compressor();
	}

	if (fork() == 0) {
		close(pipefd_compressor[0]);
		close(pipefd_compressor[1]);

		sorter();
	}

	close(pipefd_compressor[0]); // chiudo il lato read delle pipe
	close(pipefd_sort[0]);

	pthread_t t1, t2, t3;


	if (pthread_create(&t1, NULL, thread_1, NULL) != 0) {
		perror("pthread_create");

		exit(EXIT_FAILURE);
	}

	if (pthread_create(&t2, NULL, thread_2, NULL) != 0) {
		perror("pthread_create");

		exit(EXIT_FAILURE);
	}

	if (pthread_create(&t3, NULL, thread_3, NULL) != 0) {
		perror("pthread_create");

		exit(EXIT_FAILURE);
	}


	printf("[thread_principale]\n");

// il thread principale legge da stdin un carattere alla volta fino a che non trova il carattere 'q', in tal caso scrive il valore di counter2 (vedi thread2) su stdout ed invoca exit.
	while (getchar() != 'q')
		;

	int loc;

	pthread_mutex_lock(&counter1_mutex);
	loc = counter2;
	pthread_mutex_unlock(&counter1_mutex);

	printf("counter2 = %d\n", loc);

	// la chiusura delle pipe comporta un errore in write nei thread 2 e 3
	close(pipefd_compressor[1]);
	close(pipefd_sort[1]);

	printf("prima di join t2\n");
	if (pthread_join(t2, NULL) != 0) {
		perror("pthread_join");
	} else
		printf("join t2 ok\n");

	printf("prima di join t3\n");
	if (pthread_join(t3, NULL) != 0) {
		perror("pthread_join");
	} else
		printf("join t3 ok\n");

	if (pthread_cancel(t1) != 0) {
		perror("pthread_cancel");
	}

	printf("prima di join t1\n");
	if (pthread_join(t1, NULL) != 0) {
			perror("pthread_join");
	} else
		printf("join t1 ok\n");


	//sleep(1);

	printf("bye!\n");

	return 0;
}

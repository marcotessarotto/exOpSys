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

/*
 * schema di utilizzo di processo lavoratore (worker) che riceve un parametro e produce un risultato restituito al processo chiamante
 *
 */

struct worker_params {
	char query[128];
	int p1;
	int p2;
	int p3;
};
typedef struct worker_params worker_params_t;

struct worker_results {
	int res1;
	int res2;
	int data_len; // contiene la lunghezza dei dati nell'array data
	char data[128];
};
typedef struct worker_results worker_results_t;

// non utilizzato in questo esempio
// nel caso di thread, il meccanismo è più semplice
void * worker_thread(void * param) {

	worker_params_t * wp = param;

    // qui il worker lavora (i.e. una query, ...)
	// alla fine restituisce i risultati

	worker_results_t * result = malloc(sizeof(worker_results_t));

	// elaborazione

	result->res1 = 42;
	strcpy(result->data,"questo è il risultato");

	return result; // oppure: pthread_exit(result)
}


/**
 * deserializza il risultato contenuto in mm e lo scrive in res
 */
void leggi_risultato_da_mm(char * mm, worker_results_t * res) {
	if (mm == NULL)
		return;

	int * i = (int *) mm;

	res->res1 = i[0];
	res->res2 = i[1];
	res->data_len = i[2];

	mm += sizeof(int) * 3;

	memcpy(res->data, mm, res->data_len);
}

/**
 * serializza res e lo scrive in mm
 */
void copia_risultato_in_mm(worker_results_t * res, char * mm) {

	int * i = (int *) mm;

	i[0] = res->res1;
	i[1] = res->res2;
	i[2] = res->data_len;

	mm += sizeof(int) * 3;

	memcpy(mm, res->data, res->data_len);

}


void worker_process(worker_params_t * parametri_per_processo_worker, char * spazio_risultato) {

	printf("[worker process]pid=%d, query=%s\n", getpid(), parametri_per_processo_worker->query);

	printf("[worker process]elaborazione in corso...\n");
	sleep(2);

	worker_results_t result;

	result.res1 = 100;
	result.res2 = 200;

	strcpy(result.data,"questo è il risultato dell'elaborazione.");
	result.data_len = strlen(result.data) + 1;

	copia_risultato_in_mm(&result, spazio_risultato);

	printf("[worker process]risultato pronto. fine del processo!\n");

	exit(EXIT_SUCCESS); // fine processo worker
}

#define SPAZIO_RISULTATO_LEN 1024

int main(int argc, char * argv[]) {

	int pid;
	int status;

	// il parametro del processo worker
	worker_params_t parametri_per_processo_worker;

	strcpy(parametri_per_processo_worker.query, "select * from ... (una query che passiamo al processo worker)");
	parametri_per_processo_worker.p1 = 42;

	// prima di fork, prepariamo una memory map che servirà a restituire il risultato dell'elaborazione al processo chiamante

	char * memoria_condivisa = malloc(SPAZIO_RISULTATO_LEN * sizeof(char));

	memoria_condivisa = mmap(NULL, SPAZIO_RISULTATO_LEN, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS , -1, 0);

	if (memoria_condivisa == MAP_FAILED) {
		perror("mmap");

		exit(EXIT_FAILURE);
	}


	switch (pid = fork()) {
	case 0:
		worker_process(&parametri_per_processo_worker, memoria_condivisa);

		break;
	case -1:
		perror("fork");

		exit(EXIT_FAILURE);
		break;
	default:
		;
	}

	pid = waitpid(-1, &status, 0);
	if (pid == -1) {
		perror("waitpid");
		exit(EXIT_FAILURE);
	}

	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
		printf("il processo worker ha restituito errore!\n");
		exit(EXIT_FAILURE);
	}

	// leggiamo il risultato:

	worker_results_t result;

	leggi_risultato_da_mm(memoria_condivisa, &result);

	printf("[main process]risultato ricevuto da worker: res1=%d res2=%d datalen=%d data=%s\n", result.res1, result.res2, result.data_len, result.data);

	printf("bye!\n");

	return 0;
}

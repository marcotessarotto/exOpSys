#define _GNU_SOURCE
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
#include <pthread.h>


#include <syslog.h>

#include <sys/socket.h>
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h> // inet_pton

// 127.0.0.1 è l'indirizzo
#define BIND_ADDRESS "127.0.0.1"
#define LISTEN_BACKLOG 10
#define PORT 20000

int serversocket_fd;

#define THREADS_START_SIZE 10
int threads_capacity; // capacità dell'array
int threads_pos = 0; // dove scrivere il prossimo elemento nell'array

struct _thread_data {
	pthread_t t;
	int sfd;
};

typedef struct _thread_data thread_data;

thread_data * threads;
pthread_mutex_t threads_mutex = PTHREAD_MUTEX_INITIALIZER;

#define NUMBER_OF_CLIENTS 3
#define BUF_SIZE 1024

void init_thread_array() {
	if (threads != NULL) {
		printf("threads già inizializzato\n");
		exit(EXIT_FAILURE);
	}

	threads = calloc(THREADS_START_SIZE, sizeof(thread_data));

	threads_capacity = THREADS_START_SIZE;
}

void add_thread_to_array(pthread_t t, int sfd) {

	pthread_mutex_lock(&threads_mutex);

	if (threads_pos == threads_capacity) {

		threads_capacity += 10;

		threads = realloc(threads, sizeof(thread_data) * threads_capacity);
	}

	threads[threads_pos].t = t;
	threads[threads_pos].sfd = sfd;
	threads_pos++;

	pthread_mutex_unlock(&threads_mutex);
}

void remove_thread_from_array(pthread_t t) {
	pthread_mutex_lock(&threads_mutex);

	for(int i = 0; i < threads_pos; i++ )
			if (pthread_equal(threads[i].t, t) != 0) {
				threads[i].t = 0;
				threads[i].sfd = -1;
				return;
			}

	pthread_mutex_unlock(&threads_mutex);
}

void send_msg_to_all_clients(char * buffer, int len) {

	pthread_t t = pthread_self();

	pthread_mutex_lock(&threads_mutex);

	for(int i = 0; i < threads_pos; i++ ) {
		printf("%d send_msg_to_all_clients threads[i].sfd=%d threads[i].t=%ld\n",i, threads[i].sfd, threads[i].t);

		if (threads[i].sfd != -1 && pthread_equal(threads[i].t, t) == 0) {
			if (write(threads[i].sfd, buffer, len) < 0) {
				perror("write to client socket");
			}
		}
	}

	pthread_mutex_unlock(&threads_mutex);
}

void * server_thread(void * arg) {

	int clientfd = *((int *) arg);

	free(arg);

	pthread_t self = pthread_self();

	// leggiamo i dati dal socket
	// ciò che riceviamo dal client, lo scriviamo su un file

	// altro esempio: manda il messaggio ricevuto agli altri client

	char buffer[BUF_SIZE];
	int len;

	while ((len = read(clientfd, buffer, BUF_SIZE)) > 0) {

		// mandiamo il messaggio ricevuto a tutti gli altri client connessi (tranne al socket di questo thread)

		printf("server_thread %ld, received data from socket %d, data len = %d, data = %s\n", self, clientfd, len, buffer);

		send_msg_to_all_clients(buffer, len);

	}

	if (len == 0) { // EOF

	} else if (len < 0) {
		perror("server_thread, read error");

	}

	close(clientfd);

	remove_thread_from_array(pthread_self());

	printf("server_thread %ld termina\n", self);

	return NULL;
}


void * server_main_thread(void * arg) {

	// vedere schema a pag. 1156
	// vedere figura 56-3 a pag. 1159

	struct sockaddr_in address; // structure describing an Internet socket address

	printf("lo è una interfaccia di rete visibile solo localmente\n"
			"il comando 'ip address show dev lo' restituisce:\n");
	system("ip address show dev lo");

	// printf("premi enter per continuare\n");
	//getchar();


	printf("\n"
			"socket DOMAIN:\n"
			"AF_UNIX: unix sockets, gestite dal kernel\n"
			"AF_INET: IPv4\n"
			"AF_INET6: IPv6\n");

	printf("\n"
			"socket TYPE :\n"
			"SOCK_STREAM: per AF_INET -> Transmission Control Protocol (TCP)\n"
			"SOCK_DGRAM: per AF_INET -> User Datagram Procotol (UDP)\n");

	printf("\n"
			"creo un socket nel dominio AF_INET (IPv4) che userà il protocollo TCP\n");
	if ((serversocket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	printf("\naddress è di tipo struct sockaddr_in: descrive l'indirizzo di un socket internet\n");
	address.sin_family = AF_INET; // IPv4
	address.sin_port = htons(PORT);

	// inet_pton converte un indirizzo internet da formato testuale in formato binario
	inet_pton(AF_INET, BIND_ADDRESS, &address.sin_addr);

	printf("\n"
			"collego il socket all'indirizzo di rete %s ed alla porta TCP %d\n", BIND_ADDRESS, PORT);
	printf("in questo modo, il server è raggiungibile dai client (attraverso indirizzo e porta)\n");
	// notare che bind è definita con parametro struct sockaddr address, ma qui utilizziamo il tipo struct sockaddr_in
	if (bind(serversocket_fd, (struct sockaddr * ) &address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	printf("\n"
			"il socket (passivo) accetta connessioni da altri socket (attivi)\n"
			"socket passivo o 'listening socket'\n");
	if (listen(serversocket_fd, LISTEN_BACKLOG) == -1) { // backlog è il numero massimo di connessioni attive in coda (pending connections)
		perror("listen");
		exit(EXIT_FAILURE);
	}

	printf("\nil comando 'netstat -ant | grep 60000' restituisce:\n");

	system("netstat -ant | grep 60000"); //

	// (si blocca fino a che non viene aperta una connessione in ingresso)

	/* Now we can accept incoming connections one
	 at a time using accept(2) */
	struct sockaddr_in peer_addr;
	unsigned int peer_addr_size = sizeof(struct sockaddr_in);
	int clientfd;

	while (1) {

		printf("sto per chiamare accept (si blocca se non ci sono connessioni attive in coda)\n");
	    clientfd = accept(serversocket_fd, (struct sockaddr * ) &peer_addr, &peer_addr_size);
	    // accept crea un nuovo socket, che è connesso al socket attivo entrante
	    if (clientfd == -1) {
		    perror("accept");
		    exit(EXIT_FAILURE);
	    }

	    // se siamo qui, accept ha restituito, prendendola dalla coda, una connessione attiva che deve essere servita

	    // creiamo un nuovo thread (che risponderà al nuovo socket restituito da accept

	    pthread_t t;
	    int * arg = malloc(sizeof(int));
	    *arg = clientfd;

	    if (pthread_create(&t, NULL, server_thread, (void *) arg) != 0) {
	    	printf("pthread_create ha restituito errore");
	    } else {
	    	add_thread_to_array(t, clientfd);
	    }

	}

	return NULL;
}

void * client_thread(void * arg) {
	int clientfd = *((int *) arg);

	free(arg);

	pthread_t self = pthread_self();

	// leggiamo i dati dal socket
	// ciò che riceviamo dal client, lo scriviamo su un file

	// altro esempio: manda il messaggio ricevuto agli altri client

	char buffer[BUF_SIZE];
	int len;

	while ((len = read(clientfd, buffer, BUF_SIZE)) > 0) {

		// mandiamo il messaggio ricevuto a tutti gli altri client connessi (tranne al socket di questo thread)

		printf("client_thread %ld, received data from socket %d, data len = %d, data = %s\n", self, clientfd, len, buffer);


	}

	if (len == 0) { // EOF

	} else if (len < 0) {
		perror("server_thread, read error");

	}

	close(clientfd);

	remove_thread_from_array(pthread_self());

	printf("server_thread %ld termina\n", self);

	return NULL;
}

void connect_to_server(int id) {

	int sfd;
	struct sockaddr_in address; // structure describing an Internet socket address

	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("non posso aprire socket");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET; // IPv4
	address.sin_port = htons(PORT);

	// inet_pton converte un indirizzo internet da formato testuale in formato binario
	inet_pton(AF_INET, BIND_ADDRESS, &address.sin_addr);

	if (connect(sfd, (struct sockaddr * ) &address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	pthread_t t;
    int * arg = malloc(sizeof(int));
    *arg = sfd;

    if (pthread_create(&t, NULL, client_thread, (void *) arg) != 0) {
    	printf("pthread_create ha restituito errore");
    }

	sleep(1);

	char buffer[1024];

	if (id == 0)
		for (int i = 0; i < 10; i++) {

			sprintf(buffer, "pid=%d, msg %d\n", getpid(), i);

			printf("client pid=%d, sto per mandare il messaggio %i\n", getpid(), i);

			write(sfd, buffer, strlen(buffer) + 1);

			sleep(1);

		}
	else
		sleep(10);


	printf("client pid=%d termina\n", getpid());

	exit(EXIT_SUCCESS);
}


int main(int argc, char * argv[]) {

	printf("***LAVORI IN CORSO***\n");
	return 0;

	pthread_t t;

	init_thread_array();

	if (pthread_create(&t, NULL, server_main_thread, NULL) != 0) {
		printf("non posso creare server_main_thread\n");
		exit(EXIT_FAILURE);
	}

	sleep(1);

	printf("starting clients\n");


	for (int i = 0; i < NUMBER_OF_CLIENTS; i++) {

		if (fork() == 0) {
			connect_to_server(i);
		}

	}

	printf("digita 'q' per chiudere il server\n");

	while (getchar() != 'q')
		;

	if (close(serversocket_fd) != 0) {
		perror("close(serversocket_fd)");
	}

	while (wait(NULL) != -1)
		;

	printf("server chiuso, bye\n");

	return 0;
}

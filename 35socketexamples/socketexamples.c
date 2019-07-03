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
#include <signal.h>
#include <time.h>
#include <syslog.h>
#include <sys/socket.h>
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h> // inet_pton
#include <netdb.h>

#define SERVER_ADDR "localhost"
#define PORT_NUM_STR "40001"
#define PORT_NUM 40001
#define BUF_MAX_SIZE 1024
#define BACKLOG 10

const char * decode_ai_family(int ai_family) {
	switch (ai_family) {
	case AF_INET:
		return "AF_INET";
	case AF_INET6:
		return "AF_INET6";
	default:
		return "?";
	}
}

const char * decode_ai_socktype(int ai_socktype) {
	switch (ai_socktype) {
	case SOCK_STREAM:
		return "SOCK_STREAM";
	case SOCK_DGRAM:
		return "SOCK_DGRAM";
	default:
		return "?";
	}
}

const char * decode_ai_protocol(int ai_protocol) {
	switch (ai_protocol) {
	case 6:
		return "TCP";
	case 17:
		return "UDP";
	default:
		return "?";
	}
}

void connetti_al_server() {

	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int client_socket;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC;                /* Allows IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    if (getaddrinfo(SERVER_ADDR, PORT_NUM_STR, &hints, &result) != 0) {
    	perror("getaddrinfo");
    	exit(EXIT_FAILURE);
    }

    int i = 0;

    for (rp = result; rp != NULL; rp = rp->ai_next) {

    	printf("getaddrinfo #%d: ai_family=%s ai_socktype=%s ai_protocol=%s\n", i, decode_ai_family(rp->ai_family), decode_ai_socktype(rp->ai_socktype), decode_ai_protocol(rp->ai_protocol));

        client_socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (client_socket == -1) {
        	printf("apertura socket fallito #%d\n", i);
        	perror("socket");
            continue;                           /* On error, try next address */
        }

        if (connect(client_socket, rp->ai_addr, rp->ai_addrlen) != -1) {
        	printf("apertura socket riuscita #%d\n", i);

            break;                              /* Success */
        }

        /* Connect failed: close this socket and try next address */
        //perror("connect");

        close(client_socket);

        i++;
    }

    char * msg_al_server = "hello!";

    write(client_socket, msg_al_server, strlen(msg_al_server) + 1);

    char buffer[BUF_MAX_SIZE];

    if (read(client_socket, buffer, BUF_MAX_SIZE) > 0) {
    	printf("dati ricevuti dal server: %s\n", buffer);
    }

    close(client_socket);



}


void simple_server() {
    // descrittori dei socket
    int server_socket, incoming_socket;

    // strutture di indirizzi per socket
    struct sockaddr_in serv_addr, client_addr;

    char buff[BUF_MAX_SIZE];

    // creiamo il socket, dominio IPv4, TCP
    server_socket = socket(AF_INET, SOCK_STREAM, 0); // TCP socket

    if(server_socket == -1) {
      perror("socket");
      exit(EXIT_FAILURE);
    } else {
    	printf("server - socket ok\n");
    }

    // inizializzo l'indirizzo del server a zero
    bzero((char *)&serv_addr, sizeof(serv_addr));

    // famiglia di indirizzi
    serv_addr.sin_family = AF_INET;

    // il server può ricevere connessioni su qualsiasi indirizzo disponibile della macchina
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // numero a 16 bit, porta di ascolto del server
    // htons (host to network short) interpreta correttamente l'endianess
    serv_addr.sin_port = htons(PORT_NUM);

    // attachiamo il server socket ad un indirizzo ed una porta
    if (bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    	perror("bind");
    	exit(EXIT_FAILURE);
    } else {
    	printf("server - bind ok\n");
    }

    // il socket diventa passivo, specifico la dimensione della coda delle connessioni entranti in attesa di essere "accettate"
    if (listen(server_socket, BACKLOG) == -1) {
    	perror("listen");
    	exit(EXIT_FAILURE);
    } else {
    	printf("server - listen ok\n");
    }

    printf("server - before accept\n");
    socklen_t size = sizeof(client_addr);

    int client_number = 0;

    // aspetta fino a quando un client stabilisce una connessione con il server
    // viene restituito un nuovo socket
    while ((incoming_socket = accept(server_socket, (struct sockaddr *)&client_addr, &size)) != -1) {

    	printf("nuova connessione in ingresso\n");

    	int len;
        if ( (len = read(incoming_socket, buff, sizeof(buff))) > 0)
        	printf("ricevuti dati dal client, len=%d, %s\n", len, buff);
        else if (len == -1)
        	perror("read");


        // scriviamo qualcosa al client
        sprintf(buff, "ciao! sei il client numero %d\n", client_number++);

        if (write(incoming_socket, buff, strlen(buff) + 1) == -1) {
        	perror("write");
        }

        // chiudiamo il socket
        if (shutdown(incoming_socket, SHUT_RDWR) == -1) {
        	perror("shutdown");
        }

        if (client_number > 0) break;

    }

    if (incoming_socket == -1) {
    	perror("accept");
    }

    // chiudo il server socket
    if (close(server_socket) == -1) {
    	perror("server - close server_socket");
    } else {
    	printf("close server_socket ok\n");
    }
}


int main(int argc, char * argv[]) {

	if (fork() == 0) {
		simple_server();
		exit(EXIT_SUCCESS);
	}



	connetti_al_server();



	return 0;

}

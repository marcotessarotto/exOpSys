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


#define SRV_NAME "nanows"

int serversocket_fd;

void log_msg(char * msg) {
  int level, options;

  options = 0;
  level = LOG_INFO;

  openlog(SRV_NAME, options, LOG_USER);
  syslog(LOG_USER | level, "%s", msg);
  closelog();
}

void log_msg2(char * msg, int val) {
  int level, options;

  options = 0;
  level = LOG_INFO;

  openlog(SRV_NAME, options, LOG_USER);
  syslog(LOG_USER | level, "%s %d", msg, val);
  closelog();
}

static void signal_handler(int sig) {

  int wstatus;

  //printf("ricevuto segnale %d\n", sig);

  if (sig == SIGCHLD) {

    log_msg("ricevuto segnale SIGCHLD");

    pid_t pid = wait( & wstatus);

    log_msg2("il processo è terminato", pid);

  }

  // exit(EXIT_SUCCESS);

}

#define BUFFER_SIZE 1024

char buffer[BUFFER_SIZE];

#define BLAH "blah blah\n\n"

const char * header_str = "HTTP/1.1 200 OK\n"
//"Date: Mon, 27 Jul 2009 12:28:53 GMT\n"
"Server: nanows\n"
//"Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT"
"Content-Length: %d\n"
"Content-Type: text/html\n"
"Connection: Closed\n\n";

const char * html_content = "<html>"
"<body>"
"<h1>Hello, World!</h1>"
"</body>"
"</html>";

void process_client_connection(int clientfd) {
  printf("worker process PID=%d\n", getpid());

  int valread = read(clientfd, buffer, BUFFER_SIZE);

  printf("%s\n", buffer);

  char output_buffer[BUFFER_SIZE];
  output_buffer[0] = 0;

  sprintf(output_buffer, header_str, strlen(html_content));
  strcat(output_buffer, html_content);

  if (write(clientfd, output_buffer, strlen(output_buffer)) == -1) {
    perror("write");
  }

  //    if (send(clientfd , output_buffer , strlen(output_buffer) , 0 ) == -1) {
  //     perror("send");
  //    }

  if (close(clientfd) == -1) {
    perror("close");
  }
}

#define BIND_ADDRESS "127.0.0.1"
#define LISTEN_BACKLOG 10
#define PORT 8080

void start_server() {

  //struct sockaddr addr = { 0, {0}};
  struct sockaddr_in address;

  if ((serversocket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // addr.sa_family = AF_INET;
  // strncpy(addr.sa_data, BIND_ADDRESS, sizeof(addr.sa_data)-1);
  //
  // if (bind(socketfd, &addr, sizeof(struct sockaddr)) == -1) {
  // perror("bind");
  // exit(EXIT_FAILURE);
  // }

  address.sin_family = AF_INET;
  //address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  inet_pton(AF_INET, BIND_ADDRESS, &address.sin_addr);

  if (bind(serversocket_fd, (struct sockaddr * ) &address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(serversocket_fd, LISTEN_BACKLOG) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  /* Now we can accept incoming connections one
     at a time using accept(2) */
  struct sockaddr_in peer_addr;
  unsigned int peer_addr_size = sizeof(struct sockaddr_in);
  int clientfd;

  while (1) {
    clientfd = accept(serversocket_fd, (struct sockaddr * ) &peer_addr, &peer_addr_size);
    if (clientfd == -1) {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    if (fork() == 0) {

      struct timespec start, stop;

      /* Code to deal with incoming connection(s)... */
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID, & start);

      process_client_connection(clientfd);

      clock_gettime(CLOCK_PROCESS_CPUTIME_ID, & stop);

      double result = (stop.tv_sec - start.tv_sec) * 1e6 + (stop.tv_nsec - start.tv_nsec) / 1e3; // in microseconds

      printf("[worker]tempo di CPU consumato (microsecondi): %lf \n", result);

      exit(EXIT_SUCCESS);
    }

  }

}

void close_server() {

  if (close(serversocket_fd) == -1) {
    perror("close");
  }

}

void * control_thread(void * arg) {

  printf("premi enter per terminare\n");

  getchar();

  close_server();

  printf("bye!\n");

  exit(EXIT_SUCCESS);
}

int main(int argc, char * argv[]) {

	printf("***LAVORI IN CORSO***\n");
	return 0;

  if (signal(SIGCHLD, signal_handler) == SIG_ERR) {
    perror("signal");
    exit(EXIT_FAILURE);
  }

  pthread_t tid;
  pthread_create( & tid, NULL, control_thread, NULL);

  start_server();

  return 0;
}

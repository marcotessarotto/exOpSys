#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h> // signal

#include <errno.h>

/*
 * processo padre e processo figlio mappano in memoria lo stesso file,
 * ognuno per conto proprio; entrambi i processi lavorano sulla stessa
 *
 */


void child_process();
void parent_process();


#define FILE_SIZE 64

/*
 * crea il file (se non esiste già), lo apre in modalità read-write,
 * imposta la dimensione del file e restituisce il file descriptor
 */
int create_file_set_size(char * file_name, unsigned int file_size) {

	// apriamo il file in scrittura, se non esiste verrà creato,
	// se esiste già la sua dimensione viene troncata a 0

	// tratto da man 2 open
	// O_CREAT  If pathname does not exist, create it as a regular file.
	// O_TRUNC  If the file already exists and is a regular file and the access mode allows writing ... it will be truncated to length 0.
	// O_RDONLY, O_WRONLY, or O_RDWR  These request opening the file read-only, write-only, or read/write, respectively.

	int fd = open(file_name,
				  O_CREAT | O_RDWR,
				  S_IRUSR | S_IWUSR // l'utente proprietario del file avrà i permessi di lettura e scrittura sul nuovo file
				 );

	if (fd == -1) { // errore!
			perror("open()");
			return -1;
		}

	int res = ftruncate(fd, file_size);

	if (res == -1) {
		perror("ftruncate()");
		return -1;
	}

	return fd;
}


char * file_name = "output.txt";



int main(int argc, char * argv[]) {

	unsigned int file_size = FILE_SIZE;

	int res;
	int fd;

	if (argc > 1) {
		file_name = argv[1];

		if (argc > 2) {
			unsigned int temp;
			res = sscanf(argv[2], "%u", &temp);
			if (res == 1)
				file_size = temp;
		}
	}

	fd = create_file_set_size(file_name, file_size);

	if (fd == -1) {
		exit(EXIT_FAILURE);
	}

	close(fd);

	switch (fork()) {
	case 0:

		child_process();

		exit(EXIT_SUCCESS);
	case -1:
		perror("fork()");
		exit(EXIT_FAILURE);
	default:

		parent_process();
		exit(EXIT_SUCCESS);
	}


}

// restituisce la dimensione del file, -1 in caso di errore
__off_t get_file_size(char * file_name) {

	struct stat sb;
	int res;

	res = stat(file_name, &sb);

	if (res == -1) {
		perror("stat()");
		return -1;
	}

	return sb.st_size;
}

// restituisce la dimensione del file, -1 in caso di errore
__off_t get_fd_size(int fd) {

	struct stat sb;
	int res;

	res = fstat(fd, &sb);

	if (res == -1) {
		perror("fstat()");
		return -1;
	}

    // printf("File size: %lld bytes\n", (long long) sb.st_size);

    return sb.st_size;
}


// restituisce l'indirizzo della memory map, NULL in caso di errore
// questa funzione è usata da entrambi i processi
char * common_prepare_memory_map(char * fname, __off_t * file_size) {

	char * addr;

	// ipotesi: il file esiste già
	int fd = open(fname, O_RDWR); // apriamo il file in modalità read-write

	if (fd == -1) { // errore!
		perror("open()");
		return NULL;
	}

	*file_size = get_fd_size(fd);

	// mmap restituisce l'indirizzo della zona di memoria dove
	// è mappato tutto il file (o una sua parte)
	addr = mmap(NULL, // NULL: è il kernel a scegliere l'indirizzo
			*file_size, // dimensione della memory map
			PROT_READ | PROT_WRITE, // memory map leggibile e scrivibile
			MAP_SHARED, // memory map condivisibile con altri processi
			fd,
			0); // offset nel file

	close(fd);

	if (addr == MAP_FAILED) {
		perror("mmap()");
		return NULL;
	}

	return addr;
}


// ipotesi: il processo figlio conosca soltanto il nome del file su cui operare
// il processo figlio mappa in memoria il file,
// e per 10 volte a distanza di un secondo scrive in tutto la memory map un carattere
void child_process() {

	char * addr;
	__off_t file_size;
	char val = 'a';
	int counter = 0;

	addr = common_prepare_memory_map(file_name, &file_size);

	if (addr == NULL) {
		exit(EXIT_FAILURE);
	}

	// in una shell dentro la cartella di progetto eseguire (quando il file output.txt esiste già) :
	// watch -n 1 cat output.txt

	while (counter++ < 10) {

		printf("[child] before fill memory map with '%c'\n", val);

		for (__off_t i = 0; i < file_size; i++)
			addr[i] = val;

		val++;

		if (val > 'z')
			val = 'a';

		sleep(1); // il processo dorme un secondo
	}

	// non servirebbe perchè il processo non fa altro
	// e quindi ci penserà il kernel a liberare tutte le risorse usate dal processo
	if (munmap(addr, file_size) == -1) {
		perror("munmap");
	}

	printf("[child] terminating\n");

}


void parent_process_signal_handler(int signum) {
	// riceviamo SIGCHLD: Child stopped or terminated

	pid_t child_pid;

	// NON è sicuro chiamare printf da un signal handler!
	// printf non è async-signal-safe (vedere Kerrisk sezione 21.1.2)
	// printf è usato qui solo a scopo diagnostico/didattico
	printf("[parent] parent_process_signal_handler\n");

	child_pid = wait(NULL);

	printf("[parent] signal handler: process %u has terminated, exiting\n", child_pid);

	exit(EXIT_SUCCESS);
}


void parent_process() {

	char * addr;
	__off_t file_size;
	__off_t pos;
	char * previous_addr;

	// il processo padre registra il gestore di segnale per SIGCHLD
	if (signal(SIGCHLD, parent_process_signal_handler) == SIG_ERR) {
		perror("signal");
		exit(EXIT_FAILURE);
	}

	addr = common_prepare_memory_map(file_name, &file_size);

	if (addr == NULL) {
		exit(EXIT_FAILURE);
	}

	previous_addr = malloc(file_size);
	if (previous_addr == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	memcpy(previous_addr, addr, file_size);

	// monitora continuamente la memory map per cambiamenti apportati dall'altro processo
	while(1) {
		for (pos = 0; pos < file_size; pos++) {
			if (addr[pos] != previous_addr[pos]) {
				printf("[parent] addr[%ld] has changed, new value: '%c'\n", pos, addr[pos]);
				break;
			}
		}

		memcpy(previous_addr, addr, file_size);
	}


}

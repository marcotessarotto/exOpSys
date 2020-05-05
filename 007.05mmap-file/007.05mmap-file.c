#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <errno.h>

#include <openssl/evp.h>

/*
 * richiede: sudo apt install libssl-dev
 *
 * il programma crea un file di dimensione FILE_SIZE,
 * lo mappa in memoria, ci scrive dei dati, crea un processo figlio
 * per calcolare SHA3_512 dei dati.
 *
 *
 * https://github.com/marcotessarotto/openssl-sha3/
 * https://en.wikipedia.org/wiki/SHA-3
 *
 * SHA3 -> secure hash algorithm
 *
 * The Secure Hash Algorithms are a family of cryptographic hash functions
 * published by the National Institute of Standards and Technology (NIST)
 * as a U.S. Federal Information Processing Standard (FIPS).
 */

#define FILE_SIZE 1024*1024*16

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


#define HANDLE_ERROR(msg) { fprintf(stderr, "%s\n", msg); exit(EXIT_FAILURE); }
#define HANDLE_ERROR2(msg, mdctx) { fprintf(stderr, "%s\n", msg); EVP_MD_CTX_destroy(mdctx); exit(EXIT_FAILURE); }

unsigned char * sha3_512(char * addr, unsigned int size, int * result_len_ptr) {

	EVP_MD_CTX * mdctx;
	int val;
	unsigned char * digest;
	unsigned int digest_len;
	EVP_MD * algo = NULL;

	algo = EVP_sha3_512();

	if ((mdctx = EVP_MD_CTX_create()) == NULL) {
		HANDLE_ERROR("EVP_MD_CTX_create() error")
	}

	// initialize digest engine
	if (EVP_DigestInit_ex(mdctx, algo, NULL) != 1) { // returns 1 if successful
		HANDLE_ERROR2("EVP_DigestInit_ex() error", mdctx)
	}

	// provide data to digest engine
	if (EVP_DigestUpdate(mdctx, addr, size) != 1) { // returns 1 if successful
		HANDLE_ERROR2("EVP_DigestUpdate() error", mdctx)
	}

	digest_len = EVP_MD_size(algo); // sha3_512 returns a 512 bit hash

	if ((digest = (unsigned char *)OPENSSL_malloc(digest_len)) == NULL) {
		HANDLE_ERROR2("OPENSSL_malloc() error", mdctx)
	}

	// produce digest
	if (EVP_DigestFinal_ex(mdctx, digest, &digest_len) != 1) { // returns 1 if successful
		OPENSSL_free(digest);
		HANDLE_ERROR2("EVP_DigestFinal_ex() error", mdctx)
	}

	char * result = malloc(digest_len);
	if (result == NULL) {
		perror("malloc()");
		exit(EXIT_FAILURE);
	}

	memcpy(result, digest, digest_len);

	*result_len_ptr = digest_len;

	OPENSSL_free(digest);
	EVP_MD_CTX_destroy(mdctx);

	return result;
}

int main(int argc, char * argv[]) {

	char * file_name = "output.txt";
	unsigned int file_size = FILE_SIZE;

	int res;
	int fd;

	char * addr;

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

	// mappiamo nella memoria virtuale del processo l'intero file
	//

	// man 2 mmap
	// MAP_SHARED: Share this mapping.  Updates to the mapping are visible to other processes
	// mapping the same region, and (in the case of file-backed mappings) are carried  through  to  the
    // underlying file.

	// mmap restituisce l'indirizzo della zona di memoria dove
	// è mappato tutto il file (o una sua parte)
	addr = mmap(NULL, // NULL: è il kernel a scegliere l'indirizzo
			file_size, // dimensione della memory map
			PROT_READ | PROT_WRITE, // memory map leggibile e scrivibile
			MAP_SHARED, // memory map condivisibile con altri processi
			fd,
			0); // offset nel file

	if (addr == MAP_FAILED) {
		perror("mmap()");
		exit(EXIT_FAILURE);
	}

	close(fd); // il file descriptor si può chiudere subito dopo mmap, la memory map rimane attiva

	// mmap mette a disposizione l'intero contenuto del file tramite
	// un array di char allocato nella memoria del processo.
	// possiamo leggere e scrivere l'array (e quindi il file).

	// il kernel sincronizza la memory map con il file sicuramente quando:
	// - la memory map viene chiusa con munmap()
	// - un processo chiama msync()

	// la memory map è visibile dal processo figlio (MAP_SHARED),
	// è una tecnica di Inter Process Communication (IPC)
	// perchè permette ai processi di scambiare dati tra di loro.

	// nel resto dell'esempio, il processo padre scrive dei dati nella memory map
	// poi crea il processo figlio; questo calcola SHA3 dei dati e li restituisce
	// scrivendoli all'inizio della stessa memory map.
	// il processo padre riceve il risultato quando esce da wait().

	// esercizio: modificare il programma aggiungendo una seconda memory map dove
	// il processo figlio scrive il risultato (senza "rovinare" i dati nella prima memory map)

	memset(addr, 0xFF, file_size);

	unsigned char * digest;
	int digest_len;

	switch (fork()) {
	case 0: // child process

		digest = sha3_512(addr, file_size, &digest_len);

		// copy hash to memory map
		memcpy(addr, digest, digest_len);

		printf("[child] SHA3_512 on %u bytes: ", file_size);

		for (int i = 0; i < digest_len; i++) {
			printf("%02x", digest[i]);
		}

		printf("\n[child] bye!\n");

		exit(EXIT_SUCCESS);
	case -1:
		perror("fork()");
		exit(EXIT_FAILURE);
	}

	if (wait(NULL) == -1) {
		perror("wait()");
		exit(EXIT_FAILURE);
	}

	printf("[parent] SHA3_512 from child process: ");
	for (int i = 0; i < 512 / 8; i++) {
		printf("%02x", addr[i] & 0xFF);
	}

	printf("\n[parent]bye!\n");

	return 0;
}

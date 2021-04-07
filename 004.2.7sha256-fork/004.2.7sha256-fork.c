#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>


// building on Debian requires libssl-dev package (sudo apt install libssl-dev)

/*
 *
 * SHA-256 (256 bit) is part of SHA-2 set of cryptographic hash functions, designed by the U.S. National Security Agency (NSA) and
 * published in 2001 by the NIST as a U.S. Federal Information Processing Standard (FIPS).
 *
 * A hash function is an algorithm that transforms (hashes) an arbitrary set of data elements, such as a text file,
 * into a single fixed length value (the hash).
 * The computed hash value may then be used to verify the integrity of copies of the original data without providing
 * any means to derive said original data.
 * Irreversible, a hash value may be freely distributed, stored and used for comparative purposes.
 * SHA stands for Secure Hash Algorithm.
 *
 *
 * this program takes one or more file names as parameters; for each file it forks itself and calculates sha256 digest.
 *
 */

struct digest_result {
	unsigned int digest_len;
	unsigned char * digest;
};

#define BUF_SIZE 1024*16

struct digest_result calculate_sha256(int fd) {

	char * buffer;
	int bytes_read;
	struct digest_result result = { 0, NULL}; // if returned, means this functions was not successful

	EVP_MD_CTX * mdctx;
	unsigned char * digest;
	unsigned int digest_len;
	EVP_MD * algo;

	buffer = malloc(BUF_SIZE);
	if (buffer == NULL) {
		perror("malloc()");
		return result;
	}

	algo = EVP_sha256();

	if ((mdctx = EVP_MD_CTX_create()) == NULL) {
		printf("EVP_MD_CTX_create() error\n");
		return result;
	}

	// initialize digest engine
	if (EVP_DigestInit_ex(mdctx, algo, NULL) != 1) { // EVP_DigestInit_ex returns 1 if successful
		EVP_MD_CTX_destroy(mdctx);
		printf("EVP_DigestInit_ex() error\n");
		return result;
	}

	while ((bytes_read = read(fd, buffer, BUF_SIZE)) > 0) { // read returns 0 on EOF, -1 on error
		// provide data to digest engine
		if (EVP_DigestUpdate(mdctx, buffer, bytes_read) != 1) { // EVP_DigestUpdate returns 1 if successful
			EVP_MD_CTX_destroy(mdctx);
			printf("EVP_DigestUpdate() error\n");
			return result;
		}

	}

	if (bytes_read == -1) {
		perror("read error");
		exit(1);
	}

	digest_len = EVP_MD_size(algo);

	if ((digest = (unsigned char *)OPENSSL_malloc(digest_len)) == NULL) {
		EVP_MD_CTX_destroy(mdctx);
		printf("OPENSSL_malloc() error\n");
		return result;
	}

	// produce digest
	if (EVP_DigestFinal_ex(mdctx, digest, &digest_len) != 1) { // EVP_DigestFinal_ex returns 1 if successful
		OPENSSL_free(digest);
		EVP_MD_CTX_destroy(mdctx);
		printf("EVP_DigestFinal_ex() error\n");
		return result;
	}

	//OPENSSL_free(digest);
	EVP_MD_CTX_destroy(mdctx);

	free(buffer);

	result.digest = digest;
	result.digest_len = digest_len;

	return result; // OK: a copy of struct result is returned
}



int main(int argc,  char * argv[]) {

	if (argc  == 1) {
		printf("provide one or more file name(s), I will calculate sha256 digest of each file by forking myself\n");
		exit(1);
	}

	int wstatus;
	int modal_result = -1;
	int pid;
	int fd;
	struct digest_result result;


	for (int i = 1; i < argc; i++) {

		fd = open(argv[i], O_RDONLY);
		if (fd == -1) {
			perror("open()");
			continue;
		}

		switch( pid = fork() ) {
		case -1:
			perror("fork()");
			exit(1);
		case 0: // child process

			result = calculate_sha256(fd);

			if (result.digest_len == 0) {
				printf("ERROR! pid=%d\n", getpid());
				exit(1);
			}

			// for sha256, digest has len of 32 bytes
			// printf("%d\n", result.digest_len);

			printf("pid=%d filename=%s ", getpid(), argv[i]);
			for (int i = 0; i < result.digest_len; i++) {
				printf("%02x", result.digest[i]);
			}

			printf("\n");

			OPENSSL_free(result.digest);

			exit(0);

			break;
		default:
			// parent
			close(fd);
		}

	}

	while ((pid = wait(&wstatus)) != -1) {
        if (WIFEXITED(wstatus)) {

        	modal_result = WEXITSTATUS(wstatus);

            printf("[parent] child process %d ha terminato, ha restituito: %d\n", pid, modal_result);
        }
		//
	}

	printf("[parent] finished!\n");

	return 0;
}

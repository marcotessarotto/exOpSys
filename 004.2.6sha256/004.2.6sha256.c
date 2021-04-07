#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// building on Debian requires libssl-dev package (sudo apt install libssl-dev)

#define BUF_SIZE 1024


#define HANDLE_ERROR(msg) { fprintf(stderr, "%s\n", msg); exit(EXIT_FAILURE); }
#define HANDLE_ERROR2(msg, mdctx) { fprintf(stderr, "%s\n", msg); EVP_MD_CTX_destroy(mdctx); exit(EXIT_FAILURE); }

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
 * test: compare outputs of:
 *
 * sha256sum /usr/bin/ls
 *
 * Debug/004.2.6sha256 /usr/bin/ls
 *
 */

int main(int argc,  char * argv[]) {

	char buffer[BUF_SIZE];
	int bytes_read;

	EVP_MD_CTX * mdctx;
	unsigned char * digest;
	unsigned int digest_len;
	EVP_MD * algo;

	algo = EVP_sha256();

	if (argc > 1) {
		// optional parameter is considered as name of the file to read
		int fd;

		fd = open(argv[1], O_RDONLY);

		if (fd == -1) {
			perror("open()");
			exit(1);
		}

		if (dup2(fd, STDIN_FILENO) == -1) {
			perror("dup2()");
			exit(1);
		}

		if (close(fd) == -1) {
			perror("close()");
		}
	}


	if ((mdctx = EVP_MD_CTX_create()) == NULL) {
		HANDLE_ERROR("EVP_MD_CTX_create() error")
	}

	// initialize digest engine
	if (EVP_DigestInit_ex(mdctx, algo, NULL) != 1) { // EVP_DigestInit_ex returns 1 if successful
		HANDLE_ERROR2("EVP_DigestInit_ex() error", mdctx)
	}

	while ((bytes_read = read(STDIN_FILENO, buffer, BUF_SIZE)) > 0) { // read returns 0 on EOF, -1 on error
		// provide data to digest engine
		if (EVP_DigestUpdate(mdctx, buffer, bytes_read) != 1) { // EVP_DigestUpdate returns 1 if successful
			HANDLE_ERROR2("EVP_DigestUpdate() error", mdctx)
		}

	}

	if (bytes_read == -1) {
		perror("read error");
		exit(1);
	}

	digest_len = EVP_MD_size(algo);

	if ((digest = (unsigned char *)OPENSSL_malloc(digest_len)) == NULL) {
		HANDLE_ERROR2("OPENSSL_malloc() error", mdctx)
	}

	// produce digest
	if (EVP_DigestFinal_ex(mdctx, digest, &digest_len) != 1) { // EVP_DigestFinal_ex returns 1 if successful
		OPENSSL_free(digest);
		HANDLE_ERROR2("EVP_DigestFinal_ex() error", mdctx)
	}

	for (int i = 0; i < digest_len; i++) {
		printf("%02x", digest[i]);
	}

	OPENSSL_free(digest);
	EVP_MD_CTX_destroy(mdctx);

	return 0;
}

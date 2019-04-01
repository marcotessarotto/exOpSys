#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "files_common.h"


void * create_anon_mmap(size_t size) {

	// MAP_SHARED: condivisibile tra processi
	// PROT_READ | PROT_WRITE: posso leggere e scrivere nello spazio di memoria
	void * memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	if (memory == MAP_FAILED) {
		fprintf(stderr, "mmap error:%s\n", strerror(errno));

		return NULL;
	}

	return memory;

}


#define ONE_GB (1024*1024*1024L)

void test_mmap(void) {

	printf("esempio di allocazione di spazio di memoria con system call mmap\n");


	void * buffer = create_anon_mmap(ONE_GB);

	if (buffer == NULL) {

		return;
	}

	printf("allocated mmap size=%ld\n", ONE_GB);

	// do something

	if (munmap(buffer, ONE_GB) == -1) {
		fprintf(stderr, "munmap error:%s\n", strerror(errno));
	}

	printf("free mmap\n\n");

}

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


#define ONE_GB (1024*1024*1024UL)

void test_mmap(void) {

	printf("esempio di allocazione di spazio di memoria con system call mmap\n\n");

	print_system_info();

    printf("prima di mmap\n\n");



	char * buffer = create_anon_mmap(ONE_GB);

	if (buffer == NULL) {
		return;
	} else {
		printf("la memory map anonima è stata allocata (ma non ancora utilizzata), size=%lu\n\n", ONE_GB);
	}

	print_system_info();

	printf("dopo mmap\n\n");

	printf("premi invio per continuare\n");
	getchar();

	print_system_info();
	printf("ora utilizziamo il 10%% della memory map\n\n");

	for (unsigned long i = 0; i < ONE_GB / 10; i++)
		buffer[i] = i & 0xFF;

	print_system_info();
	printf("ho usato il 10%% della memory map\n");

	printf("\npremi invio per continuare\n");
	getchar();

	if (munmap(buffer, ONE_GB) == -1) {
		fprintf(stderr, "munmap error:%s\n", strerror(errno));
	}


	printf("la memory map è stata liberata\n\n");
	print_system_info();

	getchar();

}

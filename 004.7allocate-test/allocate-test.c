#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *argv[]) {

	// esempio: allochiamo 1, 10, 100 GB di memoria:

	// vedere:
	// man 3 malloc

	// void *malloc(size_t size);
	// size_t è 'unsigned long'

	//printf("sizeof(size_t)= %lu\n", sizeof(size_t));

	// nota:
	// perchè scrivo "1024 * 1024 * 1024L" (con la L sull'ultimo 1024) ?
	// cosa succede se scrivo "1024 * 1024 * 1024" ? (provate...)

#define ONE_GIGABYTE (1024 * 1024 * 1024L)

	unsigned long alloc_test [] = { ONE_GIGABYTE, 10 * ONE_GIGABYTE, 100 * ONE_GIGABYTE, -1 };

	unsigned long * ptr = alloc_test; // &alloc_test[0]

	//printf("alloc_test[3] = %lu\n", alloc_test[3]);

	while (*ptr != -1) {
		printf("provo ad allocare %lu GB...", *ptr / ONE_GIGABYTE);

		char * test = malloc(*ptr);

		if (test == NULL) {
			printf("richiesta malloc: fallita!\n");
		} else {
			printf("richiesta malloc: OK!\n");
		}

		free(test); // se passo NULL a free è ok, non succede nulla (vedere man free)

		ptr++;
	}

	return EXIT_SUCCESS;
}


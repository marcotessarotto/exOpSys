#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cap1.h"


/*
 * launch program with debug:
 *
 * export DEBUG=something
 * ./00libro
 *
 * or
 *
 * DEBUG=1 ./00libro
 */

#define EX1_8 18
#define EX1_9 19
#define EX1_10 110

int main(int argc, char *argv[]) {

	int exercise  = EX1_8;

	if (argc > 1 && !strcmp(argv[1],"-h")) {
		printf("00libro [parameter]\n");
		printf("parameter:\n");

		printf("1-8\n");
		printf("1-9\n");
		printf("1-10\n");

		return 0;
	} else if (argc > 1) {
		if (!strcmp(argv[1],"1-8"))
			exercise  = EX1_8;
		else if (!strcmp(argv[1],"1-9"))
			exercise  = EX1_9;
		else if (!strcmp(argv[1],"1-10"))
			exercise  = EX1_10;
	}



	isDebugEnvSet();

	switch (exercise) {
		case EX1_8:
			count_blanks_tabs_newlines_es_1_8();
			break;
		case EX1_9:
			copy_io_replace_1_9();
			break;
		case EX1_10:
			copy_io_replace_1_10();
			break;
	}

	return 0;

}

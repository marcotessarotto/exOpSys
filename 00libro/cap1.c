#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cap1.h"

extern char **environ;  /* Or define _GNU_SOURCE to get it from <unistd.h> */

int DEBUG = 0;

int prefix(const char *pre, const char *str);

void isDebugEnvSet(void) {

    char **ep;

    for (ep = environ; *ep != NULL; ep++) {
//    	puts(*ep);
    	if (prefix("DEBUG=", *ep)) {
    		printf("DEBUG is set\n");
    		DEBUG = 1;

    		return;
    	}
    }

}

int prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

void count_lines_in_input() {

	int c;
	int numero_linee = 0;

	while ((c = getchar()) != EOF)
		if (c == '\n')
			numero_linee++;

	char spazio_vuoto = ' ';

	// scrivere il risultato

}

enum TYPES {
	BLANK, TAB, NEWLINE, NUM_TYPES
};

// enum TYPES type_names[] = {  BLANK, TAB, NEWLINE };

#define STR(x) #x

void count_blanks_tabs_newlines_es_1_8(void) {

	int counters[NUM_TYPES];
	int c;

	for (int i = 0; i < NUM_TYPES; i++)
		counters[i] = 0;

	printf("count_blanks_tabs_newlines_es_1_8()\n");


	while ((c = getchar()) != EOF) {
		if (DEBUG)
			printf("char %d\n", c);

		switch (c) {
			case ' ':
				counters[BLANK]++;
				break;
			case '\t':
				counters[TAB]++;
				break;
			case '\n':
				counters[NEWLINE]++;
				break;
			default:
				break;
		}

//		if (c == ' ')
//			counters[BLANK]++;
//		else if (c == '\t')
//			counters[TAB]++;
//		else if (c == '\n')
//			counters[NEWLINE]++;
	}

	printf("results:\n");

	printf(STR(BLANK) " = %d\n", counters[BLANK]);
	printf(STR(TAB) " = %d\n", counters[TAB]);
	printf(STR(NEWLINE) " = %d\n", counters[BLANK]);

	printf("finished! bye\n");
}

#define OUTSIDE_SPACE 0
#define INSIDE_SPACE 1

void copy_io_replace_1_9(void) {

	int ch;
	register int state = OUTSIDE_SPACE;

	printf("copy_io_replace_1_9()\n");

	while ((ch = getchar()) != EOF) {

		if (ch == ' ') {
			if (state == OUTSIDE_SPACE) {
				state = INSIDE_SPACE;
				putchar(ch);
			}
		} else {
			if (state == INSIDE_SPACE)
				state = OUTSIDE_SPACE;
			putchar(ch);
		}

	}

	printf("finished! bye\n");

}



void copy_io_replace_1_10(void) {

	int ch;

	printf("copy_io_replace_1_10()\n");

	while ((ch = getchar()) != EOF) {

		switch (ch) {
			case '\t':
				printf("\\t");
				break;
			case '\b':
				printf("\\b");
				break;
			case '\\':
				printf("\\\\");
				break;
			default:
				putchar(ch);
				break;
		}

	}

	printf("finished! bye\n");

}


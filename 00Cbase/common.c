#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "common_settings.h"





/*
 * getchat "alternativo"; anche il getchar originale usa la system call read
 */
int my_getchar() {

	char ch; // sizeof(char) vale 1

	ssize_t s = read(STDIN_FILENO, &ch, 1); // system call; leggiamo esattamente un byte da stdin

	if (s == -1) {
		// error
		fprintf(stderr, "my_getchar error: %d  %s\n", errno, strerror(errno));
		// https://stackoverflow.com/a/504039/974287

		return EOF;
	} else if (s == 0) {
		// EOF
		return EOF;
	} else {
		// ok il dato c'è
		if (isDebugEnabled())
			printf("my_getchar: %d\n", ch);

		return ch;
	}
}

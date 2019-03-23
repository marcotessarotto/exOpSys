#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <termios.h>

#include "common_settings.h"


extern char **environ;  /* Or define _GNU_SOURCE to get it from <unistd.h> */

int DEBUG = 0;
int TTY_ECHO_OFF = 0;

int is_debug_enabled() {
	return DEBUG;
}

int is_tty_echo_enabled() {
	return TTY_ECHO_OFF;
}

// "java".startsWith("abc"): restituisce true se la stringa inizia per ....
int prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}


void check_debug_env_set(void) {

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

void check_tty_echo_off_env_set(void) {
    char **ep;

    for (ep = environ; *ep != NULL; ep++) {
    	if (prefix("TTY_ECHO_OFF=", *ep)) {
    		printf("TTY_ECHO_OFF is set\n");
    		TTY_ECHO_OFF = 1;

    		return;
    	}
    }
}

// https://stackoverflow.com/a/1798833/974287
void set_tty_echo(int set) {

    static struct termios configuration;

    /* tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to configuration */
    tcgetattr(STDIN_FILENO, &configuration);

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    configuration.c_lflag &= set ? ICANON : ~(ICANON);

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &configuration);

    /*This is your part:
    I choose 'e' to end input. Notice that EOF is also turned off
    in the non-canonical mode*/
//    while((c=getchar())!= 'e')
//        putchar(c);

}


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
		if (is_debug_enabled())
			printf("my_getchar: %d\n", ch);

		return ch;
	}
}

#include <stdio.h>    // <== direttive del preprocessore
#include <stdlib.h>   // i simboli < > dicono al preprocessore di cercare i file AL DI FUORI della cartella di progetto
#include <string.h>   // il preprocessore legge tutto il file string.h e lo inserisce al posto della singola linea
#include <unistd.h>   // ad esempio: path completo è /usr/include/unist.h (è un file "di sistema")

#include "common_settings.h"
#include "hello_world.h"
#include "temp_conversion.h"
#include "char_io.h"
#include "call_examples.h"

/*
 * NOTA BENE:
 * questi esempi sono tratti dal libro "The C Programming Language (Second Edition)"
 * di Kernighan e Ritchie
 */

// DUE TIPI DI COMMENTI...

/*
 * altro tipo di commento
 */

extern char **environ;  /* Or define _GNU_SOURCE to get it from <unistd.h> */

int DEBUG = 0;

int isDebugEnabled() {
	return DEBUG;
}

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

// "java".startsWith("abc"): restituisce true se la stringa inizia per ....
int prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}


enum COSA_ESEGUIRE_TYPE {
	NOP, PRINTF, TEMPERATURE, CHAR_IO, FUNCTION_CALLS
} cosa_eseguire;


int main(int argc, char *argv[]) {
	// scorciatoia per eseguire: Ctrl+F11

	//////////////////////////////////////////
	// modificare questa variabile per eseguire la sezione di codice voluta
	cosa_eseguire = FUNCTION_CALLS;
	//////////////////////////////////////////

	isDebugEnvSet();

	if (cosa_eseguire == NOP) {
		printf("non faccio nulla! devi cambiare il valore della variabile 'cosa_eseguire'! Bye!\n");
	} else if (cosa_eseguire == PRINTF) {

		hello_world();
		hello_world_string_multi_line();
		hello_world_multi_printf();
		hello_world_printf();

	} else if (cosa_eseguire == TEMPERATURE) {

		temperature_conversion_int();

		temperature_conversion_float();

		temperature_conversion_float_for();

		temperature_conversion_celsius_to_fahrenheit();

	} else if (cosa_eseguire == CHAR_IO) { // IO: input-output

//		copy_input_to_output();

//		count_lines_in_input();
//
		word_count();

//		count_to_array();

	} else if (cosa_eseguire == FUNCTION_CALLS) {

		function_call_examples();

	}




}

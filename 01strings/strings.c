#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <errno.h>


const char * SAMPLE_STRING = "The strchr() and strrchr() functions return a pointer to the matched\n"
	       "character or NULL if the character is not found.  The terminating\n"
	       "null byte is considered part of the string, so that if c is specified\n"
	       "as '\\0', these functions return a pointer to the terminator.\n";


void string_concat(void) {

	printf("\n***string_concat()\n\n");

	char strA[] = "prima parte";

	char strB[] = "seconda parte";

	char space[] = " ";

	printf("strA = %s\n", strA);
	printf("strB = %s\n", strB);
	printf("space = %s\n", space);


	// vogliamo concatenare strA, space e strB
	int len_strA = strlen(strA);
	int len_strB = strlen(strB);
	int len_space = strlen(space);

	printf("len_strA = %d\n", len_strA);
	printf("len_strB = %d\n", len_strB);
	printf("len_space = %d\n", len_space);

	int len_concat_str = len_strA + len_space + len_strB + 1; // \0 finale

	printf("len_concat_str = %d\n", len_concat_str);

	printf("\n\n");

	char * concat_str = malloc(sizeof(char) * len_concat_str);

	concat_str[0] = '\0'; // inizializzo a stringa vuota
	printf("concat_str = [%s], len=%ld\n", concat_str, strlen(concat_str));

	strcat(concat_str, strA); // oppure: strcpy(concat_str, strA);
	printf("concat_str = [%s], len=%ld\n", concat_str, strlen(concat_str));

	strcat(concat_str, space);
	printf("concat_str = [%s], len=%ld\n", concat_str, strlen(concat_str));

	strcat(concat_str, strB);
	printf("concat_str = [%s], len=%ld\n", concat_str, strlen(concat_str));


}

void string_search(void) {

	printf("\n***string_search()\n\n");

	const char * test_string = SAMPLE_STRING;

	char * pos;
	char search_char = 'a';

	printf("stringa di lavoro: [%s]\n\n", test_string);

	printf("cerchiamo il carattere %c nella stringa.\n", search_char);

	pos = strchr(test_string, search_char);

	if (pos != NULL) {

		printf("carattere trovato!\n");

		printf("indirizzo di test_string: %p\n", test_string);
		printf("indirizzo del carattere cercato: %p\n", pos);

		printf("posizione del carattere all'interno della stringa: %p (%ld) - %p (%ld) = %d\n",
				pos, (unsigned long) pos,
				test_string, (unsigned long) test_string,
				(int) (pos - test_string));

	} else {
		printf("carattere NON trovato!\n");
	}

	printf("\n\n");

	char * search_str = "pointer";

	pos = strstr(test_string, search_str);

	if (pos != NULL) {
		printf("stringa [%s] trovata alla posizione %d\n", search_str, (int) (pos - test_string));
	} else {
		printf("stringa [%s] NON trovata!\n", search_str);
	}


	search_str = "THE";

	pos = strcasestr(test_string, search_str); // ignora il case delle stringhe; non standard extension

	if (pos != NULL) {
		printf("stringa [%s] trovata alla posizione %d\n", search_str, (int) (pos - test_string));
	} else {
		printf("stringa [%s] NON trovata!\n", search_str);
	}


}


void string_token(void)
{

	printf("\n***string_token()\n\n");

	char * word;
	char * saveptr;
	char cmd [] = "ls -la   notes.txt abcd.txt";

	printf( "original string: [%s]\n", cmd );

	//  char temp[1024];
	//  strcpy( temp, cmd );   /* first make a copy of the cmd */

	/* before: "ls -la    notes.txt" */

	printf("caratteri delimitatori: spazio e \\t\nsaveptr salva lo stato del parsing della stringa\n");
	printf("la stringa viene modificata (vengono inseriti \\0 a separare i 'token')\n");

	// strtok_r è "thread safe"
	word = strtok_r( cmd, " \t", &saveptr ); // nella prima chiamata, si passa cmd; NULL nelle successive
	/* after:  "ls\0-la    notes.txt" */
	/*          ^
			 word   */

	while ( word != NULL )
	{
		printf( "[%s] address=%p index=%d\n",word, word, (int) (word- cmd));
		word = strtok_r( NULL, " \t", &saveptr );
	}

	printf("fine strtok_r\n");

	/* after: "ls\0-la\0   notes.txt\0 ...." */

//	printf( "[%s]\n", cmd );

}


void string_operations() {

	printf("\n***string_operations()\n\n");

	// upper case? lower case?
	/*
       #include <ctype.h>

       int toupper(int c);
       int tolower(int c);
	 */

 	char * my_copy = strdup(SAMPLE_STRING); // usa malloc per creare un duplicato

 	char * old_ptr = my_copy;
 	char * another_str = NULL;

 	size_t len = strlen(my_copy);

 	printf("prima di realloc: len=%ld address=%p [%s]\n", len, my_copy, my_copy);

 	// voglio accodare a my_copy un altra stringa, come faccio?

 	another_str = malloc(1); // questa allocazione provoca una copia in realloc

 	my_copy = realloc(my_copy, len + 1024); // se realloc deve copiare per fare il resize, fa free del vecchio array

 	printf("\ndopo realloc:     len=%ld address=%p [%s]\n\n", strlen(my_copy), my_copy, my_copy);

 	if (old_ptr != my_copy) {
 		printf("l'indirizzo è cambiato perchè realloc ha COPIATO il vecchio array in un nuovo array più grande\n");
 	} else {
 		printf("l'indirizzo NON è cambiato perchè realloc ha aggiunto un pezzo all'array originale\n");
 	}


 	strcat(my_copy, "*****");

 	printf("\ndopo strcat :     len=%ld address=%p [%s]\n", strlen(my_copy), my_copy, my_copy);

 	free(my_copy);

 	free(another_str);
}


int main(int argc, char *argv[]) {

	string_concat();

	string_search();

	string_token();

	string_operations();

	// stringa -> terminato da \0
	// array -> dimensione nota
	// stream di input: EOF

	printf("\n\nbye!");

	return 0;
}

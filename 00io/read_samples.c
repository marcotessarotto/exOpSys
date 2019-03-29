#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int read_2_ints() {

	int counter = 0;

	int a,b;


	while (scanf("%d %d", &a, &b) == 2) {
		printf("a=%d b=%d diff=%d\n", a, b, a - b);

		counter++;
	}

	return counter;
}


/*
 * int my_strlen(char * data, int maxlen);

 restituisce la lunghezza della stringa data, scorrendo al massimo maxlen caratteri.

Se nello scorrimento raggiunge il limite di maxlen caratteri, restituisce -1.
 */

int my_strlen_ok1(char * data, int maxlen) {
	int pos = 0;

	while (*data++ != '\0' && pos++ < maxlen)
		;

	if (pos > maxlen)
		return -1;

	return pos;
}

int my_strlen_ok2(char *data, int maxlen) {

	int len = strlen(data);

	if(len > maxlen)
		return -1;

	return len;
}


int my_strlen_ok3(char *data, int maxlen){

     int length = 0;

     for(int i = 0; i < strlen(data); i++) {
          if(i < maxlen)
               length++;
          else
               return -1;

     }

     return length;
}

void test_strlen() {

	printf("\ntest_strlen\n");

	char * test = "0123456789";

	printf("my_strlen_ok1(str,3) = %d\n", my_strlen_ok1(test, 3));  // -1
	printf("my_strlen_ok1(str,10) = %d\n", my_strlen_ok1(test, 10)); // 10
	printf("my_strlen_ok1(str,11) = %d\n", my_strlen_ok1(test, 11)); // 10

	printf("\n");
	printf("my_strlen_ok2(str,3) = %d\n", my_strlen_ok2(test, 3));  // -1
	printf("my_strlen_ok2(str,10) = %d\n", my_strlen_ok2(test, 10)); // 10
	printf("my_strlen_ok2(str,11) = %d\n", my_strlen_ok2(test, 11)); // 10

	printf("\n");
	printf("my_strlen_ok3(str,3) = %d\n", my_strlen_ok3(test, 3));  // -1
	printf("my_strlen_ok2(str,10) = %d\n", my_strlen_ok3(test, 10)); // 10
	printf("my_strlen_ok3(str,11) = %d\n", my_strlen_ok3(test, 11)); // 10

	printf("\n");
	printf("strnlen(str, 3) = %ld\n", strnlen(test, 3));  // 3
	printf("strnlen(str, 10) = %ld\n", strnlen(test, 10)); // 10
	printf("strnlen(str, 11) = %ld\n", strnlen(test, 11)); // 10

	printf("\n");

	float val = 123.45678;
	printf("%10.2f\n", val); // 10 si riferisce a tutto il numero, completo di '.'

	printf("\n");
}


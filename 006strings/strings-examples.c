#include <stdio.h>
#include <string.h>
#include <limits.h>


int main(int argc, char **argv) {

	char * utf_8_str = "touch 😸.txt";

	printf("UTF-8 string: %s\n", utf_8_str);

	printf("len(utf_8_str) = %ld\n", strlen(utf_8_str));

	for (int i = 0; i < strlen(utf_8_str) + 1; i++) {
		printf("utf_8_str[%d] = %d\n", i, (unsigned char)(utf_8_str[i]));
	}


	return 0;
}

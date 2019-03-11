#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "array_list.h"

#define SIZE 30

int my_enum_function(long id, void * data) {
	printf("key=%ld value=%s\n", id, (char *)data);

	return -1; // for the enumerator: go ahead!
}

void test_array_list() {

	// create array
	ARRAY_LIST * array = create_new_array01();

	if (array == NULL) {
		exit(EXIT_FAILURE);
	}

	// add elements
	for (int i = 0; i < 100; i++) {
		char * buf = malloc(SIZE);

		// first index: 0
		// last index:  SIZE-1
		// number of items : last_index - first_index + 1
		memset(buf, i, SIZE-1); // http://man7.org/linux/man-pages/man3/memset.3.html
		buf[SIZE-1] = 0; // buf becomes a string

		put_item02(array, i ,buf);
	}

	// loop over elements
	for (int i = 0; i < 150; i++) {

		char * data = get_item03(array,i);

		printf("[%d]=%s\n", i, data);

	}

	// remove part of elements
	for (int i = 0; i < 100; i+=2) { // remove only even indexes
		char * data = remove_item04(array, i);

		free(data);
	}

	char value_to_search[SIZE];
	memset(value_to_search, 51, SIZE-1);
	value_to_search[SIZE-1] = 0;

	// search for values
	long search_result = find_value06(array, (void * )value_to_search, strcmp);

	printf("risultato della ricerca con find_value06 = %ld\n", search_result); // should be found!


	// enumerate elements
	enumerate_values07(array,my_enum_function); // should output only elements with odd index


	// destroy array
	destroy_array05(array, free); // destroy everything, for each value (which is a char *) free is used

}


int main(int argc, char *argv[]) {

	printf("exRound3 TASK A starting...\n");

	test_array_list();


	printf("exRound3 TASK A finished successfully!\n");

	// TASK B goes here!!!

	return 0;

}

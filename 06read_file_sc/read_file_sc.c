#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

#define BUFFER_SIZE 1024

struct linked_list {

	char * data;
	int size;

	struct linked_list * next;
};


struct linked_list * create_linked_list_item(char * data, int size) {

	struct linked_list * head = malloc(sizeof(struct linked_list));

	head->next = NULL;
	head->data = data;
	head->size = size;

	return head;
}

/**
 *
 */
struct linked_list * append_item(struct linked_list * head, char * data, int size) {

	if (head == NULL || data == NULL)
		return NULL;

	struct linked_list * item = create_linked_list_item(data, size);

	// reach tail
	struct linked_list * tail = head;

	while (tail->next != NULL)
		tail = tail->next;

	tail->next = item;

//	item->next = head->next;
//
//	head->next = item;

	return item;
}

int get_total_data_size(struct linked_list * head) {

	if (head == NULL)
		return 0;

	int counter = 0;

	do {

		counter += head->size;

		head = head->next;

	} while (head != NULL);

	return counter;
}

void free_linked_list(struct linked_list * head) {
	if (head == NULL)
		return;

	while (head != NULL) {

		printf("freeing data at address %p\n", head->data);

		free(head->data);

		struct linked_list * item = head;

		head = head->next;

		printf("freeing item at address %p\n", item);

		free(item);
	}

}

int main(int argc, char *argv[]) {

//	char * fileName = "/home/marco/testfile.txt";

	char * fileName = "/proc/cpuinfo";

	int fd = open(fileName, O_RDONLY);

	if (fd == -1) { // errore!

		fprintf(stderr,"open error %d\n", errno);

		return -1;
	}

	struct linked_list * head = create_linked_list_item(NULL,0);

	printf("descriptor = %d\n", fd);

	char * buffer;
	ssize_t bytesRead;
	int totalBytesRead = 0;

	buffer = malloc(BUFFER_SIZE * sizeof(char));

	while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {

		append_item(head, buffer, bytesRead);

		fprintf(stdout,"read %ld bytes from file\n", bytesRead);

		totalBytesRead += bytesRead;

		// buffer for next read
		buffer = malloc(BUFFER_SIZE * sizeof(char));

	}

	free(buffer);

	int calculated_total_data_size = get_total_data_size(head);

	fprintf(stdout,"total read %d bytes from file\n", totalBytesRead);

	printf("total data size in linked list: %d\n", calculated_total_data_size);

	free_linked_list(head);

	close(fd);


	return 0;

}

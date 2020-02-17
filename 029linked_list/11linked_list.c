#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct linked_list_node {
	char * value;

	struct linked_list_node * next;
};

struct linked_list {
	struct linked_list_node * head;
	struct linked_list_node * tail;
};

void add_value(struct linked_list * list, char * value) {
	if (value == NULL)
		return;

	struct linked_list_node * node = malloc(sizeof(struct linked_list_node));
	node->value = malloc(strlen(value) * sizeof(char));
	strcpy(node->value, value);
	node->next = NULL;

	if (list->head == NULL) {
		list->head = list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
}

struct linked_list_node * search(struct linked_list * list, char * value) {
	struct linked_list_node * node = list->head;

	while (node != NULL) {
		if (!strcmp(value, node->value))
			return node;

		node = node->next;
	}

	return NULL;
}

void print_linked_list(struct linked_list * list) {

	struct linked_list_node * node = list->head;

	int counter = 0;

	while (node != NULL) {
		printf("#%d=%s\n", counter++, node->value);
		node = node->next;
	}

}

void remove_node(struct linked_list_node * node) {
	free(node->value);
	free(node);
}

int remove_value(struct linked_list * list, char * value) {

	//	1. la lista è vuota; oppure
//	if (list->head == NULL)
//		return 0;
	//	2. il nodo da rimuovere è lunico della lista; oppure
//	if (list->head->next == NULL && !strcmp(list->head->value, value)) {
//		remove_node(list->head);
//		list->head = list->tail = NULL;
//		return -1;
//	}
	//	3. il nodo da rimuovere è head; oppure


	//	4. il nodo da rimuovere è tail; oppure
	//	5. il nodo da rimuovere si trova da qualche parte tra head e tail; oppure
	//	6. il valore da rimuovere non è presente nella lista!


	if (list->head == NULL)
		return 0;

	struct linked_list_node * node = list->head;

	if (!strcmp(node->value, value)) {
		if (list->head == list->tail) {
	  // remove head
			remove_node(list->head);
			list->head = list->tail = NULL;
		}
		else {
		    node = list->head;

		    list->head = node->next;

		    remove_node(node);
		}
		return -1;
	}

	while (node->next != NULL && strcmp(node->next->value, value)) {
		node = node->next;
	}

	if (node->next != NULL) {
		struct linked_list_node * tmp;
		tmp = node->next;

//		if (node->next == list->tail) {
//			remove_node(list->tail);
//			list->tail = node;
//			node->next = NULL;
//		} else { // all'interno
//			node->next = node->next->next;
//			remove_node(tmp);
//		}

		if (node->next == list->tail)
			list->tail = node;

	    node->next = node->next->next;

	    remove_node(tmp);

	    return -1;
	   }

	return 0;

}

int main() {

	struct linked_list list = { NULL, NULL };
//	printf("%x\n", list.head);

	add_value(&list, "123");
	add_value(&list, "abc");
	add_value(&list, "def");
	add_value(&list, "3423534534534");
	add_value(&list, "sdfsfdsf");

	print_linked_list(&list);

	printf("\n");

	remove_value(&list, "def");

	print_linked_list(&list);

	remove_value(&list, "123");
	remove_value(&list, "abc");
	remove_value(&list, "def");
	remove_value(&list, "3423534534534");
	remove_value(&list, "sdfsfdsf");

	print_linked_list(&list);

	printf("\nfinished!\n");

	return 0;
}

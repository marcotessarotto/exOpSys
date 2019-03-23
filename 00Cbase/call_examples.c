#include <stdio.h>

#include "call_examples.h"

/*
 * differenze tra Java e C:
 * in Java non posso restituire una struttura complessa by value
 */

struct INNER_ARRAY return_a_struct() {

	struct INNER_ARRAY result;

	printf("return_a_struct - inizializzo struct INNER_ARRAY e lo restituisco\n");

	for (int i = 0; i < 79; i++)
		result.data[i] = i % 10 + '0';

	result.data[79] = '\0';

	return result;
}

void pass_a_struct(struct INNER_ARRAY param) {

	printf("inside pass_a_struct - modifico param, passato by value (l'originale non viene toccato)\n");

	for (int i = 0; i < 79; i++)
		param.data[i] = i % 3 + 'A';

}

void pass_a_struct_ptr(struct INNER_ARRAY * param) {

	printf("inside pass_a_struct_ptr - modifico param by address\n");

	for (int i = 0; i < 79; i++)
		param->data[i] = i % 3 + 'D';

}


void function_call_examples(void) {

	struct INNER_ARRAY local_var;

	local_var = return_a_struct(); // viene copiato l'intero valore (array dentro struct) restituito dalla funzione in local_var

	printf("valore restituito da return_a_struct: %s\n", local_var.data);


	struct INNER_ARRAY * heap_var;

	heap_var = malloc(sizeof(struct INNER_ARRAY));

	// inizializzo temp_var
	for (int i = 0; i < 80; i++)
		heap_var->data[i] = 'Z';

	printf("\n");

	printf("heap_var: %s\n", heap_var->data);

	pass_a_struct(*heap_var); // by value! la funzione lavora su una copia, l'originale non viene modificato

	printf("heap_var: %s\n", heap_var->data);

	printf("\n");


	printf("local_var: %s\n", local_var.data);

	pass_a_struct(local_var); // by value! la funzione lavora su una copia

	printf("local_var: %s\n", local_var.data);

	printf("\n");

	pass_a_struct_ptr(&local_var); // by address! la funzione lavora sull'originale

	pass_a_struct_ptr(heap_var); // by address! la funzione lavora sull'originale


	printf("\n");
	printf("entrambe le variabili sono state modificate:\n");

	printf("heap_var: %s\n", heap_var->data);

	printf("local_var: %s\n", local_var.data);

	free(heap_var);
}

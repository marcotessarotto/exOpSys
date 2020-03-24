#include <stdio.h>
#include <stdlib.h>


void bubble_sort(int * array, int array_dimension);

int main() {


	int int_array [] = {
			10,9,8,7,6,5,4,3,2,1
	};

    // sizeof(rnd_array_int) restituisce la dimensione totale dell'array,
    // lo può fare perchè il numero di celle dell'array è noto

    int int_array_len = sizeof(int_array) / sizeof(int); // numero di celle dell'array

    // SCRIVERE bubble_sort (definizione più sotto)
    bubble_sort(int_array, int_array_len);

    printf("risultato di bubble_sort():\n");

    for (int i = 0; i < int_array_len; i++) {
    	printf("%d ", int_array[i]);
    }

    printf("\n");

}

void swap_int(int *x, int *y) {
	int t = *x;
	*x = *y;
	*y = t;
}


/*
 * la funzione bubble_sort ordina l'array passato come parametro
 * array_dimension: numero di celle dell'array
 */
void bubble_sort(int * array, int array_dimension) {

	// ESERCIZIO: implementare bubble sort (pseudocodice riportato sotto)
	int n, newn;

	n = array_dimension;


	//do {
	while (n > 1) {

		newn = 0;

		for (int i = 1; i < n; i++) {
			if (array[i-1] > array[i]) {
				swap_int(&array[i-1], &array[i]);
				newn = i;
			}
		}

		n = newn;
	}

	//} while (n > 1);


/*

procedure bubbleSort(A : list of sortable items)
    n := length(A)
    repeat
        newn := 0
        for i := 1 to n - 1 inclusive do
            if A[i - 1] > A[i] then
                swap(A[i - 1], A[i])
                newn := i
            end if
        end for
        n := newn
    until n ≤ 1
end procedure
*/

}




void swap_wrong(int a, int b) {
	int temp;

	temp = a;
	a = b;
	b = temp;
}


void swap(int * a, int * b) {
	int temp;

	temp = *a;
	*a = *b;
	*b = temp;
}


int main(int argc, char * argv[]) {

	int a = 100;

	int b = 200;

	int * ptr;

	// &: restituisce l'indirizzo dell'oggetto in memoria
	ptr = &a;

	// * : operatore "indirezione" o "dereferencing"
	*ptr = 300; // quale variabile viene modificata?

	//

	ptr = &b;

	*ptr = 400; // quale variabile viene modificata?

	*ptr = *ptr + 10;

	//

	a = *ptr + 1;
	// quanto vale a?

	swap_wrong(a,b);

	swap(&a, &b);

	// potete usare il debugger per scoprire:

	// quanto vale a?

	// quanto vale b?


	return 0;
}

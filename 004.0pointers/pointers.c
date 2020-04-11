
// vedere anche:
// https://repl.it/@MarcoTessarotto/esempio-calloc-realloc-free

void swap_wrong(int a, int b) {
	int temp;

	temp = a;
	a = b;
	b = temp;
}


void swap(int * indirizzo_di_a, int * indirizzo_di_b) {
	int temp;

	temp = *indirizzo_di_a;
	*indirizzo_di_a = *indirizzo_di_b;
	*indirizzo_di_b = temp;

	//// ciò che segue è sbagliato:

//	int * temp;
//	temp = indirizzo_di_a;
//	indirizzo_di_a = indirizzo_di_b;
//	indirizzo_di_b= temp;

}


int main(int argc, char * argv[]) {

	int a = 100;

	int b = 200;

	int * ptr;

	// &: restituisce l'indirizzo dell'oggetto in memoria
	ptr = &a;

	// * : operatore "indirezione" o "dereferencing"
	*ptr = 300; // quale variabile viene modificata? a

	//

	ptr = &b;

	*ptr = 400; // quale variabile viene modificata? b

	*ptr = *ptr + 10; // il valore di b diventa 410

	//

	a = *ptr + 1;
	// quanto vale a? 411

	// a = 411, b = 410
	swap_wrong(a,b);

	// quale sarà il valore di a e b?

	swap(&a, &b);

	// potete usare il debugger per scoprire:

	// quanto vale a?

	// quanto vale b?


	return 0;
}

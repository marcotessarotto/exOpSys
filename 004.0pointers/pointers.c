

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

	ptr = &a;

	*ptr = 300;

	ptr = &b;

	*ptr = 400;

	//

	swap_wrong(a,b);

	swap(&a, &b);

	// usare il debugger per scoprire:

	// quanto vale a?

	// quanto vale b?


	return 0;
}

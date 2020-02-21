#include <stdio.h>




int main(int argc, char **argv) {

	char ch;
	int a, b;
	unsigned int ua, ub;
	long la, lb;

	float c;
	double d;

	char * str = "hello world!";
	char * numeri = "0123456789";

	ch = 'Z';
	a = 10;
	b = -20;

	ua = 0xFFFFFFFF;
	ub = 0;

	la = 1L << 32; // provare: 1 << 32

	c = 3.1492;

	d = 0.123456789;

	printf("scrivo un char: %c\n", ch);

	printf("scrivo int: a=%d b=%d\n", a, b);

	printf("scrivo unsigned int: ua=%u ub=%x \n", ua, ub);

	printf("scrivo long: la=%ld\n", la);

	printf("scrivo float: %f\n", c);

	printf("scrivo float: %1.3f\n", c);

	printf("scrivo double: %e\n", d);
	printf("scrivo double: %1.12f\n", d);

	printf("scrivo stringa di caratteri: %s\n", str);
	printf("scrivo stringa di caratteri: %s\n", numeri);


	printf("\n");

	printf("sizeof(char)=%ld\n", sizeof(char));

	printf("sizeof(short int)=%ld\n", sizeof(short int));
	printf("sizeof(int)=%ld\n", sizeof(int));

	printf("sizeof(long)=%ld\n", sizeof(long));
	printf("sizeof(long long)=%ld\n", sizeof(long long));


	printf("sizeof(float)=%ld\n", sizeof(float));
	printf("sizeof(double)=%ld\n", sizeof(double));
	printf("sizeof(long double)=%ld\n", sizeof(long double));

	printf("sizeof(char *)=%ld\n", sizeof(char *));


	/*

	printf("1 %d\n", 5 / 9); // cosa stamperebbe? attenzione che sto dividento due numeri interi

	printf("2 %f\n", 5 / 9); // cosa stamperebbe? printf si aspetta un valore float (floating point)

	printf("3 %f\n", 5 / 9.);

	printf("4 %f\n", 5. / 9);

	printf("5 %f\n", 5. / 9);

	printf("6 %d\n", 5. / 9);
	*/

	return 0;
}

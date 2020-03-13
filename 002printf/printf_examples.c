#include <stdio.h>
#include <string.h>
#include <limits.h>


int main(int argc, char **argv) {

	char ch;

	int a, b; // intero con segno; potrei usare il tipo dati 'signed int'
	unsigned int ua, ub; // intero senza segno

	long la, lb;
	unsigned long ula;

	float c;
	double d;

	char * str = "hello world!"; // stringhe di caratteri
	char * numeri = "0123456789";

	ch = 'Z'; // carattere ASCII 90: Z
	// ch = 90;

	// int (con segno)
	a = 1000000;
	b = -2000000;

	// unsigned int
	ua = 1234567890;
	ub = 4321;
	// ub = -4321; // e questo?

	// long
	la = 1L << 32;
	lb = 0x123456789abcdef0L;

	// unsigned long
	ula = 1234567890000L;

	c = 3.1492;

	d = 0.123456789;


	// in Unix (e Linux) ogni processo, quando viene avviato, ha tre file descriptors (descrittori di file) pronti per l'uso:

	// standard input (0, stdin) da cui leggere
	// standard output (1, stdout) su cui scrivere
	// standard error (2, stderr) su cui scrivere messaggi di errore o diagnostici

	// tipicamente sono "attaccati" al terminale da cui viene avviato il processo
	// quindi: input da tastiera, output (1 e 2) su schermo


	// **** char ****
	printf("(1)scrivo un char: %c", ch);
	printf("\n");
	// oppure:
	printf("(2)scrivo un char: ");
	putchar(ch); // putchar('Z');
	printf("\n\n");

	printf("il valore numerico di '%c' è %d ", ch, ch); // capirlo bene...
	printf("\n\n");

	// **** int ****

	printf("scrivo un int: %d", a);
	printf("\n");

	printf("scrivo due int: a=%d b=%d", a, b);
	printf("\n\n");

	// **** unsigned int ****

	printf("scrivo due unsigned int: ua=%u ub=%x", ua, ub);
	printf("\n\n");

	// **** long ****

	printf("scrivo long: la=%ld lb=%lx", la, lb);
	printf("\n\n");

	// **** unsigned long ****

	printf("scrivo unsigned long: ula=%lu", ula);
	printf("\n\n");

	// **** float ****

	printf("scrivo float: %f\n", c);
	printf("scrivo float: %1.3f\n", c);
	printf("scrivo float con esponente: %e", c);
	printf("\n\n");

	// **** double ****

	printf("scrivo double: %e\n", d);
	printf("scrivo double: %1.12f", d);
	printf("\n\n");

	// stringhe di caratteri

	printf("scrivo stringa di caratteri: %s\n", str);
	printf("scrivo stringa di caratteri: %s\n", numeri);


	printf("\n");


	// vedi progetto 002data-types
	printf("sizeof(short int) = %ld bytes\n", sizeof(short int));
	printf("sizeof(int) = %ld bytes\n", sizeof(int));

	printf("sizeof(long) = %ld bytes\n", sizeof(long));
	printf("sizeof(unsigned long) = %ld bytes\n", sizeof(unsigned long));
	printf("sizeof(long long) = %ld bytes\n", sizeof(long long));
	printf("sizeof(unsigned long long) = %ld bytes\n", sizeof(unsigned long long));


	printf("sizeof(float) = %ld bytes\n", sizeof(float));
	printf("sizeof(double) = %ld bytes\n", sizeof(double));
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

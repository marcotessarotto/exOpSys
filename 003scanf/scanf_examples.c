#include <stdio.h>

/*

da https://en.wikipedia.org/wiki/Scanf_format_string

    %a : Scan a floating-point number in its hexadecimal notation.
    %d : Scan an integer as a signed decimal number.
    %i : Scan an integer as a signed number. Similar to %d,
         but interprets the number as hexadecimal when preceded by 0x and octal
         when preceded by 0. For example, the string 031 would be read
         as 31 using %d, and 25 using %i. The flag h in %hi indicates conversion
         to a short and hh conversion to a char.
    %u : Scan for decimal unsigned int (Note that in the C99 standard the input value minus sign is optional, so if a minus sign is read, no errors will arise and the result will be the two's complement of a negative number, likely a very large value. See strtoul().[failed verification])
    Correspondingly, %hu scans for an unsigned short and %hhu for an unsigned char.

    %f : Scan a floating-point number in normal (fixed-point) notation.
    %g, %G : Scan a floating-point number in either normal or exponential notation. %g uses lower-case letters and %G uses upper-case.

    %x, %X : Scan an integer as an unsigned hexadecimal number.
    %o : Scan an integer as an octal number.

    %s : Scan a character string. The scan terminates at whitespace. A null character is stored at the end of the string, which means that the buffer supplied must be at least one character longer than the specified input length.
    %c : Scan a character (char). No null character is added.

    whitespace: Any whitespace characters trigger a scan for zero or more whitespace characters. The number and type of whitespace characters do not need to match in either direction.
    %lf : Scan as a double floating-point number. "Float" format with the "long" specifier.
    %Lf : Scan as a long double floating-point number. "Float" format the "long long" specifier.
*/

void altri_esempi_scanf() {

	char ch;
	int a, b;
	unsigned int ua, ub;
	long la, lb;

	float c;
	double d;

	printf("scrivi un char: ");
	scanf("%c", &ch);

	// ch = getchar();

	printf("ho letto il seguente char: %c\n", ch);

	printf("scrivi un int: ");
	scanf("%d", &a);
	printf("ho letto il seguente int: %d\n", a);

	printf("scrivi un unsigned int: ");
	scanf("%u", &ub);
	printf("ho letto il seguente unsigned int: %u\n", ub);

	printf("scrivi un int in base 16: ");
	scanf("%x", &a);
	printf("ho letto il seguente int: %x\n", a);

}


int main(int argc, char **argv) {

	int a,b;
	int sum;
	int res;

	if (1) {

		altri_esempi_scanf();

		return 0;
	}

	if (0) {

		printf("scrivi due numeri interi: ");

		res = scanf("%d %d", &a, &b);

		printf("scanf ha restituito: %d (numero di valori letti)\n", res);

		if (res == EOF) {
			printf("EOF! bye!");
			return 1;
		}

		sum = a + b;

		printf("sum = %d\n", sum);

	}

	/*
	 * esercizio da fare:
	 *
	 * chiedere all'utente tre input:
	 * l'operazione da fare (un char tra: + - /) e due numeri
	 * eseguire l'operazione richiesta e scrivere il risultato
	 *
	 * esempio:
	 *
	 * input:
	 * + 10 20
	 *
	 * risultato:
	 * 30
	 *
	 */

	return 0;
}

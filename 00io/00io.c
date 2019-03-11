#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE 10

void fflush_stdin();


void read_input_unsafe() {

	char buffer[STR_SIZE+1]; // 80 caratteri + \0 finale

	printf("input (max 10 caratteri) : ");

	// PROBLEMA di SICUREZZA:
	// attenzione!! rischio OVERFLOW (andare oltre il limite di 80 caratteri), scanf non sa che esiste il limite...
	scanf("%s", buffer); // IMPORTANTE: passo l'indirizzo del buffer; equivalente a &(buffer[0])

	printf("1 hai scritto:%s len=%li\n", buffer, strlen(buffer));

}


void read_input_safe() {

	char buffer[STR_SIZE+1]; // 80 caratteri + \0 finale

	printf("input (max 10 caratteri) : ");

	scanf("%10s", buffer); // <<<====== OK, scanf "sa" che può restituire al max 80 caratteri

	printf("2 hai scritto:%s len=%li\n", buffer, strlen(buffer));

}


// direttive per il preprocessore
#define STR2(a) #a
#define STR(a) STR2(a)

// più complesso perchè utilizziamo il preprocessore in modo "avanzato"
void read_input_safe_macro() {

	char buffer[STR_SIZE+1]; // 80 caratteri + \0 finale

	// questa serve soltanto per capire cosa succede più sotto...
	char * test_string = "questa "
					  " è "
					  " una stringa"
					  "unica!"; // 4 costanti stringhe concatenate

	printf("input (max " STR(STR_SIZE) " caratteri) : ");

	fflush_stdin(); // svuotiamo stdin, se l'utente in read_input_safe ha inputato più di 10 caratteri, ce li ritroviamo al prossimo scanf

	scanf("%" STR(STR_SIZE) "s", buffer); // <<<====== OK, scanf "sa" che può restituire al max 80 caratteri

	printf("3 hai scritto:%s len=%li\n", buffer, strlen(buffer));

}

void fflush_stdin()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}


int main(int argc, char * argv[]) {

	read_input_unsafe();

	read_input_safe();

	read_input_safe_macro();



	printf("\nesempio di lettura valori da stringa con sscanf\n");

	char * sample_input_string = "1,100.1,3.14,0xFF24,A";


	int a;
	float b;
	double c;
	int d;
	char e;

	sscanf(sample_input_string, "%d,%f,%lf,%x,%c", &a, &b, &c, &d, &e);

	printf("risultato: a=%d b=%f c=%lf d=%x e=%c\n", a, b, c, d, e);

	printf("bye!\n");

	return 0;
}

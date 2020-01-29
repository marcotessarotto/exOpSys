#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "read_samples.h"
#include "strtoi_samples.h"

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

//	read_input_unsafe();
//
//	read_input_safe();
//
//	read_input_safe_macro();



	printf("\nesempio di lettura valori da stringa con sscanf\n");

	char * sample_input_string = "1,100.1,3.14,0xFF24,A";

	printf("input string: %s\n", sample_input_string);


	int a;
	float b;
	double c;
	int d;
	char e;

	int numero_variabili_lette = sscanf(sample_input_string, "%d,%f,%lf,%x,%c", &a, &b, &c, &d, &e);

	printf("ho letto %d variabili\n", numero_variabili_lette);

	printf("risultato: a=%d b=%f c=%lf d=%x e=%c\n", a, b, c, d, e);


	///////////////


//	char *ptr="fruit|apple|lemon",type[10],fruit1[10],fruit2[10];
//
//	sscanf(ptr, "%[^|]%*c%[^|]%*c%s", type, fruit1, fruit2);
//	printf("%s,%s,%s\n", type, fruit1, fruit2);


	///////////////


	printf("\nesempio di lettura valori da stringa con sscanf(2)\n");

	sample_input_string = "\"nome\",\"cognome\",\"123456789\",\"1234.56789\"";

	printf("input string: %s\n", sample_input_string);

	char nome[64] = {};
	char cognome[64] = {};
	long numero_intero = 0;
	double numero_double = 0.0;

	printf("così scanf NON funziona bene....\n");
	// NON riesce a leggere.... legge tutto come una unica stringa di caratteri
	numero_variabili_lette = sscanf(sample_input_string, "\"%64s\",\"%64s\",\"%ld\",\"%lf\"",nome, cognome, &numero_intero, &numero_double);

	printf("ho letto %d variabili\n", numero_variabili_lette);

	printf("risultato: nome=%s, len(nome)=%ld, cognome=%s, len(cognome)=%ld, numero_intero=%ld, numero_double=%lf\n",
			nome, strlen(nome), cognome, strlen(cognome), numero_intero, numero_double);


	printf("\ncosì scanf funziona bene!\n");

	numero_variabili_lette = sscanf(sample_input_string, "\"%64[^\"]\",\"%64[^\"]\",\"%ld\",\"%lf\"",nome, cognome, &numero_intero, &numero_double);
	/*
	 * \"%[^\"]\"  ->    \"   %s (tutti i caratteri diversi da \")   \"
	 */

	printf("ho letto %d variabili\n", numero_variabili_lette);

	printf("risultato: nome=%s, len(nome)=%ld, cognome=%s, len(cognome)=%ld, numero_intero=%ld, numero_double=%lf\n\n",
			nome, strlen(nome), cognome, strlen(cognome), numero_intero, numero_double);


	/////
	// esempio dove ' sostituisce \"

	sample_input_string = "'nome2','cognome2','987654321','9876.54321'";

	printf("altro esempio con scanf\n");

	printf("input string: %s\n", sample_input_string);

	numero_variabili_lette = sscanf(sample_input_string, "'%64[^']','%64[^']','%ld','%lf'",nome, cognome, &numero_intero, &numero_double);

	printf("ho letto %d variabili\n", numero_variabili_lette);

	printf("risultato: nome=%s, len(nome)=%ld, cognome=%s, len(cognome)=%ld, numero_intero=%ld, numero_double=%lf\n",
				nome, strlen(nome), cognome, strlen(cognome), numero_intero, numero_double);

	// prove con puntatori

	printf("\nesempio con puntatori\n");

	char str[100];

	char * ptr;

	printf("sizeof(char *) = %ld\n", sizeof(char *));
	printf("sizeof(char [100]) = %ld\n", sizeof(char [100]));
	printf("sizeof(char * [100]) = %ld\n", sizeof(char * [100]));
	printf("sizeof(char (*) [100]) = %ld\n", sizeof(char (*) [100]));
	printf("\n");

	ptr = str; //  conversione da char [] a char *
	printf("str = %p\n", ptr); // puntatore a char *

	ptr = &str; // puntatore a 'char [100]'
	printf("&str = %p\n",ptr); //

	ptr = &str[0]; // indirizzo di str[0]
	printf("&str[0] = %p\n", ptr);

	ptr = &str[1]; // indirizzo di str[1]
	printf("&str[1] = %p\n", ptr);

//	scanf("%s", str);
//    printf("Entered string is %s \n", str);


//	read_2_ints();

	test_strlen();


	//
	strtoi_samples();


	printf("\nbye!\n");

	return 0;
}

#include <stdio.h>    // <== direttive del preprocessore
#include <stdlib.h>   // i simboli < > dicono al preprocessore di cercare i file AL DI FUORI della cartella di progetto
#include <string.h>


// di seguito, troviamo delle DEFINIZIONI di funzioni

void // void: la funzione non restituisce nulla
hello_world(void) {  // void: la funzione non ha nessun parametro

	printf("hello world!\n"); // <= \n è un carattere speciale: fa andare a capo il "cursore"

	printf("\\ si chiama \"backslash\" ovvero \"Escape Character\". \n");

	printf("\\n è un carattere singolo che ha codice ASCII 10, viene chiamato \"line feed\" o \"new line\"\n");

	printf("\\n è un carattere speciale: si dice che è una Escape Sequence; è come premere Enter in un editor...vai a capo e si sposta a sinistra.\n");

	printf("ad esempio, un altro carattere speciale è \\t (tab): 123\t456 , introduce un certo numero di spazi.\n");

	printf("ASCII chart:\nhttps://en.cppreference.com/w/cpp/language/ascii\n");

	printf("\n\n");
}


void hello_world_string_multi_line() {

	// 4 stringhe adiacenti, sulla stessa riga => il compilatore le concatena in una unica stringa (compile time)
	printf("primo" " hello " "world!" "\n");

	// 4 stringhe di caratteri adiacenti, sono automaticamente concatenate dal compilatore:
	printf("secondo"
			" hello "
			"world!"
			"\n");

	printf("\n\n");
}

void hello_world_multi_printf() {

	printf("3: hello ");
	printf("world!");
	printf("\n");

	printf("\n\n");
}

// NOTA BENE: se scrivo la funzione così, vuol dire che, implicitamente, restituisce int
hello_world_printf() {

	// notare: \n ha codice numerico 10 ovvero in base esadecimale: 0xa

	printf("4: Hello,%cworld!", 0x0a); // %c viene sostituito dal carattere di valore 0xa, ovvero \n

	printf("\n\n");
}


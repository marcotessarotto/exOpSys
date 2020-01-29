#include <stdio.h>    // <== direttive del preprocessore
// i simboli < > dicono al preprocessore di cercare i file AL DI FUORI della cartella di progetto

#include "my_header.h"

int main(int argc, char **argv) {

	// printf è una funzione di GLIBC per produrre output formattato
	printf("ciao!\n");

	/*
	 * aspetti importanti:
	 *
	 * main
	 * stdin, stdout
	 * int
	 * stringhe di caratteri
	 * carattere \n
	 * argc, argv
	 * #include
	 * return
	 * preprocessore, gcc e linker
	 * glibc
	 * costanti stringa
	 * commenti linea singola, multilinea
	 * gcc warning, error
	 * compile time
	 * run time
	 * linguaggio C non è object oriented
	 *
	 * ls -l
	 * ldd
	 * Makefile
	 *
	 */

	// caratteri_ascii(); // provare in eclipse IDE: ctrl + click_tasto_sx_mouse su simbolo
	// hello_world_string_multi_line();
	// hello_world_multi_printf();


	return 0;
}

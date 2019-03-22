#ifndef CHAR_IO_H_
#define CHAR_IO_H_

// stream ("flusso") di caratteri: sequenza di caratteri
// TEXT stream: sequenza di caratteri divisa in linee;
// ogni linea si compone di zero o più caratteri seguita dal carattere "new line" (\n)

/*
 * c = getchar()    legge il prossimo carattere dall'input (stdin, di solito è la tastiera)
 *
 * putchar(c)  stampa (aggiunge) un carattere nell'output (di solito è lo schermo)
 *
 * getchar e putchar sono essere inframmezzate tra loro e con le altre funzioni;
 *
 * posso alternare tranquillamente putchar e printf.
 *
 */

void copy_input_to_output();

void count_lines_in_input();

void word_count();

void count_to_array();


struct INNER_ARRAY {
	char data[80];
};


struct INNER_ARRAY return_a_struct();

void pass_a_struct(struct INNER_ARRAY param);


void pass_a_struct_ptr(struct INNER_ARRAY * param);

#endif /* CHAR_IO_H_ */

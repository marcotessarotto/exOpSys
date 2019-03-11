
#include "array_list.h"

// crea ed inizializza un nuovo array list
struct ARRAY_LIST  * create_new_array01() {
	// usare malloc o calloc


}

// aggiunge all’array list un nuovo item, la cui chiave è id ed il valore è data
void put_item02(struct ARRAY_LIST * array, long id, void * data) {

	// controlla se la chiave id è già presente; se sì, sovrascrive il valore vecchio con il nuovo

	// se non presente, accoda il nuovo nodo alla linked list

}

// cerca e resituisce il valore associato alla chiave id; restituisce NULL se non presente
void * get_item03(struct ARRAY_LIST * array, long id) {
}

// rimuove, se presente, l’item individuato dalla chiave id; restituisce il valore che era associato alla chiave id (se era presente), altrimenti restituisce NULL
void * remove_item04(struct ARRAY_LIST * array, long id) {

	// ricerca il nodo da cancellare;
	// al primo nodo con uguale id, free del nodo MA il valore viene restituito così come sta (ci pensa il chiamante a fare free del valore

}

// free dell’intero array list, per ogni value da distruggere chiama free_fun
void destroy_array05(struct ARRAY_LIST  * array, void (*free_fun)(void *)) {
	// eliminare un item per volta; per eliminare il singolo value, invocare il metodo free_fun:
	/*
	 * void * value_cancellare = ....
	 *
	 * free_fun(value_cancellare);
	 *
	 */


}

// cerca il valore data nell’array e restituisce la chiave corrispondente; restituisce -1 se non trovata; value_cmp è il comparatore di valori
long find_value06(struct ARRAY_LIST * array, void * value, int (*value_cmp)(void *, void*)) {
	// scorrere tutta la linked list alla ricerca del valore (attenzione! non l'id) ricercato
	// usare value_cmp per confrontare value, in questo modo:
	/*
	 * void * value_da_cercare;
	 * void * value_nodo_corrente = ....
	 *
	 * int confronto = value_cmp(value_da_cercare, value_nodo_corrente);
	 *
	 * if (confronto == 0) ===>>>> i due dati sono uguali!!! trovato
	 * else ..... =====>>> i due dati sono diversi....
	 *
	 */

}

// enumera tutto il contenuto dell’array, chiama fun_ptr per ogni item; fun_ptr restituisce 0 per terminare l’enumerazione
void enumerate_values07(int (*enum_fun)(long, void *)) {
	// scorrere tutta la linked list;
	// per ogni item, invocare fun_ptr:

	// long id_corrente = .....
	// void * value_corrente = .....
	// int res = enum_fun(id, value_corrente); // se restituisce 0, vuol dire che dobbiamo terminare l'enumerazione
	// if (!res) return;
}

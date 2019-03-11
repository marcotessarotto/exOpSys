#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_

typedef struct ARRAY_ITEM_STRUCT {
   long id;   // chiave; id deve essere >= 0
   void * data; // valore

   struct ARRAY_ITEM_STRUCT * next; // ARRAY_ITEM è la struttura dati di una linked list
} ARRAY_ITEM;

typedef struct ARRAY_LIST_STRUCT {

  ARRAY_ITEM * head;

  ARRAY_ITEM * tail;

  long size; // da tenere sempre aggiornato
} ARRAY_LIST;

ARRAY_LIST  * create_new_array01(); // crea ed inizializza un nuovo array list
void put_item02(ARRAY_LIST * array, long id, void * data); // aggiunge all’array list un nuovo item, la cui chiave è id ed il valore è data; se id era già presente, il valore è sovrascitto dal nuovo
void * get_item03(ARRAY_LIST * array, long id); // cerca e resituisce il valore associato alla chiave id; restituisce NULL se non presente
void * remove_item04(ARRAY_LIST * array, long id); // rimuove, se presente, l’item individuato dalla chiave id; restituisce il valore che era associato alla chiave id (se era presente), altrimenti restituisce NULL
void destroy_array05(ARRAY_LIST  * array, void (*free_fun)(void *)); // free dell’intero array list, per ogni value da distruggere chiama free_fun
long find_value06(ARRAY_LIST * array, void * value, int (*value_cmp)(void *, void*)); // cerca il valore data nell’array e restituisce la chiave corrispondente; restituisce -1 se non trovata; value_cmp è il comparatore di valori
void enumerate_values07(ARRAY_LIST * array, int (*enum_fun)(long, void *)); // enumera tutto il contenuto dell’array, chiama fun_ptr per ogni item; fun_ptr restituisce 0 per fermare l’enumerazione


#endif /* ARRAY_LIST_H_ */

#include <stdio.h>
#include <string.h>

// per semplificare la lettura del codice, introduciamo string_t
typedef char * string_t;
// per convenzione, nella C Standard Library i tipi dati dichiarati con typedef hanno il suffisso "_t"

// potrei usare in alternativa:
typedef char * char_ptr_t;

// int main(int argc, char * argv[]) {
// int main(int argc, char ** argv) {
int main(int argc, string_t argv[]) {

  // argc è >= 1 ovvero esiste sempre argv[0]
  // argv[0] è il nome del programma
  // argv è un array di (stringhe di char)
  // stringa di caratteri: array di char terminato da '\0'

  for (int i = 0; i < argc; i++) {
    printf("argv[%d] = %s\n", i, argv[i]);
  }

  if (argc > 1) {
     if (!strcmp(argv[1],"ciao")) {
       printf("ciao!\n");
     }
  }


  // provare da shell:
  // ./012printargs passo dei parametri

  // ./012printargs "passo dei parametri"

  // ln 012printargs prova
  // ./prova

  return 0;
}

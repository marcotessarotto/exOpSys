#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>




int main(int argc, char * argv[]) {

/*

                    *FILE OFFSET*

Per ogni file aperto, il kernel mantiene un "file offset",
talvolta chiamato anche "read-write offset" o "puntatore".

Questa è la posizione nel file in cui inizierà il prossimo read() o write().
L'offset del file è espresso come una posizione di byte rispetto all'inizio del file.
Il primo byte del file è all'offset 0.

L'offset del file viene impostato in modo che punti all'inizio del file
quando viene aperto il file e viene automaticamente
aggiustato da ogni chiamata successiva read() o write()
in modo che punti, nel file, al byte successivo
a quello appena letto o scritto.

Pertanto, chiamate successive a read() e write() percorrono il file
in modo sequenziale (nella direzione che va dall'inizio del file verso la fine dello stesso).

*/

	// TODO

	// apro un file già esistente in modalità lettura/scrittura
	// ad esempio, possiamo lavorare su un file prodotto dall'esempio 005.8writefile

	// all'apertura del file, il file offset vale 0

	// spostiamo il file offset alla posizione 256 (dall'inizio del file)

	// libro Kerrisk pag. 81

	// leggiamo una parte

	// poi spostiamo il file offset rispetto alla fine del file

	// ...



	return 0;
}

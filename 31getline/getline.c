#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// vedere anche man 3 getline

#define CHUNK_SIZE 3

// legge da fd una linea intera (fino a che incontra carattere newline oppure EOF oppure errore in letura da fd)
// newline è incluso nella stringa di caratteri restituita; la stringa è terminata da \0
// scrive in *buffer l'indirizzo del buffer allocato
// scrive in *n la dimensione del buffer; restituisce n
int my_getline(int fd, char ** buffer, size_t * n) {

	char * buf = calloc(CHUNK_SIZE, sizeof(char));
	size_t size = CHUNK_SIZE;
	int pos = 0;
	char ch;
	int res;

	if (buffer == NULL) {
		return -1;
	}

	while ((res = read(fd, &ch, 1)) == 1) { // read restituisce 0 in caso di EOF, -1 se errore

		if (pos == size) {
			size += CHUNK_SIZE;
			buf = realloc(buf, size);
		}

		buf[pos++] = ch;

		if (ch == '\n') {
			break;
		}
	}

	if (res == -1) {
		perror("read");
	}

	if (pos == size) {
		size += 1;
		buf = realloc(buf, size);
	}

	buf[pos++] = '\0'; // end of string

	if (pos < size)
		buf = realloc(buf, pos);

	*buffer = buf;
	if (n != NULL)
		*n = pos;

	return pos;
}


int main(int argc, char * argv[]) {

	char * buf;
	size_t n;

	printf("scrivi qualcosa:\n");

	my_getline(STDIN_FILENO, &buf, &n);

	printf("dimensione buffer=%ld, strlen=%ld, data=*%s*\n", n, strlen(buf), buf);

	free(buf);

	return 0;
}

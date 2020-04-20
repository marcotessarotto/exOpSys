#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>



int main(int argc, char *argv[]) {

	char * file_name;
	char * text_to_write;
	int text_to_write_len;


	if (argc == 1) {
		printf("specificare come parametro il nome del file da creare e su cui scrivere\n");
		exit(EXIT_FAILURE);
	}

	file_name = argv[1];
	printf("scrivo nel file %s\n", file_name);

	// apriamo il file in scrittura, se non esiste verrà creato, se esiste già la sua dimensione viene troncata a 0

	// tratto da man 2 open
	// O_CREAT  If pathname does not exist, create it as a regular file.
	// O_TRUNC  If the file already exists and is a regular file and the access mode allows writing ... it will be truncated to length 0.
	// O_RDONLY, O_WRONLY, or O_RDWR  These request opening the file read-only, write-only, or read/write, respectively.

	int fd = open(file_name,
				  O_CREAT | O_TRUNC | O_WRONLY,
				  S_IRUSR | S_IWUSR // l'utente proprietario del file avrà i permessi di lettura e scrittura sul nuovo file
				 ); // O_RDONLY: apri il file in sola lettura

	// l'utente proprietario del file sarà lo stesso utente che ha eseguito questo programma

	/*
	S_IRWXU  00700 user (file owner) has read, write, and execute permission

	S_IRUSR  00400 user has read permission

	S_IWUSR  00200 user has write permission

	S_IXUSR  00100 user has execute permission
	 */

	if (fd == -1) { // errore!

		perror("open()");

		exit(EXIT_FAILURE);
	}

	// write something
	// https://loremipsum.io/generator/?n=5&t=p
	text_to_write = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. At ultrices mi tempus imperdiet nulla malesuada pellentesque elit. Non enim praesent elementum facilisis leo vel fringilla est ullamcorper. Non quam lacus suspendisse faucibus interdum. Metus vulputate eu scelerisque felis imperdiet. At ultrices mi tempus imperdiet nulla malesuada pellentesque. At tempor commodo ullamcorper a lacus vestibulum. Consequat semper viverra nam libero justo laoreet sit amet. Facilisis magna etiam tempor orci eu. Convallis tellus id interdum velit laoreet id donec ultrices. Interdum velit euismod in pellentesque massa placerat duis ultricies. Senectus et netus et malesuada fames. Pharetra vel turpis nunc eget lorem dolor. Nisi porta lorem mollis aliquam ut porttitor leo a. Euismod elementum nisi quis eleifend quam adipiscing vitae proin.";

	text_to_write_len = strlen(text_to_write) + 1; // consideriamo anche lo '\0' finale

	printf("scrivo %d bytes\n", text_to_write_len);

	int res = write(fd, text_to_write, text_to_write_len);

	if (res == -1) {
		perror("write()");
		exit(EXIT_FAILURE);
	}

	// close file

	if (close(fd) == -1) {
		perror("close");
		exit(EXIT_FAILURE);
	}

	printf("bye!\n");

	exit(EXIT_SUCCESS);
}

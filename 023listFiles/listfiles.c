#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>


// https://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html

int main(int argc, char *argv[]) {

	DIR * dp;
	struct dirent *ep;

	char * fileName;

//	printf("sizeof unsigned char %i\n", sizeof(unsigned char));
//	printf("sizeof char %i\n", sizeof(char));
//	printf("sizeof short int %i\n", sizeof(short int));
//
//	printf("sizeof int %i\n", sizeof(int));
//	printf("sizeof unsigned long int %i\n", sizeof(unsigned long int));
//	printf("sizeof unsigned long long int %i\n", sizeof(unsigned long long int));

	fileName = (argc == 1) ? "/home/utente" : argv[1];

	dp = opendir(fileName);  //   ./

	if (dp == NULL) {

//		perror("cannot open directory! bye");

		printf("cannot open directory %s! bye", fileName);

		return -1;
	}

	while (ep = readdir(dp)) {
		puts(ep->d_name);
		// file size?? costruire path a file ed usare stat (man 2 stat)
	}

	closedir(dp);

	puts("finished! bye!");

	return EXIT_SUCCESS;
}

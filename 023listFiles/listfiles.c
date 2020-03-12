#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

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

	fileName = (argc == 1) ? "." : argv[1];

	errno = 0;

	dp = opendir(fileName);  //   ./

	if (dp == NULL) {

//		perror("cannot open directory! bye");

		printf("cannot open directory %s! bye", fileName);

		return EXIT_FAILURE;
	}



	while ((ep = readdir(dp)) != NULL) {
		//puts(ep->d_name);

		printf("%-10s ", (ep->d_type == DT_REG) ?  "regular" :
		                                    (ep->d_type == DT_DIR) ?  "directory" :
		                                    (ep->d_type == DT_FIFO) ? "FIFO" :
		                                    (ep->d_type == DT_SOCK) ? "socket" :
		                                    (ep->d_type == DT_LNK) ?  "symlink" :
		                                    (ep->d_type == DT_BLK) ?  "block dev" :
		                                    (ep->d_type == DT_CHR) ?  "char dev" : "???");

		printf("%s \n", ep->d_name);

		// file size?? costruire path a file ed usare stat (man 2 stat)
	}

	if (errno) {
		perror("readdir() error");
	}

	closedir(dp);

	puts("finished! bye!");

	return EXIT_SUCCESS;
}

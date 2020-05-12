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


	fileName = (argc == 1) ? "." : argv[1];

	errno = 0;

// man 3 opendir
//    DIR *opendir(const char *name);
//    DIR *fdopendir(int fd);


	dp = opendir(fileName);

	if (dp == NULL) {
		printf("cannot open directory %s! bye", fileName);

		return EXIT_FAILURE;
	}



	while ((ep = readdir(dp)) != NULL) {

		printf("%-10s ", (ep->d_type == DT_REG) ?  "regular" :
		                                    (ep->d_type == DT_DIR) ?  "directory" :
		                                    (ep->d_type == DT_FIFO) ? "FIFO" :
		                                    (ep->d_type == DT_SOCK) ? "socket" :
		                                    (ep->d_type == DT_LNK) ?  "symlink" :
		                                    (ep->d_type == DT_BLK) ?  "block dev" :
		                                    (ep->d_type == DT_CHR) ?  "char dev" : "???");

		printf("%s \n", ep->d_name);

		// come trovo il file size? posso usare stat (man 2 stat)
	}

	if (errno) {
		perror("readdir() error");
	}

	closedir(dp);

	puts("finished! bye!");

	return EXIT_SUCCESS;
}

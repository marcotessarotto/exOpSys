#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

// https://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html

int main(int argc, char *argv[]) {

	DIR * dir_stream_ptr;
	struct dirent * item;
	char * directory_name;
	char * absolute_path;
	int fd;

	directory_name = (argc == 1) ? "." : argv[1];

	errno = 0;

	absolute_path = realpath(directory_name, NULL);
	if (absolute_path == NULL) {
		perror("realpath");
		exit(1);
	}

	printf("realpath(\"%s\") = \"%s\"\n\n", directory_name, absolute_path);

	free(absolute_path);

	fd = open(directory_name, O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(1);
	}

// man 3 opendir
//    DIR *opendir(const char *name);
//    DIR *fdopendir(int fd);

	//dir_stream_ptr = opendir(directory_name);
	dir_stream_ptr = fdopendir(fd);

	if (dir_stream_ptr == NULL) {
		printf("cannot open directory %s! bye", directory_name);

		return EXIT_FAILURE;
	}

	int directory_counter = 0;
	int regular_file_counter = 0;
	struct stat statbuf;

	while (( item = readdir(dir_stream_ptr)) != NULL) {

		printf("type=%-10s ", ( item->d_type == DT_REG) ?  "regular" :
		                                    ( item->d_type == DT_DIR) ?  "directory" :
		                                    ( item->d_type == DT_FIFO) ? "FIFO" :
		                                    ( item->d_type == DT_SOCK) ? "socket" :
		                                    ( item->d_type == DT_LNK) ?  "symlink" :
		                                    ( item->d_type == DT_BLK) ?  "block dev" :
		                                    ( item->d_type == DT_CHR) ?  "char dev" : "???");


		printf("name=%12s ",  item->d_name);

		if (item->d_type == DT_DIR) {
			directory_counter++;
			//printf("è una directory\n");
		} else if (item->d_type == DT_REG) {
			regular_file_counter++;

			if (fstatat(fd, item->d_name, &statbuf, 0) == -1) {
				perror("fstatat");
			} else {
				printf("  file size: %lu bytes\n", statbuf.st_size);
			}
		}

		printf("\n");

		// come trovo il file size? posso usare stat (man 2 stat)
	}

	if (errno) {
		perror("readdir() error");
	}

	closedir(dir_stream_ptr);

	printf("directory_counter=%d\n", directory_counter);
	printf("regular_file_counter=%d\n", regular_file_counter);

	printf("\nfinished! bye!\n");

	return EXIT_SUCCESS;
}

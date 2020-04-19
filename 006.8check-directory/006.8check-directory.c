#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/sysmacros.h>


// is file a directory?
// this example provides also much more information about files

/*
 * returns:
 * 1: file is a directory
 * 0: file is not a directory
 * -1: error
 */
int is_file_a_directory(char * file_name) {

	struct stat sb;
	int res;

	res = stat(file_name, &sb);

	if (res == -1) {
		perror("stat()");
		return -1;
	}

	return (sb.st_mode & S_IFMT) == S_IFDIR;
}

int is_fd_a_directory(int fd) {

	struct stat sb;
	int res;

	res = fstat(fd, &sb);

	if (res == -1) {
		perror("fstat()");
		return -1;
	}

	return (sb.st_mode & S_IFMT) == S_IFDIR;
}



int main(int argc, char * argv[]) {

	struct stat sb;
	int res;

	if (argc == 1) {
		printf("arguments: provide name of files to check\n");
		exit(EXIT_FAILURE);
	}


	for (int i = 1; i < argc; i++) {

		res = stat(argv[i], &sb);

		if (res == -1) {
			perror("stat()");
			continue;
		}

        printf("file name: %s\nfile type: ", argv[i]);

        switch (sb.st_mode & S_IFMT) {
			case S_IFBLK:  printf("block device\n");            break;
			case S_IFCHR:  printf("character device\n");        break;
			case S_IFDIR:  printf("directory\n");               break;
			case S_IFIFO:  printf("FIFO/pipe\n");               break;
			case S_IFLNK:  printf("symlink\n");                 break;
			case S_IFREG:  printf("regular file\n");            break;
			case S_IFSOCK: printf("socket\n");                  break;
			default:       printf("unknown?\n");                break;
        }

        printf("Mode:                     %lo (octal)\n",
                (unsigned long) sb.st_mode);

        printf("Link count:               %ld\n", (long) sb.st_nlink);

        printf("Ownership:                UID=%ld   GID=%ld\n",
                (long) sb.st_uid, (long) sb.st_gid);

        printf("File size:                %lld bytes\n",
                (long long) sb.st_size);


        printf("Last status change:       %s", ctime(&sb.st_ctime));
        printf("Last file access:         %s", ctime(&sb.st_atime));
        printf("Last file modification:   %s", ctime(&sb.st_mtime));


	}



	return 0;
}

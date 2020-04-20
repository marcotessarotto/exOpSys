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
	int is_directory = 0;

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
			case S_IFDIR:  printf("directory\n"); is_directory = 1;   break;
			case S_IFIFO:  printf("FIFO/pipe\n");               break;
			case S_IFLNK:  printf("symlink\n");                 break;
			case S_IFREG:  printf("regular file\n");            break;
			case S_IFSOCK: printf("socket\n");                  break;
			default:       printf("unknown?\n");                break;
        }


        printf("File size: %lld bytes\n",
                (long long) sb.st_size);


        printf("file owner: user ID (UID)=%ld   group ID (GID)=%ld\n",
                (long) sb.st_uid, (long) sb.st_gid);


        printf("\nMode: %lo (octal)\n",
                (unsigned long) sb.st_mode);

        //
        if (is_directory) {

        	// l'operatore condizionale ?: è un operatore ternario corrispondente alla struttura di controllo "if-then-else"
        	// https://repl.it/@MarcoTessarotto/operatore-condizionale

        	printf(" directory owner has %sREAD permission\n", sb.st_mode & S_IRUSR ? "" : "NO ");

        	printf(" directory owner has %sWRITE permission\n", sb.st_mode & S_IWUSR ? "" : "NO ");

        	printf(" directory owner has %sSEARCH permission\n", sb.st_mode & S_IXUSR ? "" : "NO ");

//        	if (sb.st_mode & S_IRUSR) {
//				printf(" directory owner has READ permission\n");
//			} else {
//				printf(" directory owner has NO READ permission\n");
//			}
//
//			if (sb.st_mode & S_IWUSR) {
//				printf(" directory owner has WRITE permission\n");
//			} else {
//				printf(" directory owner has NO WRITE permission\n");
//			}
//			if (sb.st_mode & S_IXUSR) {
//				printf(" directory owner has SEARCH permission\n");
//			} else {
//				printf(" directory owner has NO SEARCH permission\n");
//			}

			if (sb.st_mode & S_IRGRP) {
				printf(" directory group has READ permission\n");
			} else {
				printf(" directory group has NO READ permission\n");
			}
			if (sb.st_mode & S_IWGRP) {
				printf(" directory group has WRITE permission\n");
			} else {
				printf(" directory group has NO WRITE permission\n");
			}
			if (sb.st_mode & S_IXGRP) {
				printf(" directory group has SEARCH permission\n");
			} else {
				printf(" directory group has NO SEARCH permission\n");
			}

			if (sb.st_mode & S_IROTH) {
				printf(" others have READ permission\n");
			} else {
				printf(" others have NO READ permission\n");
			}
			if (sb.st_mode & S_IWOTH) {
				printf(" others have WRITE permission\n");
			} else {
				printf(" others have NO WRITE permission\n");
			}
			if (sb.st_mode & S_IXOTH) {
				printf(" others have SEARCH permission\n");
			} else {
				printf(" others have NO SEARCH permission\n");
			}

        } else {

			if (sb.st_mode & S_IRUSR) {
				printf(" file owner has READ permission\n");
			} else {
				printf(" file owner has NO READ permission\n");
			}
			if (sb.st_mode & S_IWUSR) {
				printf(" file owner has WRITE permission\n");
			} else {
				printf(" file owner has NO WRITE permission\n");
			}
			if (sb.st_mode & S_IXUSR) {
				printf(" file owner has EXECUTE permission\n");
			} else {
				printf(" file owner has NO EXECUTE permission\n");
			}

			if (sb.st_mode & S_IRGRP) {
				printf(" file group has READ permission\n");
			} else {
				printf(" file group has NO READ permission\n");
			}
			if (sb.st_mode & S_IWGRP) {
				printf(" file group has WRITE permission\n");
			} else {
				printf(" file group has NO WRITE permission\n");
			}
			if (sb.st_mode & S_IXGRP) {
				printf(" file group has EXECUTE permission\n");
			} else {
				printf(" file group has NO EXECUTE permission\n");
			}

			if (sb.st_mode & S_IROTH) {
				printf(" others have READ permission\n");
			} else {
				printf(" others have NO READ permission\n");
			}
			if (sb.st_mode & S_IWOTH) {
				printf(" others have WRITE permission\n");
			} else {
				printf(" others have NO WRITE permission\n");
			}
			if (sb.st_mode & S_IXOTH) {
				printf(" others have EXECUTE permission\n");
			} else {
				printf(" others have NO EXECUTE permission\n");
			}
        }

        //


        //printf("Link count:               %ld\n", (long) sb.st_nlink);


        printf("Last status change:       %s", ctime(&sb.st_ctime)); // Time of last status change.
        printf("Last file access:         %s", ctime(&sb.st_atime)); // Time of last access.
        printf("Last file modification:   %s", ctime(&sb.st_mtime)); // Time of last modification.


	}



	return 0;
}

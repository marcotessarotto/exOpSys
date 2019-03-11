/*
 * 10copy_file.c

   Copy the file named argv[1] to a new file named in argv[2].
*/
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */


#ifndef BUF_SIZE        /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[]) {
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "usage: %s old-file new-file\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    /* Open input and output files */

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1) {
    	fprintf(stderr,"error opening file %s\n", argv[1]);
    	exit(EXIT_FAILURE);
    }

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;      /* rw-rw-rw- */
    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1) {
    	fprintf(stderr,"error opening file %s\n", argv[2]);
    	exit(EXIT_FAILURE);
    }

    /* Transfer data until we encounter end of input or an error */

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
        if (write(outputFd, buf, numRead) != numRead) {
        	fprintf(stderr,"error: couldn't write whole buffer\n");
        	exit(EXIT_FAILURE);
        }
    if (numRead == -1) {
    	fprintf(stderr,"error read\n");
        exit(EXIT_FAILURE);
    }

    if (close(inputFd) == -1) {
    	fprintf(stderr,"error close input\n");
        exit(EXIT_FAILURE);
    }

    if (close(outputFd) == -1) {
    	fprintf(stderr,"error close output\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}




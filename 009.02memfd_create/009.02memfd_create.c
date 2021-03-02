#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <sys/un.h>
#include <sys/socket.h>
#include <ctype.h>

#include <errno.h>

/*
 *
 * il processo padre crea un file temporaneo con la system call memfd_create.
 * prova di utilizzo di F_SEAL_SHRINK.
 * passaggio del file descriptor tra processi tramite UNIX domain socket
 *
 *
 *
 *
 *
 *
 *     https://man7.org/linux/man-pages/man2/memfd_create.2.html

	   The memfd_create() system call provides a simple alternative to
       manually mounting a tmpfs(5) filesystem and creating and opening a
       file in that filesystem.  The primary purpose of memfd_create() is to
       create files and associated file descriptors that are used with the
       file-sealing APIs provided by fcntl(2).

       The memfd_create() system call also has uses without file sealing
       (which is why file-sealing is disabled, unless explicitly requested
       with the MFD_ALLOW_SEALING flag).  In particular, it can be used as
       an alternative to creating files in tmp or as an alternative to using
       the open(2) O_TMPFILE in cases where there is no intention to
       actually link the resulting file into the filesystem.
 *
 *
 *
 */

#define THREAD_VM_SIZE (100*1024*1024)

#define BUF_SIZE 256

const char * tmp_file_name = "tmpfile";

__off_t get_fd_size(int fd);


#define SERVER_SOCKET_PATH "/tmp/unix_domain_socket"

void parent_process();

int main(int argc, char * argv[]) {

	int fd;
	int flags;

	// crea file temporaneo, la dimensione iniziale del file è 0
	if ((fd = memfd_create(tmp_file_name, MFD_ALLOW_SEALING | MFD_CLOEXEC)) == -1) {
		perror("memfd_create");
		exit(EXIT_FAILURE);
	}

/*
		By default, the new file descriptor is set to remain open across an execve(2)
		(i.e., the FD_CLOEXEC file descriptor  flag  described  in  fcntl(2)  is  initially  disabled);
		the O_CLOEXEC flag, described below, can be used to change this default.
		The file offset is set to the beginning of the file (see lseek(2)).
*/
	// F_SEAL_SHRINK
    //If this seal is set, the file in question cannot be reduced in
    //size.  This affects open(2) with the O_TRUNC flag as well as
    //truncate(2) and ftruncate(2).  Those calls fail with EPERM if
    //you try to shrink the file in question.  Increasing the file
    //size is still possible.
	if (fcntl(fd, F_SEAL_SHRINK) == -1) {
		perror("fcntl");
	}

	// imposta la dimensione del file
	if (ftruncate(fd, THREAD_VM_SIZE) == -1) {
		perror("ftruncate");
		exit(EXIT_FAILURE);
	}

	// TODO: create memory map


	__off_t file_size;

	file_size = get_fd_size(fd);

	printf("[parent] file_size = %ld bytes\n", file_size);

	int pid;
	pid = getpid();
	printf("[parent] pid = %d\n", pid);

/*
 * A second process obtains a file descriptor for the tmpfs(5) file
          and maps it.  Among the possible ways in which this could happen
          are the following:

          *  The process that called memfd_create() could transfer the
             resulting file descriptor to the second process via a UNIX
             domain socket (see unix(7) and cmsg(3)).  The second process
             then maps the file using mmap(2).

          *  The second process is created via fork(2) and thus
             automatically inherits the file descriptor and mapping.  (Note
             that in this case and the next, there is a natural trust
             relationship between the two processes, since they are running
             under the same user ID.  Therefore, file sealing would not
             normally be necessary.)

          *  The second process opens the file /proc/<pid>/fd/<fd>, where
             <pid> is the PID of the first process (the one that called
             memfd_create()), and <fd> is the number of the file descriptor
             returned by the call to memfd_create() in that process.  The
             second process then maps the file using mmap(2).
 */


	// crea un UNIX domain socket

	struct sockaddr_un svaddr, claddr;
	int sfd;

	if (unlink(SERVER_SOCKET_PATH) == -1 && errno != ENOENT) {
		perror("unlink");
		exit(EXIT_FAILURE);
	}

    if (strlen(SERVER_SOCKET_PATH) > sizeof(svaddr.sun_path) - 1) {
    	printf("Server socket path too long: %s", SERVER_SOCKET_PATH);
    	exit(EXIT_FAILURE);
    }

    sfd = socket(AF_UNIX, SOCK_DGRAM, 0); // Create server socket
    if (sfd == -1) {
    	perror("socket");
    	exit(EXIT_FAILURE);
    }


    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SERVER_SOCKET_PATH, sizeof(svaddr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) == -1) {
    	perror("bind");
    	exit(EXIT_FAILURE);
    }

    // il server socket è pronto

    // crea un processo figlio
    if (fork() == 0) {
    	// comunica col processo figlio attraverso UNIX domain socket
    	parent_process();
    }


    ssize_t numBytes;
    socklen_t len;
    char buf[BUF_SIZE];

    for (;;) {

    	// aspettiamo la richiesta di inviare il file descriptor dal client

        len = sizeof(struct sockaddr_un);
        numBytes = recvfrom(sfd, buf, BUF_SIZE, 0,
                            (struct sockaddr *) &claddr, &len);
        if (numBytes == -1) {
        	perror("recvfrom");
        	exit(EXIT_FAILURE);
        }

        printf("[parent ] received %ld bytes from %s\n", (long) numBytes, claddr.sun_path);


        // TODO:
        // rispondi ...
    }


	if (close(fd) == -1) {
		perror("close");
	}


	printf("bye!\n");


	return 0;
}

void parent_process() {


	exit(EXIT_SUCCESS);
}

// restituisce la dimensione del file, -1 in caso di errore
__off_t get_fd_size(int fd) {

	struct stat sb;
	int res;

	res = fstat(fd, &sb);

	if (res == -1) {
		perror("fstat()");
		return -1;
	}

    return sb.st_size;
}



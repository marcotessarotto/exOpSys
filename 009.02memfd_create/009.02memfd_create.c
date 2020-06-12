#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>


/*
 *
 * il processo padre crea un file temporaneo con la system call memfd_create.
 * prova di F_SEAL_SHRINK, passaggio del file descriptor tra processi via UNIX domain socket
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

#define FILE_SIZE (100*1024*1024)

const char * tmp_file_name = "tmpfile";

__off_t get_fd_size(int fd);



int main(int argc, char * argv[]) {

	int fd;

	// crea file temporaneo, la dimensione iniziale del file è 0
	if ((fd = memfd_create(tmp_file_name, MFD_ALLOW_SEALING | MFD_CLOEXEC)) == -1) {
		perror("memfd_create");
		exit(EXIT_FAILURE);
	}

	// F_SEAL_SHRINK
    //If this seal is set, the file in question cannot be reduced in
    //size.  This affects open(2) with the O_TRUNC flag as well as
    //truncate(2) and ftruncate(2).  Those calls fail with EPERM if
    //you try to shrink the file in question.  Increasing the file
    //size is still possible.
	if (fcntl(fd, F_SEAL_SHRINK) == -1) {
		perror("fcntl");
	}

	if (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
		perror("fcntl");
	}

	if (ftruncate(fd, FILE_SIZE) == -1) {
		perror("ftruncate");
		exit(EXIT_FAILURE);
	}

	__off_t file_size;

	file_size = get_fd_size(fd);

	printf("[parent] file_size = %ld bytes\n", file_size);

	int pid;
	pid = getpid();
	printf("[parent] pid = %d\n", pid);

	if (fork() == 0) {

		char cmd0[256] = { 0 };

		file_size = get_fd_size(fd);
		printf("[child] file_size = %ld bytes\n", file_size);



		sprintf(cmd0, "/proc/%d/fd", pid);

		printf("[child] ls -l %s\n", cmd0);


		pid = getpid();
		printf("[child] pid = %d\n", pid);

		//getchar();


		execlp("ls", "ls", "-l", cmd0, NULL);

		exit(EXIT_FAILURE);

	}

	// aspettiamo la conclusione del processo figlio (se è stato avviato correttamente)
	wait(NULL);


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


	// TODO:
	// crea un processo figlio;
	// comunica col processo figlio attraverso UNIX domain socket




	if (close(fd) == -1) {
		perror("close");
	}


	printf("bye!\n");


	return 0;
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



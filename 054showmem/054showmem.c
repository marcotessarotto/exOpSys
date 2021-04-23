#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/sysmacros.h>

extern char **environ;  /* Or define _GNU_SOURCE to get it from <unistd.h> */

int main() {

	// il programma mostra i contenuti di /proc/<pid>/maps per il processo corrente

	// man 5 proc

	char * program_filename = "/usr/bin/cat";
	char param[256];
	char * proc_self = "/proc/self/maps";

	int pid;

	pid = getpid();

	snprintf(param, sizeof(param), "/proc/%d/maps", pid);

	printf("il comando che il processo figlio eseguirà é:\n%s %s\n\n", program_filename, param);
	printf("le informazioni del processo corrente si trovano anche in /proc/self/\n\n");
	// "/proc/self/maps"

	// allochiamo dei dati sulla heap memory, così lo vedremo in maps
	//char * mem = malloc(1024);

	switch (fork()) {
	case -1:
		perror("fork");
		exit(1);
	case 0:
		printf("address                   perms offset  dev   inode                       pathname\n");

		// parametri per il programma che invocheremo con execve
		char * new_arguments [] = {
				program_filename,
				//param,
				proc_self,
				NULL
		};

		execve(program_filename, new_arguments,
				environ);

		perror("execve");

		return 1;
	default:
		sleep(1);
		//getchar();

	}

/*

  	  	  	  tratto da man 5 proc:


              The address field is the address space in the process that the mapping occupies.  The perms field is a set of permissions:

                  r = read
                  w = write
                  x = execute
                  s = shared
                  p = private (copy on write)

              The  offset  field  is the offset into the file/whatever; dev is the device (major:minor); inode is the inode on that device.  0 indicates that no inode is associated with
              the memory region, as would be the case with BSS (uninitialized data).

              The pathname field will usually be the file that is backing the mapping.  For ELF files, you can easily coordinate with the offset field by looking at the Offset field  in
              the ELF program headers (readelf -l).

              There are additional helpful pseudo-paths:

              [stack]
                     The initial process's (also known as the main thread's) stack.


              [vdso] The virtual dynamically linked shared object.  See vdso(7).

              [heap] The process's heap.


 */


	return 0;
}

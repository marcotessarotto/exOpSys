#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/sysmacros.h>

extern char **environ;  /* Or define _GNU_SOURCE to get it from <unistd.h> */

char prova[] = "012345678";

int main() {

	// il programma mostra i contenuti di /proc/<pid>/maps per il processo corrente

	// man 5 proc

	char * program_filename = "/usr/bin/cat";
	char param[256];
	//char * proc_self = "/proc/self/maps";

	int pid;

	pid = getpid();

	snprintf(param, sizeof(param), "/proc/%d/maps", pid);

	printf("il comando che il processo figlio eseguirà é:\n%s %s\n\n", program_filename, param);
	//printf("le informazioni del processo corrente si trovano anche in /proc/self/\n\n");
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
				param,
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

	printf("\n");

	printf("indirizzo di main(): %p\n", main);

	printf("indirizzo della variabile 'prova': %p\n", &prova);


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


esempio:

aspetti interessanti:
- permessi r--xp
- indirizzi cambiano ad ogni esecuzione
- [heap]
- [stack]

address                   perms offset  dev   inode                       pathname
558fa68de000-558fa68e0000 r--p 00000000 00:19 5840119                    /usr/bin/cat
558fa68e0000-558fa68e5000 r-xp 00002000 00:19 5840119                    /usr/bin/cat
558fa68e5000-558fa68e8000 r--p 00007000 00:19 5840119                    /usr/bin/cat
558fa68e8000-558fa68e9000 r--p 00009000 00:19 5840119                    /usr/bin/cat
558fa68e9000-558fa68ea000 rw-p 0000a000 00:19 5840119                    /usr/bin/cat
558fa730a000-558fa732b000 rw-p 00000000 00:00 0                          [heap]
7fc375b87000-7fc375ba9000 rw-p 00000000 00:00 0
7fc375ba9000-7fc375bfe000 r--p 00000000 00:19 7660450                    /usr/lib/locale/aa_DJ.utf8/LC_CTYPE
7fc375bfe000-7fc375e76000 r--p 00000000 00:19 7660449                    /usr/lib/locale/aa_DJ.utf8/LC_COLLATE
7fc375e76000-7fc375e9b000 r--p 00000000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7fc375e9b000-7fc375fe6000 r-xp 00025000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7fc375fe6000-7fc376030000 r--p 00170000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7fc376030000-7fc376031000 ---p 001ba000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7fc376031000-7fc376034000 r--p 001ba000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7fc376034000-7fc376037000 rw-p 001bd000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7fc376037000-7fc37603d000 rw-p 00000000 00:00 0
7fc376049000-7fc37604a000 r--p 00000000 00:19 7660472                    /usr/lib/locale/aa_ET/LC_NUMERIC
7fc37604a000-7fc37604b000 r--p 00000000 00:19 7661328                    /usr/lib/locale/en_US.utf8/LC_TIME
7fc37604b000-7fc37604c000 r--p 00000000 00:19 7660947                    /usr/lib/locale/chr_US/LC_MONETARY
7fc37604c000-7fc37604d000 r--p 00000000 00:19 7661169                    /usr/lib/locale/en_AG/LC_MESSAGES/SYS_LC_MESSAGES
7fc37604d000-7fc37604e000 r--p 00000000 00:19 7660948                    /usr/lib/locale/chr_US/LC_PAPER
7fc37604e000-7fc37604f000 r--p 00000000 00:19 7660825                    /usr/lib/locale/bi_VU/LC_NAME
7fc37604f000-7fc376050000 r--p 00000000 00:19 7661326                    /usr/lib/locale/en_US.utf8/LC_ADDRESS
7fc376050000-7fc376051000 r--p 00000000 00:19 7660949                    /usr/lib/locale/chr_US/LC_TELEPHONE
7fc376051000-7fc376052000 r--p 00000000 00:19 7660945                    /usr/lib/locale/chr_US/LC_MEASUREMENT
7fc376052000-7fc376059000 r--s 00000000 00:19 7636497                    /usr/lib/x86_64-linux-gnu/gconv/gconv-modules.cache
7fc376059000-7fc37605a000 r--p 00000000 00:19 7636228                    /usr/lib/x86_64-linux-gnu/ld-2.31.so
7fc37605a000-7fc37607a000 r-xp 00001000 00:19 7636228                    /usr/lib/x86_64-linux-gnu/ld-2.31.so
7fc37607a000-7fc376082000 r--p 00021000 00:19 7636228                    /usr/lib/x86_64-linux-gnu/ld-2.31.so
7fc376082000-7fc376083000 r--p 00000000 00:19 7661327                    /usr/lib/locale/en_US.utf8/LC_IDENTIFICATION
7fc376083000-7fc376084000 r--p 00029000 00:19 7636228                    /usr/lib/x86_64-linux-gnu/ld-2.31.so
7fc376084000-7fc376085000 rw-p 0002a000 00:19 7636228                    /usr/lib/x86_64-linux-gnu/ld-2.31.so
7fc376085000-7fc376086000 rw-p 00000000 00:00 0
7ffffde56000-7ffffde79000 rw-p 00000000 00:00 0                          [stack]
7ffffdebf000-7ffffdec3000 r--p 00000000 00:00 0                          [vvar]
7ffffdec3000-7ffffdec5000 r-xp 00000000 00:00 0                          [vdso]

 */


	return 0;
}

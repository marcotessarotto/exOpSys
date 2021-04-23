#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/sysmacros.h>

extern char **environ;  /* Or define _GNU_SOURCE to get it from <unistd.h> */

// questa variabile si trova nel segmento dei dati inizializzati
char prova[] = "012345678";

extern char etext, edata, end;
/*
      The addresses of these symbols indicate the end of various program segments:

       etext  This is the first address past the end of the text segment (the program code).
       edata  This is the first address past the end of the initialized data segment.
       end    This is the first address past the end of the uninitialized data segment (also known as the BSS segment).
*/

int main() {

	// il programma mostra i contenuti di /proc/<pid>/maps per il processo corrente

	char * program_filename = "/usr/bin/cat";

	// static: questa variabile si trova nel segmento dei dati non inzializzati
	static char param[256];

	int pid;

	pid = getpid();

	snprintf(param, sizeof(param), "/proc/%d/maps", pid);

	printf("il comando che il processo figlio eseguirà é:\n%s %s\n\n", program_filename, param);
	//printf("le informazioni del processo corrente si trovano anche in /proc/self/\n\n");

	// allochiamo dei dati sulla heap memory, così lo vedremo in maps
	//char * mem = malloc(1024);

	switch (fork()) {
	case -1:
		perror("fork");
		exit(1);
	case 0:
		printf("address (start-end)       perms offset  dev   inode                       pathname\n");

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
	printf("\n");

	printf("indirizzo della variabile 'prova' (initialized data segment): %p\n", &prova);
	printf("sizeof(prova) = %lu bytes\n", sizeof(prova));
	printf("\n");

	printf("indirizzo della variabile 'param' (uninitialized data segment): %p\n", &param);
	printf("sizeof(param) = %lu bytes\n", sizeof(param));
	printf("\n");

	printf("indirizzo della variabile 'program_filename' [stack]: %p\n", &program_filename);
	printf("\n");


	printf("indirizzo di etext: %p\n", &etext);
	printf("indirizzo di edata: %p\n", &edata);
	printf("indirizzo di end: %p\n", &end);
	printf("\n");

	printf("&edata - &prova: %lu\n", &edata - (char *)&prova);

	printf("\netext  This is the first address past the end of the text segment (the program code).\n"
       "edata  This is the first address past the end of the initialized data segment.\n"
       "end    This is the first address past the end of the uninitialized data segment (also known as the BSS segment).\n");


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

address (start-end)       perms offset  dev   inode                       pathname
55e68b6b0000-55e68b6b1000 r--p 00000000 00:19 8423916                    /home/marco/Documents/git/exOpSys/054showmem/Debug/054showmem
55e68b6b1000-55e68b6b2000 r-xp 00001000 00:19 8423916                    /home/marco/Documents/git/exOpSys/054showmem/Debug/054showmem
55e68b6b2000-55e68b6b3000 r--p 00002000 00:19 8423916                    /home/marco/Documents/git/exOpSys/054showmem/Debug/054showmem
55e68b6b3000-55e68b6b4000 r--p 00002000 00:19 8423916                    /home/marco/Documents/git/exOpSys/054showmem/Debug/054showmem
55e68b6b4000-55e68b6b5000 rw-p 00003000 00:19 8423916                    /home/marco/Documents/git/exOpSys/054showmem/Debug/054showmem
55e68cfcf000-55e68cff0000 rw-p 00000000 00:00 0                          [heap]
7f7c35396000-7f7c353bb000 r--p 00000000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f7c353bb000-7f7c35506000 r-xp 00025000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f7c35506000-7f7c35550000 r--p 00170000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f7c35550000-7f7c35551000 ---p 001ba000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f7c35551000-7f7c35554000 r--p 001ba000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f7c35554000-7f7c35557000 rw-p 001bd000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f7c35557000-7f7c3555d000 rw-p 00000000 00:00 0
7f7c35579000-7f7c3557a000 r--p 00000000 00:19 7636228                    /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f7c3557a000-7f7c3559a000 r-xp 00001000 00:19 7636228                    /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f7c3559a000-7f7c355a2000 r--p 00021000 00:19 7636228                    /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f7c355a3000-7f7c355a4000 r--p 00029000 00:19 7636228                    /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f7c355a4000-7f7c355a5000 rw-p 0002a000 00:19 7636228                    /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f7c355a5000-7f7c355a6000 rw-p 00000000 00:00 0
7ffe65028000-7ffe6504b000 rw-p 00000000 00:00 0                          [stack]
7ffe65199000-7ffe6519d000 r--p 00000000 00:00 0                          [vvar]
7ffe6519d000-7ffe6519f000 r-xp 00000000 00:00 0                          [vdso]

indirizzo di main(): 0x55e68b6b11c5

indirizzo della variabile 'prova' (initialized data segment): 0x55e68b6b4078
sizeof(prova) = 10 bytes

indirizzo della variabile 'param' (uninitialized data segment): 0x55e68b6b40c0
sizeof(param) = 256 bytes

indirizzo della variabile 'program_filename' [stack]: 0x7ffe65047df0

indirizzo di etext: 0x55e68b6b145d
indirizzo di edata: 0x55e68b6b4082
indirizzo di end: 0x55e68b6b41c0

&edata - &prova: 10

etext  This is the first address past the end of the text segment (the program code).
edata  This is the first address past the end of the initialized data segment.
end    This is the first address past the end of the uninitialized data segment (also known as the BSS segment).
 */


	return 0;
}

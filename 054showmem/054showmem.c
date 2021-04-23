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

	printf("indirizzo della variabile 'param': %p\n", &param);

	printf("indirizzo della variabile 'program_filename' [stack]: %p\n", &program_filename);


	printf("indirizzo di etext: %p\n", &etext);
	printf("indirizzo di edata: %p\n", &edata);
	printf("indirizzo di end: %p\n", &end);

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

address                   perms offset  dev   inode                       pathname
55a33c431000-55a33c432000 r--p 00000000 00:19 8423013                    /home/marco/Documents/git/exOpSys/054showmem/Debug/054showmem
55a33c432000-55a33c433000 r-xp 00001000 00:19 8423013                    /home/marco/Documents/git/exOpSys/054showmem/Debug/054showmem
55a33c433000-55a33c434000 r--p 00002000 00:19 8423013                    /home/marco/Documents/git/exOpSys/054showmem/Debug/054showmem
55a33c434000-55a33c435000 r--p 00002000 00:19 8423013                    /home/marco/Documents/git/exOpSys/054showmem/Debug/054showmem
55a33c435000-55a33c436000 rw-p 00003000 00:19 8423013                    /home/marco/Documents/git/exOpSys/054showmem/Debug/054showmem
55a33d713000-55a33d734000 rw-p 00000000 00:00 0                          [heap]
7fabfa949000-7fabfa96e000 r--p 00000000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7fabfa96e000-7fabfaab9000 r-xp 00025000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7fabfaab9000-7fabfab03000 r--p 00170000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7fabfab03000-7fabfab04000 ---p 001ba000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7fabfab04000-7fabfab07000 r--p 001ba000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7fabfab07000-7fabfab0a000 rw-p 001bd000 00:19 7636232                    /usr/lib/x86_64-linux-gnu/libc-2.31.so
7fabfab0a000-7fabfab10000 rw-p 00000000 00:00 0
7fabfab2c000-7fabfab2d000 r--p 00000000 00:19 7636228                    /usr/lib/x86_64-linux-gnu/ld-2.31.so
7fabfab2d000-7fabfab4d000 r-xp 00001000 00:19 7636228                    /usr/lib/x86_64-linux-gnu/ld-2.31.so
7fabfab4d000-7fabfab55000 r--p 00021000 00:19 7636228                    /usr/lib/x86_64-linux-gnu/ld-2.31.so
7fabfab56000-7fabfab57000 r--p 00029000 00:19 7636228                    /usr/lib/x86_64-linux-gnu/ld-2.31.so
7fabfab57000-7fabfab58000 rw-p 0002a000 00:19 7636228                    /usr/lib/x86_64-linux-gnu/ld-2.31.so
7fabfab58000-7fabfab59000 rw-p 00000000 00:00 0
7ffea399d000-7ffea39c0000 rw-p 00000000 00:00 0                          [stack]
7ffea39c7000-7ffea39cb000 r--p 00000000 00:00 0                          [vvar]
7ffea39cb000-7ffea39cd000 r-xp 00000000 00:00 0                          [vdso]

indirizzo di main(): 0x55a33c4321c5
indirizzo della variabile 'prova': 0x55a33c435078
indirizzo della variabile 'param': 0x55a33c4350c0
indirizzo della variabile 'program_filename' [stack]: 0x7ffea39bb620
indirizzo di etext: 0x55a33c4323dd
indirizzo di edata: 0x55a33c435082
indirizzo di end: 0x55a33c4351c0

etext  This is the first address past the end of the text segment (the program code).
edata  This is the first address past the end of the initialized data segment.
end    This is the first address past the end of the uninitialized data segment (also known as the BSS segment).

 */


	return 0;
}

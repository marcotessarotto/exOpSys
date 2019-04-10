#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <termios.h>

char * cat(char * file_name);

int virt_to_phys_user(uintptr_t *paddr, pid_t pid, uintptr_t vaddr);

int count_chars(const char* string, char ch)
{
    int count = 0;
    int i;

    int length = strlen(string);

    for (i = 0; i < length; i++)
    {
        if (string[i] == ch)
        {
            count++;
        }
    }

    return count;
}

// man 3 end
extern char etext, edata, end;


typedef struct proc_maps_item_perms {
		   int r:1;
		   int w:1;
		   int x:1;
		   int p:1;
		   int s:1;
	} proc_maps_item_perms_t;

typedef struct proc_maps_item {
    void * region_start, * region_stop;

    unsigned long offset;
    unsigned long inode;

    char path[4096];

    proc_maps_item_perms_t permission;
    char * region_type;

    char region_permissions[5];

} proc_maps_item_t;


proc_maps_item_t * get_process_memory_mappings() {
	char * maps_content = cat("/proc/self/maps");

	if (maps_content == NULL)
		return NULL;

	int number_of_lines = count_chars(maps_content, '\n');

	proc_maps_item_t * result = malloc((number_of_lines + 1) * sizeof(proc_maps_item_t));


	int i = 0;

	char * ptr = maps_content;

	while (ptr != NULL) {

		if (ptr[0] == '\n') {
			ptr++;
			continue;
		}


		// il formato di /proc/<id>/maps non è regolare:
		char * end_of_line = strchr(ptr,'\n');
		if (end_of_line == NULL)
			break;
		*end_of_line = 0;

		result[i].path[0] = '\0';

		int read_vars = sscanf(ptr, "%p-%p %s %lx %*s %lu %s", &result[i].region_start, &result[i].region_stop, &result[i].region_permissions[0], &result[i].offset, &result[i].inode, &result[i].path[0]);

		if (read_vars == -1)
			break;

		ptr = end_of_line + 1;


		result[i].permission.r = result[i].region_permissions[0] =='r';
		result[i].permission.w = result[i].region_permissions[1] =='w';
		result[i].permission.x = result[i].region_permissions[2] =='x';
		result[i].permission.p = result[i].region_permissions[3] =='p';
		result[i].permission.s = result[i].region_permissions[3] =='s';

		if (result[i].permission.x && result[i].permission.r)
			result[i].region_type = "TEXT  ";
		else if (result[i].permission.r && result[i].permission.w)
			result[i].region_type = "DATARW";
		else if (result[i].permission.r)
			result[i].region_type = "DATARO";
		if (!strcmp(result[i].path,"[heap]"))
			result[i].region_type = "HEAP  ";
		else if (!strcmp(result[i].path,"[stack]"))
			result[i].region_type = "STACK ";

		i++;
	}

	result[i].region_start = result[i].region_stop = result[i].region_type = NULL;

	return result;
}


int main(int argc, char *argv[])
{

	// printf(argv[0]);

    // in Linux, la memoria è organizzata in "pagine"
    // 32bit: una pagina è formata da 4096 bytes (4 KB) [12 bit]
    //

/*
x86-64 uses "canonical form", with 48 bits assigned for virtual address space.
Bits 48 through 63 of any virtual address must be copies of bit 47 , or the
processor will raise an exception. Canonical form addresses run from 0 through
00007FFF`FFFFFFFF, and from FFFF8000`00000000 through FFFFFFFF`FFFFFFFF, for a
total of 256 TB of usable virtual address space.

The virtual address space layout is shown below.


    ffffffff`ffffffff     _____________
                          |            |
                          |  region 2  |  <--- kernel space
    ffff8000`00000000     |____________|
                          |            |
                          | Unused     |
                          |  Space     |
                          |            |
    00007fff`ffffffff     |____________|
                          |            |
                          |   region 1 |  <--- user space
    00000000`00000000     |____________|
 */



    // segmenti (dal basso verso l'alto)
    // Code
    // Data (read only & rw)
    // BSS (dati non inizializzati nel programma, ma inizializzati dal kernel)
    // Heap
    // Stack

/*
 * tratto da https://gabrieletolomei.wordpress.com/miscellanea/operating-systems/in-memory-layout/
 *
– Text: Text segment, also known as a Code segment, is the section of memory which contains executable instructions of a program.
It may be placed below the Heap or Stack in order to prevent overflows from overwriting it.
Usually, the text segment is sharable so that only a single copy needs to be in memory for frequently executed programs, such as text editors,
the C compiler, the shells, and so on.
The text segment is usually Read-only/Execute to prevent a program from accidentally modifying its instructions.

– Data: Data segment is a shorthand for “Initialized data segment”. This portion of the virtual address space of a program contains the global and static variables, constant data.
	a) Read-only Data
	b) Initialized Read-Write Data

– BSS: BSS segment also refers to “Uninitialized data segment”, and it is named so after an ancient assembler operator that stood for “Block Started by Symbol”.
Data in this segment is initialized by the OS kernel to arithmetic 0 before the program starts executing.
Typically, this segment starts at the end of the Data segment and contains all global and static variables that are initialized to zero or do not have explicit initialization in source code.
The BSS segment is Read-Write

– Heap: Heap is the segment where dynamic memory allocation usually takes place, i.e., to allocate memory requested by the programmer for variables
whose size can be only known at run-time and cannot be statically determined by the compiler before program execution.
The heap area begins at the end of the BSS segment and grows upwards to higher memory addresses.
Heap segment is shared by all shared libraries and dynamically loaded modules in a process.

 *
 */

    // GNU Standard C Library (malloc, calloc, free) -> system calls brk, sbrk, mmap

    /* If the standard streams are connected to a tty,
     * tell the kernel to discard already buffered data.
     * (That is, in kernel buffers. Not C library buffers.)
    */
    if (isatty(STDIN_FILENO))
        tcflush(STDIN_FILENO, TCIOFLUSH);
    if (isatty(STDOUT_FILENO))
        tcflush(STDOUT_FILENO, TCIOFLUSH);
    if (isatty(STDERR_FILENO))
        tcflush(STDERR_FILENO, TCIOFLUSH);


	uid_t real_user_id = getuid(); // l'utente proprietario del processo

	uid_t effective_user_id = geteuid(); // l'utente che il kernel utilizza per decidere sui permessi

	printf("real UID: %d\neffective UID: %d\n\n", real_user_id, effective_user_id);

	printf("quale utente effettivo devo diventare? (-1 per saltare questo passaggio) ");

	int target_uid = 1000;

	scanf("%d", &target_uid); // scanf non consuma i caratteri dopo il numero che ha letto

	getchar(); // legge il carattere residuo \n (o altro...)

	if (target_uid != -1) {

		printf("\nda shell:\nid -ur  # mostra 'real UID'\nid -u   # mostra 'effective UID'\n");

		printf("\nprovo a diventare effective user %d...\n", target_uid);

		// root ha UID 0
		// utente dovrebbe avere UID 1000 (in ogni caso, vedere /etc/passwd)

		if (seteuid(target_uid) == -1) {
			perror("seteuid() fail");
		} else {
			printf("seteuid() ok!\n");
			printf("real UID: %d\neffective UID: %d\n\n", getuid(), geteuid());
		}

	}

	printf("premi enter per continuare\n");
	getchar();


	uintptr_t physical_addr;

	if (geteuid() != 0) {
		printf("***attenzione!!! effective user non è 0 => non posso determinare correttamente gli indirizzi fisici***\n");
	}

	// la lettura di /proc/PID/pagemap è una operazione 'privilegiata'
	virt_to_phys_user(&physical_addr, -1, (uintptr_t) &etext);

    printf("First address past:\n");
    printf("    program text (etext)      %10p, physical addr: %10p (%lu)\n", &etext, (void * )physical_addr, physical_addr);

    virt_to_phys_user(&physical_addr, -1, (uintptr_t) &edata);

    printf("    initialized data (edata)  %10p, physical addr: %10p (%lu)\n", &edata, (void * )physical_addr, physical_addr);
    printf("    uninitialized data (end)  %10p\n", &end);

    printf("edata - text: %lu\n", &edata - &etext);
    printf("end - edata: %lu\n", &end - &edata);
    printf("\n");

    ////////

    printf("un processore a 32 bit indirizza 4 GB di memoria\n");
    printf("i primi 3 GB sono riservati allo 'user space', l'ultimo GB è riservato al 'kernel space' a partire da 0xc0000000\n");

    printf("processori a 64 bit: 'solo' i 48 bit meno significativi sono usati per l'indirizzamento\n");
    printf("i processori a 64 bit hanno a disposizione 256 Terabyte di indirizzamento.\n");

    printf("Linux 64 bit: user space utilizza i primi 128 TB, kernel space i secondi 128 TB\n");
    printf("\n");


	// quale è la dimensione della memoria RAM del sistema?
    // less /proc/meminfo
    // MemTotal:       16088884 kB

	// quali sono gli indirizzi "reali"? quali sono gli indirizzi "virtuali"?
    // ogni processo in user space "vede" indirizzi "virtuali"...

	// quali sono i segmenti text, data, heap, stack?

	// dato un indirizzo virtuale, si può scoprire quale è il suo indirizzo "reale"?

	/*
	 *
	 * cat /proc/self/maps
	 *
	 *
	 */

    /*
        da man 5 proc:

        address           perms offset  dev   inode       pathname
        00400000-00452000 r-xp 00000000 08:02 173521      /usr/bin/dbus-daemon
        ...
        00e03000-00e24000 rw-p 00000000 00:00 0           [heap]
        00e24000-011f7000 rw-p 00000000 00:00 0           [heap]
        ...
        35b1800000-35b1820000 r-xp 00000000 08:02 135522  /usr/lib64/ld-2.15.so
        ...
        f2c6ff8c000-7f2c7078c000 rw-p 00000000 00:00 0    [stack:986]
        ...
        7fffb2c0d000-7fffb2c2e000 rw-p 00000000 00:00 0   [stack]
        7fffb2d48000-7fffb2d49000 r-xp 00000000 00:00 0   [vdso]

                  The address field is the address space in the process that the
                  mapping occupies.  The perms field is a set of permissions:

                      r = read
                      w = write
                      x = execute
                      s = shared
                      p = private (copy on write)

                  The offset field is the offset into the file/whatever; dev is
                  the device (major:minor); inode is the inode on that device.
                  0 indicates that no inode is associated with the memory
                  region, as would be the case with BSS (uninitialized data).

                  The pathname field will usually be the file that is backing
                  the mapping.  For ELF files, you can easily coordinate with
                  the offset field by looking at the Offset field in the ELF
                  program headers (readelf -l).

                  There are additional helpful pseudo-paths:

                       [stack]
                              The initial process's (also known as the main
                              thread's) stack.

                       [stack:<tid>] (from Linux 3.4 to 4.4)
                              A thread's stack (where the <tid> is a thread ID).
                              It corresponds to the /proc/[pid]/task/[tid]/
                              path.  This field was removed in Linux 4.5, since
                              providing this information for a process with
                              large numbers of threads is expensive.

                       [vdso] The virtual dynamically linked shared object.  See
                              vdso(7).

                       [heap] The process's heap.

                  If the pathname field is blank, this is an anonymous mapping
                  as obtained via mmap(2).  There is no easy way to coordinate
                  this back to a process's source, short of running it through
                  gdb(1), strace(1), or similar.

     */


    printf("\n\nget_process_memory_mappings\n");

    proc_maps_item_t * result = get_process_memory_mappings();
    int i = 0;

    while (result[i].region_start != NULL) {
    	printf("%s start=%p stop=%p size=%6lx perms=%s inode=%lu %s\n",
    			result[i].region_type,
				result[i].region_start, result[i].region_stop,
				result[i].region_stop - result[i].region_start,
				result[i].region_permissions, result[i].inode, result[i].path

    	);

    	i++;
    }

    printf("bye!\n");

	return 0;
}

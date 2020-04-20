#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/sysmacros.h>

/*
 * questo esempio prende come parametro il percorso ad un file
 * parte da / e prova a percorrere tutto il percorso, di directory in directory,
 * per verificare se l'utente che ha chiamato questo programma può arrivare fino al file
 *
 * vedere anche il comando namei
 */


void check_user_permissions(char * path) {

/* da man 2 access
 In  other  words,  access()  does  not  answer  the  "can I read/write/execute  this  file?"  question.
 It answers a slightly different question:
 "(assuming I'm a setuid binary) can the user who invoked me read/write/execute this file?",
 which gives set-user-ID programs the possibility to prevent malicious users
 from causing them to read files which users shouldn't be able to read.

 */

	struct stat sb;
	int res;
	int is_directory = 0;

	printf("path: %s\n", path);

	res = stat(path, &sb);

	if (res == -1) {
		perror("stat()");
		return;
	}

	is_directory = (sb.st_mode & S_IFMT) == S_IFDIR;

	printf("is_directory=%d ", is_directory);

    printf("owner(UID=%ld GID=%ld) \n",
            (long) sb.st_uid, (long) sb.st_gid);

    // the check on the file is done using the calling process's real UID and GID
    // i.e. "can the user who invoked me read/write/execute this file?"
    res = access(path, X_OK);

    if (res == -1) {
    	perror("access error: ");
    }

    if (is_directory)
    	printf("directory searchable by process's real UID and GID? %s\n", res == 0 ? "yes" : "*no*");

    if (res != 0) {
    	printf("user:%c%c%c ",
    			sb.st_mode & S_IRUSR ? 'r' : '-',
				sb.st_mode & S_IWUSR ? 'w' : '-',
				sb.st_mode & S_IXUSR ? 'x' : '-'
    			);

    	printf(" group:%c%c%c ",
				sb.st_mode & S_IRGRP ? 'r' : '-',
				sb.st_mode & S_IWGRP ? 'w' : '-',
				sb.st_mode & S_IXGRP ? 'x' : '-'
				);

    	printf(" other:%c%c%c \n",
				sb.st_mode & S_IROTH ? 'r' : '-',
				sb.st_mode & S_IWOTH ? 'w' : '-',
				sb.st_mode & S_IXOTH ? 'x' : '-'
				);
    }


}


int main(int argc, char * argv[]) {
	char * absolute_path;

	if (argc == 1) {
		printf("arguments: provide file path to explore starting from root\n");
		exit(EXIT_FAILURE);
	}

	absolute_path = realpath(argv[1], NULL);

	if (absolute_path == NULL) {
		perror("realpath()");
		exit(EXIT_FAILURE);
	}

	// A - process real user ID (e real group ID)
	// identifica l'utente ed il gruppo a cui il processo appartiene

	// quando l'utente fa login (con username e password), la shell di login ottiene il suo "real user ID"
	// "real group ID" da /etc/passwd

	// fork(): quando il processo è creato (clonato) eredita lo stesso real user ID del processo padre

	// B - process effective user ID (ed effective group ID)
	// sono usati dal Kernel per determinare gli effettivi permessi del processo nelle system call

	// esempio: open() con O_CREAT
    // If pathname does not exist, create it as a regular file.
    // The owner (user ID) of the new file is set to the effective user ID of the process.

	uid_t process_real_user_id = getuid();
	gid_t process_real_group_id = getgid();

	uid_t process_effective_user_id = geteuid();
	gid_t process_effective_group_id = getegid();

	printf("process real user ID:%d\n", process_real_user_id);
	printf("process effective user ID:%d\n\n", process_effective_user_id);

	printf("process real group ID:%d\n", process_real_group_id);
	printf("process effective group ID:%d\n\n", process_effective_group_id);

	// normalmente: real user ID è uguale a process effective user ID (idem per i group ID)
	// ci sono dei meccanismi con cui effective user ID può cambiare -> set-user-ID, set-group-ID


    char * ptr;

    ptr = absolute_path;

    check_user_permissions("/");

    while (ptr != NULL && *ptr == '/') {
    	// strchr() restituisce un puntatore alla prima occorrenza del carattere c nella stringa s
    	// se strchr() non trova il carattere c, restituisce NULL

    	ptr = strchr(ptr+1,'/');

    	if (ptr == NULL) {
    		check_user_permissions(absolute_path);
    		break;
    	} else {
    		*ptr = '\0';
    		check_user_permissions(absolute_path);
    		*ptr = '/';
    	}
    }

    return 0;
}

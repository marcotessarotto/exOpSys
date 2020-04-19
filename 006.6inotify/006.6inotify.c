#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */
#include <stdbool.h>    /* 'bool' type plus 'true' and 'false' constants */

#include <sys/inotify.h>
#include <limits.h>


static void show_inotify_event(struct inotify_event *i)
{
    printf("[wd=%2d] ", i->wd);

    if (i->cookie > 0)
        printf("cookie=%4d ", i->cookie);

    // The  name  field is present only when an event is returned for a file
    // inside a watched directory; it identifies the filename within the watched directory.
    // This filename is null-terminated .....

    if (i->len > 0)
        printf("file name = %s ", i->name);
    else
    	printf("*no file name* "); // event refers to watched directory

    // see man inotify
    // for explanation of events

    printf("mask = ");

    // IN_ACCESS  File was accessed (e.g., read(2), execve(2)).
    if (i->mask & IN_ACCESS)        printf("IN_ACCESS ");

    if (i->mask & IN_ATTRIB)        printf("IN_ATTRIB ");
    if (i->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");

    // IN_CLOSE_NOWRITE  File or directory not opened for writing was closed.
    if (i->mask & IN_CLOSE_WRITE)   printf("IN_CLOSE_WRITE ");

    if (i->mask & IN_CREATE)        printf("IN_CREATE ");
    if (i->mask & IN_DELETE)        printf("IN_DELETE ");
    if (i->mask & IN_DELETE_SELF)   printf("IN_DELETE_SELF ");
    if (i->mask & IN_IGNORED)       printf("IN_IGNORED ");

    // IN_ISDIR  Subject of this event is a directory.
    if (i->mask & IN_ISDIR)         printf("IN_ISDIR ");

    if (i->mask & IN_MODIFY)        printf("IN_MODIFY ");
    if (i->mask & IN_MOVE_SELF)     printf("IN_MOVE_SELF ");
    if (i->mask & IN_MOVED_FROM)    printf("IN_MOVED_FROM ");
    if (i->mask & IN_MOVED_TO)      printf("IN_MOVED_TO ");

    // IN_OPEN  File or directory was opened.
    if (i->mask & IN_OPEN)          printf("IN_OPEN ");

    if (i->mask & IN_Q_OVERFLOW)    printf("IN_Q_OVERFLOW ");
    if (i->mask & IN_UNMOUNT)       printf("IN_UNMOUNT ");
    printf("\n");

}


#define BUF_LEN 4096

char buf[BUF_LEN] __attribute__ ((aligned(__alignof__(struct inotify_event))));


int main(int argc, char * argv[]) {

	int wd;
	int inotifyFd;
	int num_bytes_read;

	char * cwd;

	cwd = getcwd(NULL, 0);

	printf("process current working directory: %s\n", cwd);

	free(cwd);

	// inotify_init() initializes a new inotify instance and
	// returns a file descriptor associated with a new inotify event queue.
    inotifyFd = inotify_init();
    if (inotifyFd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // for each command line argument:
    for (int j = 1; j < argc; j++) {

    	// inotify_add_watch()  adds  a  new  watch, or modifies an existing watch,
    	// for the file whose location is specified in pathname
        wd = inotify_add_watch(inotifyFd, argv[j], IN_ALL_EVENTS);
        if (wd == -1) {
            perror("inotify_init");
            exit(EXIT_FAILURE);
        }

        // we do not keep wd...

        printf("Watching %s using wd %d\n", argv[j], wd);
    }

    printf("ready!\n\n");

    // loop forever
    for (;;) {
    	num_bytes_read = read(inotifyFd, buf, BUF_LEN);
        if (num_bytes_read == 0) {
            printf("read() from inotify fd returned 0!");
            exit(EXIT_FAILURE);
        }

        if (num_bytes_read == -1) {

        	if (errno == EINTR) {
				printf("read(): EINTR\n");
				continue;
        	} else {
                perror("read()");
                exit(EXIT_FAILURE);
        	}
        }

        printf("read %d bytes from inotify fd\n", num_bytes_read);

        // process all of the events in buffer returned by read()

        struct inotify_event *event;

        for (char * p = buf; p < buf + num_bytes_read; ) {
            event = (struct inotify_event *) p;

            show_inotify_event(event);

            p += sizeof(struct inotify_event) + event->len;
            // event->len is length of (optional) file name
        }
    }


	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <sys/param.h>

#define err_exit(MESSAGE) (                \
    perror(MESSAGE),                       \
    exit(1)                                \
)

int main()
{
   struct utsname uts;
   char hostname[MAXHOSTNAMELEN];

   size_t size = MAXHOSTNAMELEN;

   if(gethostname(hostname,size)!=0)
      err_exit("Could not get hostname");

   printf ("gethostname returns hostname=%s\n", hostname);

   // man 2 uname
   // uname - get name and information about current kernel
   if(uname(&uts) < 0)
      err_exit("Could not get kernel information");

   printf("uname returns the following information:\n"
		   "system is %s on %s hardware\n", uts.sysname, uts.machine);
   printf("nodename is %s\n", uts.nodename);
   printf("release is %s\nversion is %s\n", uts.release,uts.version);

   return EXIT_SUCCESS;
}

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>

// example taken from:
// man lstat

int
main(int argc, char *argv[])
{
   struct stat sb;

   if (argc != 2) {
	   fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
	   exit(EXIT_FAILURE);
   }

   if (lstat(argv[1], &sb) == -1) {
	   perror("lstat");
	   exit(EXIT_FAILURE);
   }

   printf("ID of containing device:  [%lx,%lx]\n",
		(long) major(sb.st_dev), (long) minor(sb.st_dev));

   printf("I-node number:            %ld\n", (long) sb.st_ino);
   // device ID + I-node: uniquely identifies a file across all file systems


   printf("File type:                ");

   switch (sb.st_mode & S_IFMT) {
	   case S_IFBLK:  printf("block device\n");            break;
	   case S_IFCHR:  printf("character device\n");        break;
	   case S_IFDIR:  printf("directory\n");               break;
	   case S_IFIFO:  printf("FIFO/pipe\n");               break;
	   case S_IFLNK:  printf("symlink\n");                 break;
	   case S_IFREG:  printf("regular file\n");            break;
	   case S_IFSOCK: printf("socket\n");                  break;
	   default:       printf("unknown?\n");                break;
   }


   printf("Mode:                     %lo (octal)\n",
		   (unsigned long) sb.st_mode & ~S_IFMT);

   printf("Link count:               %ld\n", (long) sb.st_nlink);
   printf("Ownership:                UID=%ld   GID=%ld\n",
		   (long) sb.st_uid, (long) sb.st_gid);

   printf("Preferred I/O block size: %ld bytes\n",
		   (long) sb.st_blksize);
   printf("File size:                %lld bytes\n",
		   (long long) sb.st_size);
   printf("Blocks allocated:         %lld\n",
		   (long long) sb.st_blocks);

   // Kerrisk pag. 286

   // read(): access
   // write(): modification, change
   // open() create: access, modification, change
   // open() truncate: modification, change

   printf("Last status change:       %s", ctime(&sb.st_ctime));
   printf("Last file access:         %s", ctime(&sb.st_atime));
   printf("Last file modification:   %s", ctime(&sb.st_mtime));

   exit(EXIT_SUCCESS);
}





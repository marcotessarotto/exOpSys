#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

// https://stackoverflow.com/questions/1085083/regular-expressions-in-c-examples

// man 3 regex
// man 7 regex

// taken from example in man 3 regex


#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

static const char *const str = "application/atomcat+xml				atomcat\n";
	   //"1) John Driverhacker;\n2) John Doe;\n3) John Foo;\n";

//static const char *const re = "John.*o";

static const char *const re = ".+/.+\\b";

//static const char *const re = "ap+";

int main(void)
{
   static const char *s = str;
   regex_t     regex;
   regmatch_t  pmatch[1];
   regoff_t    off, len;
   int delta;

   if (regcomp(&regex, re, REG_NEWLINE | REG_EXTENDED))
	   exit(EXIT_FAILURE);

   printf("search string = \"%s\"\n\n", str);

   printf("regular expression: %s\n\n", re);

   printf("matches of regex in search string:\n\n");

   for (int i = 0; ; i++) {
	   if (regexec(&regex, s, ARRAY_SIZE(pmatch), pmatch, 0) == REG_NOMATCH) {
		   printf("no more regex matches\n");
		   break;
	   }

	   off = pmatch[0].rm_so + (s - str);
	   len = pmatch[0].rm_eo - pmatch[0].rm_so;

	   delta = s - str;
	   printf("match #%d, from position %d to position %d in search string:\n", i, pmatch[0].rm_so + delta, pmatch[0].rm_eo + delta); // corrispondenza #i

	   printf("offset = %jd; length = %jd\n", (intmax_t) off,
			   (intmax_t) len);
	   printf("substring = \"%.*s\"\n", len, s + pmatch[0].rm_so);

	   printf("\n");

	   s += pmatch[0].rm_eo;
   }

   regfree(&regex); // free the memory allocated to the pattern buffer by the compiling process
   // here it is not necessary since program is terminating

   exit(EXIT_SUCCESS);
}

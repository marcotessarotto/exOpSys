#include <stdio.h>
#include <string.h>


/*
 * esempio che utilizza strtok() per suddividere una stringa di caratteri
 * in blocchi di testo (token)
 */

int main () {

   //char str[] = "hello world mi chiamo pino sono date delle stringhe";
   char str[] = "prima seconda   terza quarta";
   char s[] = " "; // caratteri separatori da utilizzare per la suddivisione
   char *token;


   // ottieni il primo token
   token = strtok(str, s);

   // "cammina" attraverso gli altri token
   while( token != NULL ) {
      printf( "%s\n", token );

      token = strtok(NULL, s); // le chiamate successive a strtok
   }

   return 0;
}

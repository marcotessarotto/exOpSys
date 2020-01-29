mkdir Debug

cd Debug
 
gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"printargs.d" -MT"printargs.o" -o "printargs.o" "../printargs.c"
  
gcc  -o "01printargs"  ./printargs.o   
 



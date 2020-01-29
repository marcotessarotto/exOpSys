mkdir build_dir

cd build_dir

gcc -O2 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"01files.d" -MT"01files.o" -o "01files.o" "../01files.c"
  
gcc -O2 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"mmap_anon.d" -MT"mmap_anon.o" -o "mmap_anon.o" "../mmap_anon.c"

gcc -O2 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"system_info.d" -MT"system_info.o" -o "system_info.o" "../system_info.c"
 
gcc -pthread -o "01files"  ./01files.o ./mmap_anon.o ./system_info.o   
 


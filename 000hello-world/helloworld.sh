# salvare il file come helloworld.sh in /home/utente
# aprire un emulatore di terminale e lanciare lo script con questo comando:
# sh helloworld.sh

mkdir helloworld

cd helloworld/

echo "#include <stdio.h>
int main() {
   printf(\"hello world!\");

   return 0;
}" > helloworld.c

gedit helloworld.c &

echo "invoco il preprocessore"
cpp helloworld.c > helloworld.i

gedit helloworld.i &

echo "invoco il compilatore C"
gcc -S helloworld.i

gedit helloworld.s &

echo "invoco l'assemblatore"
as helloworld.s  -o helloworld.o

echo "invoco il linker"
ld -dynamic-linker /lib64/ld-linux-x86-64.so.2 -pie /usr/lib/gcc/x86_64-linux-gnu/8/../../../x86_64-linux-gnu/Scrt1.o /usr/lib/gcc/x86_64-linux-gnu/8/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/8/crtbeginS.o -L/usr/lib/gcc/x86_64-linux-gnu/8 -L/usr/lib/gcc/x86_64-linux-gnu/8/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/8/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/8/../../.. helloworld.o -lgcc --push-state --as-needed -lgcc_s --pop-state -lc -lgcc --push-state --as-needed -lgcc_s --pop-state /usr/lib/gcc/x86_64-linux-gnu/8/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/8/../../../x86_64-linux-gnu/crtn.o -o helloworld

./helloworld

ls -l

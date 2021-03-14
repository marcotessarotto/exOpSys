#include <stdio.h>
#include <omp.h>


// https://medium.com/swlh/openmp-on-ubuntu-1145355eeb2

// sudo apt install libomp-dev
// gcc flag: -fopenmp
// link with gomp library

// libgomp, the GNU Offloading and Multi Processing Runtime Library
// https://gcc.gnu.org/onlinedocs/libgomp/
// https://www.openmp.org/wp-content/uploads/OpenMPRef-5.0-0519-web.pdf
// https://www.openmp.org/resources/tutorials-articles/

// https://hpc-tutorials.llnl.gov/posix/



int main() {

	omp_set_num_threads(10);
	#pragma omp parallel
	{
	    printf("thread %d\n", omp_get_thread_num());
	}

	printf("***\n");

	#pragma omp parallel for
	for(int i=0;i<10;i++){
		printf("%i\n",i);
	}

	printf("***\n");

	return 0;
}

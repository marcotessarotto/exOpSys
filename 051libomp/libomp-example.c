#include <stdio.h>
#include <omp.h>


// https://medium.com/swlh/openmp-on-ubuntu-1145355eeb2

// sudo apt install libomp-dev
// gcc flag: -fopenmp
// link with gomp library


int main() {

	omp_set_num_threads(2);
	#pragma omp parallel
	{
	    printf("thread %d\n", omp_get_thread_num());
	}

	return 0;
}

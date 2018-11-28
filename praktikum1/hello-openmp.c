#include <stdio.h>
#include <omp.h>
int main ( int argc, char *argv[] ) {
	printf("Exercise 1\n");

	// get maximal number of threads
	int numMaxThreads = omp_get_max_threads();
	printf("The number of maximal threads is %i\n",numMaxThreads);

	// set own number of threads
	int ownNumThreads = 4;
	omp_set_num_threads(ownNumThreads);

	// create team of threads printing Hello World and its identifier
	#pragma omp parallel
	{
		int threadId = omp_get_thread_num();
		printf("Hello %i\n",threadId);
	}
}

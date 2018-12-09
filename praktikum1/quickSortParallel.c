#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int partition(int* A, int l, int r)
{
	int p = A[l], i = l - 1, j = r + 1;
	int tmp;
	do {
		do i++; while (A[i] < p);
		do j--; while (A[j] > p);
		if (i < j) {
			tmp = A[i];
			A[i] = A[j];
			A[j] = tmp;
		}
	} while (i < j);
	return j;
}

void quicksortTask(int* A, int lo, int hi) {
	if (lo < hi) {
		int p = partition(A, lo, hi);
		#pragma omp task final (p - lo + 1 <= 100)
			quicksortTask(A, lo, p);
		#pragma omp task final (hi - p <= 100)
			quicksortTask(A, p + 1, hi);
		#pragma omp taskwait
	}
}



int main( int argc, char** argv )
{
	int sortedCorrectly = 1; // 1 = true, 0 = false
    if( argc < 2 )
    {
        printf( "Usage: %s <array length>\n", argv[0] );
        return 1;
    }

    
	// Read in number of elements
	int nrOfElements = atoi(argv[1]);
	srand(14811);
	
	// Allocate array
	int* elements;
	elements = (int *)malloc(nrOfElements * sizeof(int));

	// Initialize array
	for (int i = 0; i < nrOfElements; i++) {
		elements[i] = rand();
		//printf("%d", elements[i]);
		//printf("\n");
	}

	// RUN QUICKSORT
	clock_t begin = clock(); //start timer
	#pragma omp parallel
		#pragma omp single 
			quicksortTask(elements, 0, nrOfElements - 1);

	// Time the execution
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	// Verify sorted order

	//check until second to last element
	for (int i = 0; i < nrOfElements - 1; i++) {
		if (elements[i] > elements[i + 1])
			sortedCorrectly = 0;
	}
	//check last element
	if (elements[nrOfElements - 2] > elements[nrOfElements - 1])
		sortedCorrectly = 0;

	printf("Sorted correctly? 1 = y, 0 = n: ");
	printf("%d", sortedCorrectly);
	printf("\n");
	printf("Time for sorting only: ");
	printf("%f", time_spent);
	printf(" s");

    return 0;
}


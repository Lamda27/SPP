#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int partition(int* A, int l, int r)
{
	int p = A[l], i = l - 1, j = r + 1;
	int tmp;
	do {
		//find element right from pivot which is smaller than pivot
		do {
			i++;
		} while (A[i] < p);
		//find element left from pivot which is bigger than pivot
		do {
			j--;
		} while (A[j] > p);
		//swap elements
		if (i < j) {
			tmp = A[i];
			A[i] = A[j];
			A[j] = tmp;
		}
	} while (i < j);
	//return new pivot position
	return j;
}

void quicksortTask(int* A, int left, int right) {
	if (left < right) {
		int p = partition(A, left, right);

		//recursion
		//no new tasks created for less than 100 elements
		#pragma omp task final (p - left + 1 <= 100)
			quicksortTask(A, left, p);
		#pragma omp task final (right - p <= 100)
			quicksortTask(A, p + 1, right);
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
	}

	// RUN QUICKSORT
	//start timer
	double startTime = omp_get_wtime();
	#pragma omp parallel
		#pragma omp single 
			quicksortTask(elements, 0, nrOfElements - 1);

	// Time the execution
	double endTime = omp_get_wtime();
	double time_spent = endTime - startTime;

	// Verify sorted order
	//check until second to last element
	for (int i = 0; i < nrOfElements - 1; i++) {
		if (elements[i] > elements[i + 1])
			sortedCorrectly = 0;
	}
	//check last element
	if (elements[nrOfElements - 2] > elements[nrOfElements - 1])
		sortedCorrectly = 0;
	
	//free allocated memory
	free(elements);
	
	printf("Sorted correctly? 1 = y, 0 = n: ");
	printf("%d", sortedCorrectly);
	printf("\n");
	printf("Time for sorting only: ");
	printf("%f", time_spent);
	printf(" s");

    return 0;
}


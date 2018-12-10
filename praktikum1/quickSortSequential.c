#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// This function partitions array A into two subarrays A_1 and A_2
// Input:
//     *l is the index of the first element in array A
//     *r is the index of the last element in array A
//
//     A
//     [  |  |  |  |  ...  |  |  |  |  ]
//      *l                           *r
//
// Output:
//     *l is now the index of the first element of array A_2, which still needs to be sorted
//     *r is now the index of the last  element of array A_1, which still needs to be sorted
//
//     A_1              A_2
//     [  |  | ... |  ] [  |  | ... |  ]
//                  *r   *l
void partition(int* A, int* l, int* r)
{
	int p = A[*r]; // choose last element as pivot
	int i, tmp;
	int j = *l - 1;

	//find elements to swap
	for (i = *l; i < *r; i++) {
		//swap elements
		if (A[i] < p) {
			j++;
			tmp = A[j];
			A[j] = A[i];
			A[i] = tmp;
		}
	}
	j++;
	tmp = A[j];
	A[j] = p;
	A[*r] = tmp;
	*r = j - 1;
	*l = j + 1;
}

// Input:
//     l is the index of the first element in array A
//     r is the index of the last element in array A
//
//     A
//     [  |  |  |  |  ...  |  |  |  |  ]
//      l                            r
void quicksort(int* A, int l, int r)
{
	int first = l, last = r;
	if (l < r) {
		partition(A, &l, &r);

		//recursion
		quicksort(A, first, r);
		quicksort(A, l, last);
	}
}

int main(int argc, char** argv)
{
	int sortedCorrectly = 1; // 1 = true, 0 = false
	if (argc < 2)
	{
		printf("Usage: %s <array length>\n", argv[0]);
		return 1;
	}
	// Read number of elements
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
	clock_t begin = clock(); //start timer
	quicksort(elements, 0, nrOfElements - 1);

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
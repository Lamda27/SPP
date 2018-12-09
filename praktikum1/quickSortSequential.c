#include <stdio.h>
#include <stdlib.h>

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
void partition ( int* A, int* l, int* r )
{
	int p = A[*r]; // choose last element as pivot

	printf("pivot is ");
	printf("%d", p);
	printf("\n");

	int i, tmp;
	int j = *l-1;

	for(i = *l; i < *r; i++) {
		if(A[i] < p) {
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
	*r = j-1;
	*l = j+1;
}

// Input:
//     l is the index of the first element in array A
//     r is the index of the last element in array A
//
//     A
//     [  |  |  |  |  ...  |  |  |  |  ]
//      l                            r
void quicksort( int* A, int l, int r )
{
	int first = l, last = r;
	//quicksort(A,first,last);
	if (l < r) {
		partition(A, &l, &r); // sollte Werte f�r l und r �ndern
		printf("l is ");
		printf("%d", l);
		printf("\n");
		printf("r is ");
		printf("%d", r);
		printf("\n");
		//HIER LIEGT PROBLEM NR 2, da sich die Werte f�r r und l nicht �ndern
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
    srand( 14811 );

    // Allocate array
	int* elements;
	elements = (int *)malloc(nrOfElements * sizeof(int));

	printf("Elements before sorting \n");
    // Initialize array
	for (int i = 0; i < nrOfElements; i++) {
		elements[i] = rand();
		printf("%d", elements[i]);
		printf("\n");
	}

	// RUN QUICKSORT
	quicksort(elements, 0, nrOfElements - 1);

	//show elements after sorting
	printf("\n");
	printf("\n");
	printf("\n");
	printf("Elements after sorting \n");
	for (int i = 0; i < nrOfElements; i++) {
		printf("%d", elements[i]);
		printf("\n");
	}

    // Time the execution
	// TODO

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
    return 0;
}

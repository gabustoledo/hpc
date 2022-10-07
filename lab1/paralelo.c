#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

#define N 1024


int main() {

	int *A = (int *) malloc(N*sizeof(int));

	for (int i = 0; i < N; i++){
		A[i] = (rand() % (66666 - 0 + 1)) + 0;
	}

	int gmax = -1;
	#pragma omp parallel shared(gmax) num_threads(4)
	{
		int max = -1; 		//variable local para cada hebra

    	#pragma omp for schedule(static, 4)
			for (int i=0; i < N; i++){
				if (A[i] > max) max = A[i];
			}

		#pragma omp critical //proveer exclusión mutua
			if (max > gmax)
				gmax = max;
	}


	printf("omp max = %d\n", gmax);

	// check with sequential code
	gmax = -1;
	for (int i=0; i < N; i++)
		if (A[i] > gmax) gmax = A[i];
	printf("sequential max = %d\n", gmax);
	exit(0);
}
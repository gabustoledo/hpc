#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1024
#define M 16
#define MINLENGTH 64

int Max;
int  max(int *A, int length, int init) {
    int h1, h2, finalMax;

    if (length <= MINLENGTH) {
        int max = -1;
		    //printf("%d doing %d, length = %d\n", omp_get_thread_num(), init, length);

        for (int i=init; i < init+length; i++){
      		if (A[i] > max) {
      			max = A[i];
      		}
      	}

        return(max);
    } else {
    	length = length/2;
    	int i1 = init;
    	int i2 = i1 + length;



    	#pragma omp task shared(h1) firstprivate(length, i1)
        h1 = max(A, length, i1);
        printf("%i \n", h1);
    	#pragma omp task shared(h2) firstprivate(length, i2)
        h2 = max(A, length, i2);
        printf("%i \n", h2);


    	#pragma omp taskwait /// --Cada hebra espera las tareas que creó, actualizando su valor máximo
		{
        	if(h1 > h2){
            finalMax  = h1;
          }
          else{
            finalMax = h2;
          }

    	}
    	return(finalMax);
	}
}


int main() {

	int n = 1024;
	//hist = malloc(M*sizeof(int));
	//for (int i=0; i < M; i++)
	//	hist[i] = 0;

	int *A = malloc(N*sizeof(int));
	for (int i = 0; i < N; i++)
		A[i] = (rand() % (66666 - 0 + 1)) + 0;

    //for (int i=0; i < N; i++)
	//	printf("%d\n", A[i]);

    omp_set_nested(1);

	#pragma omp parallel shared(n) num_threads(2)
	{
		#pragma omp single
		Max = max(A, n, 0);
	}


	printf("omp el max es %d\n", Max);

	// check with sequential
  int maxAux = -1;
  for (int i = 0; i < N; i++) {
		if (A[i] > maxAux) {
			maxAux = A[i];
		}
	}

		  printf("el max es %d\n", maxAux);

	exit(0);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 256
#define T 1000
#define C 1.0
#define DT 0.1
#define DD 2.0
#define ITER 100

int main(){

	
	float *H = malloc(N*N*sizeof(float));   // H actual
	float *H_1 = malloc(N*N*sizeof(float));	// H en t-1
	float *H_2 = malloc(N*N*sizeof(float));	// H en t-2

	// Inicializar en 0 toda H
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			H[i*N + j] = 0;
		}		
	}

	// Inicializar la ola	
	int limiteInf = 0.4*N;
	int limiteSup = 0.6*N;
	for (int i = limiteInf; i < limiteSup; i++){
		for (int j = limiteInf; j < limiteSup; j++){
			H[i*N + j] = 20;
		}		
	}

	// guardar ola en H_1
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			H_1[i * N + j] = H[i*N + j];
		}		
	}

	// // Ola en t=1
	// for (int i = 0; i < N; i++){
	// 	for (int j = 0; j < N; j++){
	// 		if(i==0 || j==0 || i==N-1 || j==N-1){
	// 			H[i*N + j] = 0;
	// 		}else{
	// 			H[i*N + j] = H_1[i*N + j] + ((C*C)/2.0)*((DT*DT)/(DD*DD))*(H_1[(i+1)*N + j] + H_1[(i-1)*N + j] + H_1[i*N + (j-1)] + H_1[i*N + (j+1)] - 4*H_1[i*N + j]);
	// 		}
	// 	}		
	// }

	// // guardar ola en t-1 y t-2
	// for (int i = 0; i < N; i++){
	// 	for (int j = 0; j < N; j++){
	// 		H_2[i * N + j] = H_1[i*N + j];
	// 		H_1[i * N + j] = H[i*N + j];
	// 	}
	// }

	int iterAux = 0;
	char nombreArchivo[] = "ola_";
	for (int k = 0; k < T; k++){

		if(k==0){
			for (int i = 0; i < N; i++){
				for (int j = 0; j < N; j++){
					if(i==0 || j==0 || i==N-1 || j==N-1){
						H[i*N + j] = 0;
					}else{
						H[i*N + j] = H_1[i*N + j] + ((C*C)/2.0)*((DT*DT)/(DD*DD))*(H_1[(i+1)*N + j] + H_1[(i-1)*N + j] + H_1[i*N + (j-1)] + H_1[i*N + (j+1)] - 4*H_1[i*N + j]);
					}
				}
			}
		}else{
			for (int i = 0; i < N; i++){
				for (int j = 0; j < N; j++){
					if(i==0 || j==0 || i==N-1 || j==N-1){
						H[i*N + j] = 0;
					}else{
						H[i*N + j] = 2*H_1[i*N + j] - H_2[i*N + j] + (C*C)*((DT*DT)/(DD*DD))*(H_1[(i+1)*N + j] + H_1[(i-1)*N + j] + H_1[i*N + (j-1)] + H_1[i*N + (j+1)] - 4*H_1[i*N + j]);
					}
				}		
			}
		}

		// Se guarda t-1 y t-2
		for (int i = 0; i < N; i++){
			for (int j = 0; j < N; j++){
				H_2[i * N + j] = H_1[i*N + j];
				H_1[i * N + j] = H[i*N + j];
			}
		}

		int mod = k%ITER;
		if(mod==0){
			char iter[10];
			sprintf(iter, "%d", iterAux);

			char nombreAux[50];
			strcpy(nombreAux,nombreArchivo);
			strcat(nombreAux,iter);
			strcat(nombreAux,".raw");

			printf("%s", nombreAux);

			FILE *f = fopen(nombreAux,"w");
			fwrite(H,sizeof(float),N*N,f);
			fclose(f);
			// printf("hay que guardar");
			iterAux += 1;
		}
	}
	

	// Escritura del archivo
	FILE *f = fopen("ejemplo.raw","w");
	fwrite(H,sizeof(float),N*N,f);
	fclose(f);
	return 0;
}
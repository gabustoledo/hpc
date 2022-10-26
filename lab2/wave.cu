/*

	High Performance Computing
	Lab2

	Gabriel Bustamante
	Nicolas Zelada

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define C 1.0
#define DT 0.1
#define DD 2.0
#define F 20

// ./wave -N 256 -x 2 -y 2 -T 1001 -f salidas/aqui.raw

void olaInicial(float *H, float *H_1, int n){
	for (int i = 0; i < n; i++){ // Esta
		for (int j = 0; j < n; j++){
			if(i==0 || j==0 || i==n-1 || j==n-1){
				H[i*n + j] = 0;
			}else{
				H[i*n + j] = H_1[i*n + j] + ((C*C)/2.0)*((DT*DT)/(DD*DD))*(H_1[(i+1)*n + j] + H_1[(i-1)*n + j] + H_1[i*n + (j-1)] + H_1[i*n + (j+1)] - 4*H_1[i*n + j]);
			}
		}
	}
}

void ola(float *H, float *H_1, float *H_2, int n){
	for (int i = 0; i < n; i++){ // Esta
		for (int j = 0; j < n; j++){
			if(i==0 || j==0 || i==n-1 || j==n-1){
				H[i*n + j] = 0;
			}else{
				H[i*n + j] = 2*H_1[i*n + j] - H_2[i*n + j] + (C*C)*((DT*DT)/(DD*DD))*(H_1[(i+1)*n + j] + H_1[(i-1)*n + j] + H_1[i*n + (j-1)] + H_1[i*n + (j+1)] - 4*H_1[i*n + j]);
			}
		}
	}
}

int main(int argc, char **argv){

	char *nombreArchivo = NULL;
	int n = 0;
	int iteraciones = 0;
	int gridSize = 0; //x
	int blockSize = 0; //y

	// Parametros de entrada
	int c;
	while (((c = getopt(argc, argv, "N:x:y:T:f:")) != -1)){
		switch (c){
		case 'N': // tamano grilla
			n = atof(optarg);
			break;
		case 'x': // x
			gridSize = atof(optarg);
			break;
		case 'y': // y
			blockSize = atof(optarg);
			break;
		case 'T': // numero de pasos
			iteraciones = atof(optarg);
			break;
		case 'f': // archivo salida
			nombreArchivo = optarg;
			break;
		}
	}

	// Se reserva memoria para las variables del host
	float *H = malloc(n*n*sizeof(float));   // H actual
	float *H_1 = malloc(n*n*sizeof(float));	// H en t-1
	float *H_2 = malloc(n*n*sizeof(float));	// H en t-2

	// Se reserva memoria para las variables del device
	float *D_H;
	float *D_H_1;
	float *D_H_2;
	cudaMallocManaged(&D_H, n*n*sizeof(float));
	cudaMallocManaged(&D_H_1, n*n*sizeof(float));
	cudaMallocManaged(&D_H_2, n*n*sizeof(float));

	int limiteInf = 0.4*n;	// limite inferior para inicio de la ola
	int limiteSup = 0.6*n;	// limite superior para inicio de la ola
	int iterAux = 0;

	// Inicializar en 0 o F segun corresponda toda H
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			if(i>=limiteInf && i<=limiteSup && j>=limiteInf && j<=limiteSup){
				H_1[i*n + j] = F;
			}else{
				H_1[i*n + j] = 0;
			}
		}
	}

	// Copia de vectores del host al device
	cudaMemcpy(D_H_1, H_1, n*n*sizeof(float), cudaMemcpyHostToDevice);

	// Inicio de 
	for (int k = 0; k < iteraciones; k++){
		if(k==0){
			olaInicial<<<gridSize, blockSize>>>(D_H,D_H_1,n);
			cudaDeviceSynchronize();
		}else{
			ola<<<gridSize, blockSize>>>(D_H,D_H_1,D_H_2,n);
			cudaDeviceSynchronize();
		}

		// copiar todo device al host

		// Se guarda t-1 y t-2
		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++){
				D_H_2[i * n + j] = D_H_1[i*n + j];
				D_H_1[i * n + j] = D_H[i*n + j];
			}
		}

		// copiar host al device
	}

	FILE *f = fopen(nombreArchivo,"w");
	fwrite(H,sizeof(float),n*n,f);
	fclose(f);

	// Liberacion de memoria
	cudaFree(H);
	cudaFree(H_1);
	cudaFree(H_2);

	return 0;
}
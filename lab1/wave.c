#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define C 1.0
#define DT 0.1
#define DD 2.0
#define F 20

// ./wave -N 256 -T 10001 -H 2 -f salidas/aqui_ -t 100

int main(int argc, char **argv){
 
	// char nombreArchivo[] = "salidas/ola_";
	char *nombreArchivo = NULL;
	int n = 0;
	int iteraciones = 0;
	int hebras = 0;
	int diferencia = 0;

	// Parametros de entrada
	int c;
	while (((c = getopt(argc, argv, "N:T:H:f:t:;")) != -1)){
		switch (c){
		case 'N': // tamano grilla
			n = atof(optarg);
			break;
		case 'T': // numero de pasos
			iteraciones = atof(optarg);
			break;
		case 'H': // numero de hebras
			hebras = atof(optarg);
			break;
		case 'f': // archivo salida
			nombreArchivo = optarg;
			break;
		case 't': // iteraciones
			diferencia = atof(optarg);
			break;
		}
	}

	// Inicializacion de variables
	float *H = malloc(n*n*sizeof(float));   // H actual
	float *H_1 = malloc(n*n*sizeof(float));	// H en t-1
	float *H_2 = malloc(n*n*sizeof(float));	// H en t-2
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

	// Ciclo para la ola
	for (int k = 0; k < iteraciones; k++){

		if(k==0){
			for (int i = 0; i < n; i++){
				for (int j = 0; j < n; j++){
					if(i==0 || j==0 || i==n-1 || j==n-1){
						H[i*n + j] = 0;
					}else{
						H[i*n + j] = H_1[i*n + j] + ((C*C)/2.0)*((DT*DT)/(DD*DD))*(H_1[(i+1)*n + j] + H_1[(i-1)*n + j] + H_1[i*n + (j-1)] + H_1[i*n + (j+1)] - 4*H_1[i*n + j]);
					}
				}
			}
		}else{
			for (int i = 0; i < n; i++){
				for (int j = 0; j < n; j++){
					if(i==0 || j==0 || i==n-1 || j==n-1){
						H[i*n + j] = 0;
					}else{
						H[i*n + j] = 2*H_1[i*n + j] - H_2[i*n + j] + (C*C)*((DT*DT)/(DD*DD))*(H_1[(i+1)*n + j] + H_1[(i-1)*n + j] + H_1[i*n + (j-1)] + H_1[i*n + (j+1)] - 4*H_1[i*n + j]);
					}
				}		
			}
		}

		// Se guarda t-1 y t-2
		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++){
				H_2[i * n + j] = H_1[i*n + j];
				H_1[i * n + j] = H[i*n + j];
			}
		}

		// Escritura del archivo en caso de que corresponda
		int mod = k%diferencia;
		if(mod==0){
			char iter[10];
			sprintf(iter, "%d", iterAux);

			char nombreAux[50];
			strcpy(nombreAux,nombreArchivo);
			strcat(nombreAux,iter);
			strcat(nombreAux,".raw");

			FILE *f = fopen(nombreAux,"w");
			fwrite(H,sizeof(float),n*n,f);
			fclose(f);
			iterAux += 1;
		}
	}

	return 0;
}
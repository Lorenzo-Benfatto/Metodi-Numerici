// A program to rebin with correlations a gaussian Metropolis

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

int main(void){
	FILE *f,*g;
	unsigned long s;
	int i=0,j,k,N=100000,M=1000,x,l=4;
	float a;
	int b,c;
	float *dati = malloc(sizeof(long double) * N);
//	int* matrice = malloc((M * N) * sizeof(long double));
	f=fopen("metrogauss.txt","r");
	if(f==NULL){
	 perror("File non correttamente aperto");
	 exit(1);
	}
	g=fopen("resample_binned.txt","w");
	if(g==NULL){
	 perror("File non correttamente aperto");
	 exit(2);
	}
	srand(time(NULL));
	x=fscanf(f, "%f  %d  %d", &a, &b, &c);
	while (x!=EOF){
			dati[i]=a;
			x=fscanf(f, "%f  %d  %d", &a, &b, &c);
			i++;
		}
	for(j=0;j<M;j++){
		for (k=0;k<N/(l+1);k++){
			s = rand();
			s <<= 15;
			s ^= rand();
			s %= N+1;
			for(int h=0;h<l;h++){
				if(s+h>N-1) fprintf(g,"%lf ",dati[s+h+1-N]);
				if(s+h<=N-1) fprintf(g,"%lf ",dati[s+h]);
			}
		}
		fprintf(g,"\n");
	}

	free(dati);
//	free(matrice);
	fclose(f);
	fclose(g);
	printf("Arrivederci \n");
	return 0;
}

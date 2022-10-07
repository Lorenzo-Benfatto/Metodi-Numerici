#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

int main(void){
	FILE *f,*g;
	int N=10000000, i=0, j, b, x, z, s=5, k, maxk=5000,taglio=500000;
	float a,C=0,mean=0;
	float *dati = malloc(sizeof(float) * N);
	f=fopen("metrogauss.txt","r");
	if(f==NULL){
	 perror("File non correttamente aperto");
	 exit(1);
	}
	g=fopen("Cakka.txt","w");
	if(g==NULL){
	 perror("File non correttamente aperto");
	 exit(2);
	}
	x=fscanf(f, "%f  %d", &a, &b);
	while (x!=EOF){
			dati[i]=a;
			x=fscanf(f, "%f  %d", &a, &b);
			i++;
		}
		for(z=taglio;z<N;z++){
			mean=dati[z]+mean;
		}
		mean=mean/(N-taglio);
		for(k=1;k<maxk;k++){
			for(j=taglio;j<N-k*s;j++){
				C=(dati[j]-mean)*(dati[j+k*s]-mean)+C;
			}
			C=C/(N-taglio-k*s);
			fprintf(g,"%f  %d \n",C,k*s);
		}
	free(dati);
	fclose(f);
	fclose(g);
	return 0;
	}
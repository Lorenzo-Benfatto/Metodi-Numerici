#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

int main(void){
	FILE *f,*g;
	unsigned long s;
	int i=0,j,k,N=1000,M=100,x,l=4,taglio=1;
	int V=N-taglio;
	float mag, ene;
	int iter;
	float *dati = malloc(sizeof(long double) * (N-taglio));
	f=fopen("misure.txt","r");
	if(f==NULL){
	 perror("Testa di cazzo manca il file da leggere");
	 exit(1);
	}
	g=fopen("resample_binned.txt","w");
	if(g==NULL){
	 perror("Non riesco a scrivere coglione");
	 exit(2);
	}
	srand(time(NULL));
	x=fscanf(f, "%f  %f  %d", &mag, &ene, &iter);
	while (x!=EOF){
			dati[i]=ene;
			x=fscanf(f, "%f  %f  %d", &mag, &ene, &iter);
			i++;
		}
	for(j=0;j<M;j++){
		for (k=taglio;k<N/(l+1);k++){
			s = rand();
			s <<= 15;
			s ^= rand();
			s %= V+1;
			s=s+taglio;
			printf("%ld \n",s);
			for(int h=0;h<l+1;h++){
				if(s+h>N-1){
				 fprintf(g,"%lf ",dati[s+h+1-N]);
				 printf("sono dentro il primo if");
				}
				if(s+h<=N-1){
				 fprintf(g,"%lf ",dati[s+h]);
				 printf("sono dentro il secondo if");
				}

			}
		}
		fprintf(g,"\n");
	}

	free(dati);
	fclose(f);
	fclose(g);
	printf("Arrivederci \n");
	return 0;
}

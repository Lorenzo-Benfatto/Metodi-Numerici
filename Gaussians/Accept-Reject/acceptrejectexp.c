// A program that simulates a Gaussian distribution via an Accept-Reject algorithm. 
// It's supposed to be ran after having created the file "gaussian.txt" using the campionamentoexp.c program.

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

int main(void){
	int N=1000000,M=100000000;										
	float r,gamma=10,sgamma;
	double x,a,b,p,q;
	sgamma=1/gamma;
	srand(time(NULL));
	FILE*f,*g;
	f=fopen("gaussian.txt","r");
	if(f==NULL){
	 perror("File di merda");
	 exit(1);
	};
	g=fopen("acceptreject.txt","w");
	if(g==NULL){
	 perror("File di merda");
	 exit(2);
	};
	x=fscanf(f,"%lf %lf",&a,&b); 
	while(x!=EOF){
		r=rand()%M;
		r=(r/M)*2-1;
		q=1-pow(sgamma*log(b*sgamma),2);
		p=sqrt(q)*b;
		if(r<p) {
			fprintf(g,"%f %f \n",r,p);
		}
		x=fscanf(f,"%lf %lf",&a,&b);
	}
	fclose(f);
	fclose(g);
	return 0;
}

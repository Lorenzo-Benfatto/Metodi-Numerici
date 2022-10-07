#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

int main(void) {
	int n=1000000,i,N=1000000;
	float x,y;
	double r,phi,a,b;
	FILE*f;
	f=fopen("gaussian.txt","w");
	if(f==NULL){
	 perror("File di merda");
	 exit(1);
	}
	srand(time(NULL));
	for(i=0;i<N;i++){
		x=rand()%n;
		x=x/n;
		y=rand()%n;
		y=y/n;
		r=sqrt(-log(1-x));
		phi=2*M_PI*y;
		a=r*cos(phi);
		b=r*sin(phi);
		fprintf(f,"%f  %f \n", a,b);
	}
	fclose(f);
	return 0;
}

// A program that simulates a gaussian distribution via a Metropolis Monte-Carlo algorithm.

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

int main(void){
	int a, start=0, N=10000000, M=100000;
	float mu=5, sigma2=1, d=0.01;
	float xp, y, r, p, q; //y è il xk+1
	FILE *f;
	f=fopen("metrogauss.txt","w");
	if(f==NULL){
	 perror("File non correttamente aperto");
	 exit(1);
	}
	srand(time(NULL));

	y=start;
	for (int i = 0; i < N; i++){
		xp=rand()%(M);
		xp=xp/M;
		xp=y+d*(1-2*xp);
		r=1;
		p=exp((-pow((xp-mu),2)+pow((y-mu),2))/(2*sigma2));
		if(r>p){
			r=p;
		}
		q=rand()%(M);
		q=q/M;
		a=0;
		if(q<r){
			y=xp;
			a=1;
		}
		fprintf(f, "%f  %d  %d \n", y,i,a);
	}
	fclose(f);
	return 0;
}

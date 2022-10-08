#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

int main(void){
	int i,N=1000000,M=1000000;
	float x,y,gamma=10;
	srand(time(NULL));
	FILE*f;
	f=fopen("gaussian.txt","w");
	if(f==NULL){
	 perror("File di merda");
	 exit(1);
	}
	for(i=0;i<N;i++){
		x=rand()%M;
		x=x/M;
		y=-log(1-x)/gamma;
		fprintf(f,"%f  %f \n", x,y);
	}
fclose(f);
return 0;
}

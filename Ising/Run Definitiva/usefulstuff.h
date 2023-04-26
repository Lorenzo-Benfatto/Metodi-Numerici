#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>


//funzione che restituisce un numero random tra 0 e 1 prendendo come argomento il numero di cifre decimali desiderate
float rand_01(float r, int M){
	srand(time(NULL));
	r=rand()%(M+1);
	r=r/M;
	return r;
}

//funzione per controllare la corretta apertura di un file. Ha per argomenti il puntatore al file e il "numero" di file per exit() 
void control_file(FILE*f){
	if(f==NULL){
		printf("NOOOOOOOOOOOOOOOOOO BUONOOOOOOOOOOOOOO\n");
		exit(-1);
	}
	return ;
}

//funzione per calcolare la media di un array a di cui passo indirizzo puntatore e la lunghezza
float mean(float *a, int lung){
	float sum=0;
	for(int i=0; i<lung; i++){
		sum=sum+a[i];
	}
	sum=sum/lung;
	return sum;
}



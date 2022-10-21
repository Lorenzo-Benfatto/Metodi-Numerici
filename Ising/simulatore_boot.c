#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"listfunction.h"
#include"simulazione_boot.h"

/* IN QUESTO CODICE VIENE ESEGUITA LA SIMULAZIONE MONTECARLO PER VARI VALORI DI BETA, RIPORTATI NEL FILE "beta.txt". PER OGNI VALORE VIENE SALVATA LA CONFIGURAZIONE FINALE DEL RETICOLO NEI FILE "betai.txt" E NEL FILE "medie.txt" SONO RIPORTATE LE MEDIE DELLA MAGNETIZZAZIONE E DELL'ENERGIA PER CIASCUN VALORE DI BETA */

int main(void){
	FILE *f,*g;         // puntatori ai file dei beta e delle medie
	int p=0, L;         // p serve per scorrere il file dei beta e L è la lunghezza dell'array dei beta
    
	f=fopen("beta.txt","r");
	if(f==NULL){
	 perror("File di merda");
	 exit(1);
	}
	g=fopen("medie.txt","w");
	if(g==NULL){
	 perror("File di merda");
	 exit(1);
	}
	list *b=NULL;           // lista contenente i valori dei beta, letti da file
	b = scan_file(f,b);         // lettura dei valori dei beta, con funzione ("listfunction.h")
	L=count(b);         // funzione che conta la lunghezza di una lista ("listfunction.h")
	FILE *beta[L], *misure[L];          // puntatore per i file che salvano la configurazione, per ciascun beta
	while(b!=NULL){
        char filemisure[20];
        sprintf(filemisure, "misure%.3f.txt", b->val); //it modifies each time the name of the file to be created
        misure[p]=fopen(filemisure, "w");
        if(misure[p]==NULL){ //check if the file is opened correctly
            perror("File non correttamente aperto");
            exit(p);
            }
		simulazione(b->val, misure[p]);
		fprintf(g," %f %f %f \n",medie[0],medie[1],b->val);
       	char filename[20];
		sprintf(filename, "beta%d.txt", p); //it modifies each time the name of the file to be created
      	beta[p]=fopen(filename, "w");
       	if(beta[p]==NULL){ //check if the file is opened correctly
       	    perror("File non correttamente aperto");
       	    exit(p);
       	    }
       	for(int j=0; j<nlatt; j++){
       	    for(int k=0; k<nlatt; k++){
       	        fprintf(beta[p], "%d ", field[j][k]);
        	}
        	fprintf(beta[p],"\n");
      	}
     	fclose(beta[p]);
        fclose(misure[p]);
     	printf("ho fatto il beta numero %d, stai sciallo \n", p); 
		b=b->next;
		p++;
	}
	printf("Tutto a posto \n");
	fclose(f);
	fclose(g);
	return 0;
}

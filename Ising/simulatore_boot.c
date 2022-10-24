#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"listfunction.h"
#include"simulazione_boot.h"
#include "usefulstuff.h"


/* IN QUESTO CODICE VIENE ESEGUITA LA SIMULAZIONE MONTECARLO PER VARI VALORI DI BETA, RIPORTATI NEL FILE "beta.txt". 
PER OGNI VALORE VIENE SALVATA LA CONFIGURAZIONE FINALE DEL RETICOLO NEI FILE "betai.txt" E NEL FILE "medie.txt"
SONO RIPORTATE LE MEDIE DELLA MAGNETIZZAZIONE E DELL'ENERGIA PER CIASCUN VALORE DI BETA */

int main(void){
	FILE *f,*g;         // puntatori ai file dei beta e delle medie
	int p=0, L, s=0;         // p serve per scorrere il file dei beta e L è la lunghezza dell'array dei beta. s serve a scorrere i file di beta di cui voglio stampare il lattice
	float beta_interesting[3]={0.395,0.44,0.5};   //array contenente i beta di cui voglio vedere il lattice. Assicurarsi che i valori inseriti siano presenti nel file beta_init.txt

	f=fopen("beta.txt","r");
	control_file(f);
    char filemedie[200];
    sprintf(filemedie, "/mnt/c/Users/aministratore/Documenti/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Bootstrap/Nuove_versioni/Nlatt = %d/medie.txt",N);
	g=fopen(filemedie,"w");

	printf("Odio i gay \n");

	control_file(g);

	list *b=NULL;           // lista contenente i valori dei beta, letti da file
	b = scan_file(f,b);         // lettura dei valori dei beta, con funzione ("listfunction.h")
	L=count(b);         // funzione che conta la lunghezza di una lista ("listfunction.h")
	FILE *beta[3], *misure[L];          // puntatore per i file che salvano la configurazione, per ciascun beta
	while(b!=NULL){
        char filemisure[200];
        sprintf(filemisure, "/mnt/c/Users/aministratore/Documenti/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Bootstrap/Nuove_versioni/Nlatt = %d/Risultati/misure%.3f.txt", N, b->val); //it modifies each time the name of the file to be created
        misure[p]=fopen(filemisure, "w");

        printf("Odio i neri \n");

        control_file(misure[p]);

		simulazione(b->val, misure[p]); /*eseguo la simulazione per il valore di beta in causa. Funzione di simulazione_boot.h
		Mi creerà automaticamente un file di misure contenente valore di magnetizz, energia e beta associato 
		per ogni misura fatta*/
		fprintf(g," %f %f %f \n",medie[0],medie[1],b->val);  //stampo medie di magn e energia con beta associato in file medie.txt
		
		if (b->val==beta_interesting[0]||b->val==beta_interesting[1]||b->val==beta_interesting[2]){ //faccio i file contenenti i lattice interessanti

			printf("%f\n", b->val);
	       	char filename[200];
			sprintf(filename, "/mnt/c/Users/aministratore/Documenti/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Bootstrap/Nuove_versioni/Nlatt = %d/Lattice/beta%.3f.txt", N, b->val); //it modifies each time the name of the file to be created
	      	beta[s]=fopen(filename, "w");

	      	printf("Odio i rom \n");

	       	control_file(beta[s]);

	       	for(int j=0; j<nlatt; j++){
	       	    for(int k=0; k<nlatt; k++){
	       	        fprintf(beta[s], "%d ", field[j][k]);
	        	}
	        	fprintf(beta[s],"\n");
	      	}
	     	fclose(beta[s]);
	     	s++;
      	}
     	
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
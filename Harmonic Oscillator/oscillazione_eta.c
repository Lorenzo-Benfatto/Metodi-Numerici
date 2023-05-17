#include"/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/oscillazione.h"


/* IN QUESTO CODICE VIENE ESEGUITA LA SIMULAZIONE MONTECARLO PER VARI VALORI DI eta, RIPORTATI NEL FILE "eta.txt". 
PER OGNI VALORE VIENE SALVATA LA CONFIGURAZIONE FINALE DEL RETICOLO NEI FILE "eta{i}.txt" E NEL FILE "medie.txt"
SONO RIPORTATE LE MEDIE DELLA MAGNETIZZAZIONE E DELL'ENERGIA PER CIASCUN VALORE DI eta */

/*La lunghezza del reticolo devve essere specificata nel file oscillazione.h,
		mentre il numero di misure nel file input.txt, che verrà letto in oscillazione.h*/

int main(void){
	FILE *f,*g;         // puntatori ai file dei eta e delle medie
	int p=0, L, s=0;         // p serve per scorrere il file dei eta e L è la lunghezza dell'array dei eta. s serve a scorrere i file di eta di cui voglio stampare il lattice
	float eta_interesting[3]={0,0,0};   //array contenente i eta di cui voglio vedere il lattice. Assicurarsi che i valori inseriti siano presenti nel file eta.txt

	f=fopen("/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/eta.txt","r");
	control_file(f);


	list *eta=NULL;           // lista contenente i valori dei eta, letti da file
	eta = scan_file(f,eta);         // lettura dei valori dei eta, con funzione ("listfunction.h")
	L=count(eta);         // funzione che conta la lunghezza di una lista ("listfunction.h")
	FILE *etan[3], *misure[L];          // puntatore per i file che salvano la configurazione, per ciascun eta
	while(eta!=NULL){
        char filemisure[200];
        sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/Eta/misure%.1f.txt", eta->val); //it modifies each time the name of the file to etae created
        misure[p]=fopen(filemisure, "w");
        control_file(misure[p]);

		Harmonic_metropolis(eta->val, misure[p]); /*eseguo la simulazione per il valore di eta in causa. Funzione di oscillazione.h
		Mi creerà automaticamente un file di misure contenente valore di  energia e eta associato 
		per ogni misura fatta. */
				
		if (eta->val==eta_interesting[0]||eta->val==eta_interesting[1]||eta->val==eta_interesting[2]){ //faccio i file contenenti i lattice interessanti

			printf("%f\n", eta->val);
	       	char filename[200];
			sprintf(filename, "/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/Accettanza/eta%.1f.txt", eta->val); //it modifies each time the name of the file to etae created
	      	etan[s]=fopen(filename, "w");
	      	control_file(etan[s]);

	     	fclose(etan[s]);
	     	s++;
      	}
     	
        fclose(misure[p]);
     	printf("ho fatto il etan numero %d, stai sciallo \n", p); 
		eta=eta->next;
		p++;
	}
	printf("Tutto a posto \n");
	fclose(f);
	fclose(g);
	return 0;
}
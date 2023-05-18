#include"/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/oscillazione.h"


/* IN QUESTO CODICE VIENE ESEGUITA LA SIMULAZIONE MONTECARLO PER VARI VALORI DI eta, RIPORTATI NEL FILE "eta.txt". 
PER OGNI VALORE NEL FILE "misure%.1f.txt" SONO RIPORTATE LE MEDIE DELLA MAGNETIZZAZIONE E DELL'ENERGIA PER CIASCUN 
VALORE DI eta */

/*La lunghezza del reticolo devve essere specificata nel file oscillazione.h,
		mentre il numero di misure nel file input.txt, che verrà letto in oscillazione.h*/

int main(void){
	FILE *f;         // puntatore al file degli eta
	int p=0, L;         // p serve per scorrere il file degli eta e L è la lunghezza dell'array degli eta. 

	f=fopen("/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/eta.txt","r");
	control_file(f);

	list *eta=NULL;           // lista contenente i valori degli eta, letti da file
	eta = scan_file(f,eta);         // lettura degli valori degli eta, con funzione ("listfunction.h")
	L=count(eta);         // funzione che conta la lunghezza di una lista ("listfunction.h")
	FILE *misure[L];          // puntatore per i file che salvano gli obs, per ciascun eta
	while(eta!=NULL){
        char filemisure[200];
        sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/Eta/misure_eta=%.1f.txt", eta->val); //it modifies each time the name of the file to etae created
        misure[p]=fopen(filemisure, "w");
        control_file(misure[p]);

		Harmonic_metropolis(eta->val, misure[p]); /*eseguo la simulazione per il valore di eta in causa. 
		Funzione di oscillazione.h Mi creerà automaticamente un file di misure contenente i valori delle osservabili che
		ci interessano (y^2medio, Dy^2medio, ymedio) e eta associato per ogni misura fatta. */
     	
        fclose(misure[p]);
     	printf("ho fatto il etan numero %d, stai sciallo \n", p); 
		eta=eta->next;
		p++;
	}
	printf("Tutto a posto \n");
	fclose(f);
	return 0;
}
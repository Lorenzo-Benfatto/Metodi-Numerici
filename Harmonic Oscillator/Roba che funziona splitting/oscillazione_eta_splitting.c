#include"/Users/margherita/Desktop/UNIPI/QUINT_ANNO/OscillatoreArmonico/Splitting/splitting_energy.h"


/* IN QUESTO CODICE VIENE ESEGUITA LA SIMULAZIONE MONTECARLO PER VARI VALORI DI eta, RIPORTATI NEL FILE "eta.txt". 
PER OGNI VALORE NEL FILE "misure%.1f.txt" SONO RIPORTATE LE MEDIE DELLA MAGNETIZZAZIONE E DELL'ENERGIA PER CIASCUN 
VALORE DI eta */

/*La lunghezza del reticolo devve essere specificata nel file oscillazione.h,
		mentre il numero di misure nel file input.txt, che verrà letto in oscillazione.h*/

int main(void){
	FILE *f;         // puntatore al file degli eta
	int p=0, L;         // p serve per scorrere il file degli eta e L è la lunghezza dell'array degli eta. 
	float ennepereta;
	float singoloeta;
	f=fopen("/Users/margherita/Desktop/UNIPI/QUINT_ANNO/OscillatoreArmonico/Splitting/valori_split.txt","r");
	control_file(f);
 
	list *enne=NULL;           // lista contenente i valori degli eta, letti da file
	list *N=NULL;
	
       
	enne = scan_file(f,enne);         // lettura degli valori degli eta, con funzione ("listfunction.h")
	L=count(enne)-1;         // funzione che conta la lunghezza di una lista ("listfunction.h")
	ennepereta=val_posizione(0,enne);
	enne=move_to_position(1,enne);
	FILE *misure[L];          // puntatore per i file che salvano gli obs, per ciascun eta
	while(enne!=NULL){
	//      char filemisure[500];
	  float eta;
	  eta=ennepereta/(enne->val);
	//      sprintf(filemisure, "/mnt/c/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/N_Eta_fisso/N_Eta=%.0f/misure(N=%.0f)_eta=%.3f.txt", ennepereta, enne->val, eta); //it modifies each time the name of the file to etae created
	//      misure[p]=fopen(filemisure, "w");
	//      control_file(misure[p]);



	  Harmonic_metropolis(enne->val, ennepereta);
	  printf("ok\n"); 
	   /*eseguo la simulazione per il valore di eta in causa. 
	Funzione di oscillazione.h Mi creerà automaticamente un file di misure contenente i valori delle osservabili che
	ci interessano (y^2medio, Dy^2medio, ymedio) e eta associato per ogni misura fatta. */

	   printf("######################################################## \n");
	   printf("              Ho terminato il valore %f                  \n", enne->val);
	   printf("######################################################## \n");      

//        fclose(misure[p]);
		enne=enne->next;
		p++;
	}
        


// DOPO C'è DA METTERE SCELTA TRA LE OPZIONI DI HARMONIC_OSCILLATOR E FARGLI FARE COSE DIVERSE


	fclose(f);
	return 0;
}

#include"/mnt/c/Users/aministratore/Documenti/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/FunzioneDondaFondamentale/update_metro_funzione_d_onda.h"


/* IN QUESTO CODICE VIENE ESEGUITA LA SIMULAZIONE MONTECARLO PER VARI VALORI DI eta, RIPORTATI NEL FILE "eta.txt". 
PER OGNI VALORE NEL FILE "misure%.1f.txt" SONO RIPORTATE LE MEDIE DELLA MAGNETIZZAZIONE E DELL'ENERGIA PER CIASCUN 
VALORE DI eta */

/*La lunghezza del reticolo devve essere specificata nel file oscillazione.h,
		mentre il numero di misure nel file input.txt, che verrà letto in oscillazione.h*/

int main(void){
	FILE *f;         // puntatore al file degli eta
	int p=0, L;         // p serve per scorrere il file degli eta e L è la lunghezza dell'array degli eta. 
	float ennepereta;
    printf("apro valori \n");
	f=fopen("/mnt/c/Users/aministratore/Documenti/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/FunzioneDondaFondamentale/valori_unsoloeta.txt","r");
	control_file(f);
    printf("ho aperto valori \n");

    int scelta;
    float eta;
    float primovalore;
    int enne;

    list *y=NULL;

        y=scan_file(f,y);
        L=count(y)-1;
        primovalore=val_posizione(0,y);
        y=move_to_position(1,y);

        FILE *misure[L];

        if(primovalore>9) {
            scelta=1;

         printf("############################################################# \n");
         printf("Ha scelto di simulare il limite al continuo, buona giornata\n");
         printf("############################################################# \n");
     }

     else {

        scelta=0;

         printf("######################################################## \n");
         printf("Ha scelto di variare la temperatura, buona giornata\n");
         printf("######################################################## \n");

     }


        while(y!=NULL){
        char filemisure[500];
        if(primovalore>9){
            float penne;
            penne=primovalore/(y->val);
            enne=(int)penne;
            eta=y->val;

            sprintf(filemisure, "/mnt/c/Users/aministratore/Documenti/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/N_Eta_fisso/N_Eta=%.0f/misure(N=%d)_eta=%.3f.txt", primovalore, enne, eta); //it modifies each time the name of the file to etae created
        }
        else{
            eta=primovalore;
            enne=(int)y->val;
         sprintf(filemisure, "/mnt/c/Users/aministratore/Documenti/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/N_variabile/misure(eta=%.3f)_N=%d.txt", eta, enne);
     }
        misure[p]=fopen(filemisure, "w");
        control_file(misure[p]);

        Harmonic_metropolis(y->val, misure[p], scelta, primovalore); 
         /*eseguo la simulazione per il valore di eta in causa. 
        Funzione di oscillazione.h Mi creerà automaticamente un file di misure contenente i valori delle osservabili che
        ci interessano (y^2medio, Dy^2medio, ymedio) e eta associato per ogni misura fatta. */
 
         printf("######################################################## \n");
         printf("              Ho terminato il valore %f                  \n", y->val);
         printf("######################################################## \n");      

        fclose(misure[p]);
        y=y->next;
        p++;
    }
        
    printf("############################################################# \n");
    printf("Ci auguriamo sia stata una piacevole esperienza, arrivederci!\n");
    printf("############################################################# \n");
	fclose(f);
	return 0;
}
#include"/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/oscillazione.h"


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
    f=fopen("/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/valori.txt","r");
    control_file(f);

    list *eta=NULL;           // lista contenente i valori degli eta, letti da file
    list *N=NULL;
    
    
        printf("######################################################## \n");
        printf("       Per simulare il limite al continuo digiti 1       \n");
        printf("######################################################## \n");
        printf("   Per simulare la variazione di temperatura digiti 0    \n");
        printf("######################################################## \n");

        int scelta;
        scanf("%d", &scelta);

        if(scelta==1){
            printf("############################################################# \n");
            printf("Ha scelto di simulare il limite al continuo, buona giornata\n");
            printf("############################################################# \n");
            eta = scan_file(f,eta);         // lettura degli valori degli eta, con funzione ("listfunction.h")
            L=count(eta)-1;         // funzione che conta la lunghezza di una lista ("listfunction.h")
            ennepereta=val_posizione(0,eta);
            eta=move_to_position(1,eta);
            FILE *misure[L];          // puntatore per i file che salvano gli obs, per ciascun eta
            while(eta!=NULL){
                char filemisure[500];
                float penne;
                penne=ennepereta/(eta->val);
                int enne;
                enne=(int)penne;
                sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/Eta/misure(N=%d)_eta=%.3f.txt", enne, eta->val); //it modifies each time the name of the file to etae created
                misure[p]=fopen(filemisure, "w");
                control_file(misure[p]);

                Harmonic_metropolis(eta->val, misure[p], scelta, ennepereta); 
                 /*eseguo la simulazione per il valore di eta in causa. 
                Funzione di oscillazione.h Mi creerà automaticamente un file di misure contenente i valori delle osservabili che
                ci interessano (y^2medio, Dy^2medio, ymedio) e eta associato per ogni misura fatta. */
         
                printf("Ho terminato il valore %f\n", eta->val);     

                fclose(misure[p]);
                eta=eta->next;
                p++;
            }
        }


// DOPO C'è DA METTERE SCELTA TRA LE OPZIONI DI HARMONIC_OSCILLATOR E FARGLI FARE COSE DIVERSE


        else {
            printf("######################################################## \n");
            printf("Ha scelto di variare la temperatura, buona giornata\n");
            printf("######################################################## \n");
            N = scan_file(f,N);         // lettura degli valori degli eta, con funzione ("listfunction.h")
            L=count(N)-1;         // funzione che conta la lunghezza di una lista ("listfunction.h")
            singoloeta=val_posizione(0,N);
            N=move_to_position(1,N);
            FILE *misure[L];          // puntatore per i file che salvano gli obs, per ciascun eta
            while(N!=NULL){
                char filemisure[500];
                sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/misure(eta=%.3f)_N=%.3f.txt", singoloeta, N->val); //it modifies each time the name of the file to etae created
                misure[p]=fopen(filemisure, "w");
                control_file(misure[p]);

                Harmonic_metropolis(N->val, misure[p], scelta, singoloeta); 
                 /*eseguo la simulazione per il valore di eta in causa. 
                Funzione di oscillazione.h Mi creerà automaticamente un file di misure contenente i valori delle osservabili che
                ci interessano (y^2medio, Dy^2medio, ymedio) e eta associato per ogni misura fatta. */
                printf("Ho terminato il valore %f\n", N->val);

                fclose(misure[p]);
                N=N->next;
                p++;
            }       
        }


    printf("############################################################# \n");
    printf("Ci auguriamo sia stata una piacevole esperienza, arrivederci!\n");
    printf("############################################################# \n");
    fclose(f);
    return 0;
}
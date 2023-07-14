#include "/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/scalar2d.h"

/* IN QUESTO CODICE VIENE ESEGUITA LA SIMULAZIONE MONTECARLO PER VARI VALORI DI Nt, RIPORTATI NEL FILE "valori.txt". 
PER OGNI VALORE NEL FILE "misure(....).txt" SONO RIPORTATE LE MEDIE del termine di massa, termine cinetico spaziale, 
termine cinetico temporale PER CIASCUN VALORE DI Nt */

/*il numero di misure è nel file input.txt, che verrà letto in scalar2d.h*/


int main(void){
    FILE *f;         // puntatore al file degli Nt
    int p=0, L;         // p serve per scorrere il file degli eta e L è la lunghezza dell'array degli Nt . 
    float Ntm, Nxm;        //sono i primi due valori del file scanfato e sono Nt * m e Ns * m
    float singolom; // valore che leggo come terzo elem del file scanfato, serve solo nel caso di variazione di temperatura
    float m;  //gli assegnerò il valore della massa nel caso scelta=1, quando la massa non è fissa
    int Nx;     //gli assegnerò il valore della Nx ricavato a partire da Nt e m letti da file

    list *Nt=NULL;           // lista contenente i valori degli Nt, letti da file
    
    char valori[300];
    printf("######################################################## \n");
    printf("Per simulare il limite al continuo (nt*m, ns*m fisso) digiti 1\n");
    printf("######################################################## \n");
    printf("Per simulare la variazione di temperatura (Nt variabile, m fisso) digiti 0\n");
    printf("######################################################## \n");
    printf("######################################################## \n");
    printf("Per simulare entrambe digiti 2\n");
    printf("######################################################## \n");

    int scelta; 
    scanf("%d", &scelta);       //scegli quale simulazione fare digitando da tastiera

    sprintf(valori,"/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/valori.txt"); //file degli Nt
    f=fopen(valori,"r");
    control_file(f);

    Nt = scan_file(f,Nt);         // lettura degli valori degli nt, con funzione ("listfunction.h")
    L=count(Nt)-3;         // funzione che conta la lunghezza di una lista ("listfunction.h")
    Ntm = val_posizione(0,Nt);
    Nxm = val_posizione(1,Nt);
    singolom = val_posizione(2,Nt);
    Nt = move_to_position(3,Nt);  //da qui in poi i valori della lista sono solo Nt

    if(scelta==1 || scelta==2){
        printf("############################################################# \n");
        printf("Ha scelto di simulare il limite al continuo, buona giornata\n");
        printf("############################################################# \n");
        
        FILE *misure[L];          // puntatore per i file che salvano gli obs, per ciascun Nt

        while(Nt!=NULL){
            char filemisure[500];
            //ricavo valore della massa e dell?'Nx da Ntm e Nt
            m=Ntm/(Nt->val);   
            Nx = (int)(Nxm/m);

            sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/Continuo/misure(Ntm=%.0f,Nxm=%.0f,Nt=%.0f,Nx=%d,m=%.3f).txt", Ntm, Nxm, Nt->val, Nx, m); //it modifies each time the name of the file to etae created
            misure[p]=fopen(filemisure, "w");
            control_file(misure[p]);

            Scalar_2D(misure[p], Nt->val, Nx, m); 
             /*eseguo la simulazione per il valore di nt in causa. 
            Funzione di scalar2d.h Mi creerà automaticamente un file di misure contenente i valori delle osservabili che
            ci interessano e associato per ogni misura fatta. */
     
            printf("Ho terminato il valore %f\n", Nt->val);     

            fclose(misure[p]);
            Nt=Nt->next;
            p++;
        }
    }


//Questo secondo caso è analogo al primo solo che uso un nome diverso per il puntatore a lista perché nel 
//caso in cui scelga di fare entrambe le simulazioni altrimenti la lista parte da NULL e non gli piace

    if (scelta == 0 || scelta == 2){
        printf("######################################################## \n");
        printf("Ha scelto di variare la temperatura, buona giornata\n");
        printf("######################################################## \n");

        list *Nt2=NULL;
        Nt2=scan_file(f,Nt2);
        Nt2=move_to_position(3,Nt2);
        FILE *misure[L];          // puntatore per i file che salvano gli obs, per ciascun eta
        while(Nt2!=NULL){
            Nx = Nt2->val;
            char filemisure[500];
            Ntm = Nt2->val * singolom;
            Nxm = Ntm;
            sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/Temperatura/misure(Ntm=%.0f,Nxm=%.0f,Nt=%.0f,Nx=%d,m=%.3f).txt", Ntm, Nxm,Nt2->val, Nx, singolom); //it modifies each time the name of the file to etae created
            misure[p]=fopen(filemisure, "w");
            control_file(misure[p]);

            Scalar_2D(misure[p], Nt2->val, Nt2->val, singolom); 
             /*eseguo la simulazione per il valore di eta in causa. 
            Funzione di oscillazione.h Mi creerà automaticamente un file di misure contenente i valori delle osservabili che
            ci interessano (y^2medio, Dy^2medio, ymedio) e eta associato per ogni misura fatta. */
            printf("Ho terminato il valore %f\n", Nt2->val);

            fclose(misure[p]);
            Nt2=Nt2->next;
            p++;
        }       
    }


    printf("############################################################# \n");
    printf("Ci auguriamo sia stata una piacevole esperienza, arrivederci!\n");
    printf("############################################################# \n");
    fclose(f);
    return 0;
}
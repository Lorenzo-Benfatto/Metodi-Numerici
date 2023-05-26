#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"/home/dario/Documents/UNI/Metodi/usefulstuff.h"
#include "/home/dario/Documents/UNI/Metodi/listfunction.h"

//======================================================LEGGI=============================================
/*Nella cartella Nlatt=... devono essere presenti le seguenti cartelle: 
 - CK, in cui verranno salvati i file "C(K)_(eta)..." con i c(k) relativi a ogni eta scelto;
 - Risultati, in cui vengono messi i file del simulatore.c da cui prendiamo
    le misure con cui calcolare il C(k)


Il file eta_interessanti.txt con i valori di eta interessanti l'ho messo nella cartella Bootstrap che contiene 
tutte le cartelle Nlatt, tanto è uguale per tutti */
//========================================================================================================

int Nlatt, N; //Nlatt lunghezza reticolo e N numero di misure

void read_file_input(){ //leggo da file input.txt N e L e altre robe che qui non mi interesssano
    FILE *in;
    in = fopen("/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/input.txt", "r");

    control_file(in);
    int x, i_decorrel, iflag, i_term;  //idec e iflag non servono a niente in questo file, ma li scanfo da input2.txt...
    float d_metro;
    x=fscanf(in, "%f  %d  %d  %d  %d", &d_metro, &N, &i_decorrel, &iflag, &i_term);
    fclose(in);
    return ;
}

int main(void){
    FILE *valori;
    list *eta=NULL;           // lista contenente i valori degli eta, letti da file

    valori=fopen("/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/valori_eta.txt","r");
    control_file(valori);


    eta = scan_file(valori,eta);         // lettura degli valori degli eta, con funzione ("listfunction.h")
    float ennepereta=val_posizione(0,eta);
    eta=move_to_position(1,eta);
    /* Apro il file delle misure */
    FILE *misure;          // puntatore per i file che salvano gli obs, per ciascun eta
    while(eta!=NULL){

        /* Dichiaro e apro il file misure */
        char filemisure[500];
        float penne;
        penne=ennepereta/(eta->val);
        Nlatt=(int)penne;
        sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/Eta/misure(N=%d)_eta=%.3f.txt", Nlatt, eta->val); //it modifies each time the name of the file to etae created
        misure=fopen(filemisure, "r");
        control_file(misure);

        /* Leggo gli input */
        read_file_input();


        int i=0, x, d=1, taglio=0, iter;  // x = variabile per leggere il file, taglio è numerto di misure da tagliare prima che termalizzi la storia montecarlo, d = moltiplicatore per il numero di k (lunghezza di correlazione)
        float mean_y=0, mean_y2=0, c_y=0, c_y2 = 0, y2, dy2, y;            /* c è il valore di C(k). mean_mag e mean_ene sono le medie
                                                                          contenute nel file di medie.txt. mag ene sono le energie per ogni passo del metropolis nel simulazione.h 
                                                                          m, e, b mi servono per scanfare il file medie.txt e sono rispettivamente meìag media, ene media e eta*/
        
        
        FILE *Ck;  //medie punta a file di medie restituito da "simulatore.c" da cui prenderemo i valori medi di ene e mag; Ck ci salvo i Ck per ogni eta; Misure ci scanfo i file misure.txt 
       
        /* Apertura dei file da cui leggere i valori di cui calcolare il C(k) e in cui salvare il valore ottenuto per C(k)*/
        
        char fileCK[200];
        sprintf(fileCK, "/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/Splitting/C(k)_(eta=%.3f,N=%d).txt", eta->val, Nlatt);
        Ck=fopen(fileCK,"w");  //file in cui scriverò il C(k)

        control_file(Ck);

        /* Leggo il file misure e salvo in dati le variabili del campo e del campo al quadrato */
        float dati_y[N], dati_y2[N]; // dati per il campo e campo al quadrato

        x=fscanf(misure, "%f  %f  %f  %d", &y2, &dy2, &y, &iter);
        
        while(x!=EOF){
            dati_y2[i]=y2;
            dati_y[i]=y;

            mean_y = mean_y + y;
            mean_y2 = mean_y2 + y2;

            x=fscanf(misure, "%f  %f  %f  %d", &y2, &dy2, &y, &iter);
            i++;
        }
        
        /* Media */
        mean_y = mean_y/N;
        mean_y2 = mean_y2/N;


        /* Calcolo della normalizzazione del C(k). Qui si mette k = 0, quindi viene la sommatoria sul quadrato */

        float ck0_y=0, ck0_y2=0;              //mi salvo in ck0 il valore del C(k) per k==0 col quale dovrò normalizzare il resto dei c(k)
        for(int j=taglio; j<N; j++){
                ck0_y=pow((dati_y[j]-mean_y),2)+ck0_y;
                ck0_y2=pow((dati_y2[j]-mean_y2),2)+ck0_y2;
            }
        ck0_y=ck0_y/(N-taglio);
        ck0_y2=ck0_y2/(N-taglio);  

        /* Calcolo del C(k) */

        for(int k=0; k<200; k++){
            for(int j=taglio; j<N-k*d; j++){
                c_y=(dati_y[j]-mean_y)*(dati_y[j+k*d]-mean_y)+c_y;
                c_y2=(dati_y2[j]-mean_y2)*(dati_y2[j+k*d]-mean_y2)+c_y2;
            }
            c_y=c_y/ck0_y;
            c_y=c_y/(N-k*d-taglio);

            c_y2=c_y2/ck0_y2;
            c_y2=c_y2/(N-k*d-taglio);
            fprintf(Ck, "%d  %f  %f\n", k*d, c_y, c_y2);
        }

        fclose(Ck);
        fclose(misure);
        
        eta=eta->next;
    }
        
    
    return 0;
}
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"/home/dario/Documents/Metodi/Modulo1/Ising/usefulstuff.h"
#include "/home/dario/Documents/Metodi/Modulo1/Ising/listfunction.h"

//======================================================LEGGI=============================================
/*Nella cartella Nlatt=... devono essere presenti le seguenti cartelle: 
 - CK, in cui verranno salvati i file "C(K)_(beta)..." con i c(k) relativi a ogni beta scelto;
 - Risultati, in cui vengono messi i file del simulatore.c da cui prendiamo
    le misure con cui calcolare il C(k)


Il file beta.txt con i valori di beta interessanti l'ho messo nella cartella Bootstrap che contiene 
tutte le cartelle Nlatt, tanto è uguale per tutti */
//========================================================================================================

int Nlatt, N; //Nlatt lunghezza reticolo e N numero di misure

void read_file_input(){ //leggo da file input.txt N e L e altre robe che qui non mi interesssano
    FILE *in;
    in = fopen("/home/dario/Documents/Metodi/Modulo1/Ising/input2.txt", "r");
    printf("ok0\n");
    control_file(in);
    int x, iflag, idec; 
    float extfield;
    x=fscanf(in, "%d  %d  %d  %f  %d", &iflag, &N, &idec, &extfield, &Nlatt );
    fclose(in);
    return ;
}

int main(void){
    FILE *bet;
    bet=fopen("/home/dario/Documents/Metodi/Modulo1/Ising/beta_init.txt","r");
    control_file(bet);

    read_file_input();
              // lunghezza di un lato del reticolo
    int i=0, x, y, d=1, taglio=0, idec;          // N = numero di misure, x = variabile per leggere il file, taglio 0 numerto di misure da tagliare prima che termalizzi la storia montecarlo, d = moltiplicatore per il numero di k (lunghezza di correlazione).
    //idec è il passo del metropolis fatto dal simulazione.h
    float beta, b , mean_mag, mean_ene, c=0, mag, ene, m, e;            /* c è il valore di C(k). mean_mag e mean_ene sono le medie
                                                                      contenute nel file di medie.txt. mag ene sono le energie per ogni passo del metropolis nel simulazione.h 
                                                                      m, e, b mi servono per scanfare il file medie.txt e sono rispettivamente meìag media, ene media e beta*/
    
    int p=0;
    x=fscanf(bet, "%f", &beta);
    while(x!=EOF){
        //float *dati=malloc(sizeof(float)*N);
        float dati[N];
        printf("ok ciao\n");
        FILE *medie, *Ck, *misure;  //medie punta a file di medie restituito da "simulatore.c" da cui prenderemo i valori medi di ene e mag; Ck ci salvo i Ck per ogni beta; Misure ci scanfo i file misure.txt 
        /* Apertura dei file da cui leggere i valori di cui calcolare il C(k) e in cui salvare il valore ottenuto per C(k)*/
        char filename[200];
        sprintf(filename, "/home/dario/Documents/Metodi/Modulo1/Ising/Bootstrap/Nlatt=%d/medie.txt", Nlatt);      // per creare il nome del file in base alla lunghezza del reticolo
        medie=fopen(filename,"r");
        printf("ok1\n");
        control_file(medie);
        sprintf(filename, "/home/dario/Documents/Metodi/Modulo1/Ising/Bootstrap/Nlatt=%d/CK/C(k)_(beta)%.3f.txt", Nlatt, beta);
        Ck=fopen(filename,"w");  //file in cui scriverò il C(k)
        printf("ok2\n");
        control_file(Ck);
        y=fscanf(medie, "%f  %f  %f", &m, &e, &b); //scanfo le medie e il beta in questione. Devo confrontare il b col beta del ciclo while
        while(y!=EOF){
            if(b==beta){
                mean_ene=e;
                mean_mag=m;
                printf("%f %f %f \n", b, e, m);

            }

            y=fscanf(medie, "%f  %f  %f", &m, &e, &b);
        }
        char mis[300];
        sprintf(mis,"/home/dario/Documents/Metodi/Modulo1/Ising/Bootstrap/Nlatt=%d/Risultati/misure%.3f.txt", Nlatt, beta);
        misure=fopen(mis,"r");
        printf("%d %f\n", Nlatt, beta);
        printf("ok3\n");
        control_file(misure);


        x=fscanf(misure, "%f  %f  %d  %f", &mag, &ene, &idec, &b);
        i=0;
        while(x!=EOF){
            dati[i]=ene;
            x=fscanf(misure, "%f  %f  %d  %f", &mag, &ene, &idec, &b);
            i++;

        }
        printf("ok4\n");
        for(int k=0; k<10; k++){
            for(int j=taglio; j<N-k*d; j++){
                c=(dati[j]-mean_ene)*(dati[j+k*d]-mean_ene)+c;
            }
            c=c/(N-k*d-taglio);
            fprintf(Ck, "%d  %f\n", k*d, c);
        }
        

        printf("c=%f mean=%f nel caso di Beta=%f\n", c, mean_ene, beta);
        //free(dati);
        printf("ok chiuso %d\n", N);


        fclose(medie);
        fclose(Ck);
        fclose(misure);
        printf("ok finale\n");
        x=fscanf(bet, "%f", &beta);
    }
    return 0;
}

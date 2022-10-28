#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"usefulstuff.h"

int main(void){
    
    int L = ;           // lunghezza di un lato del reticolo
    int i=0, x, N=10000000, d=1, taglio=500000, idec;          // N = numero di misure, x = variabile per leggere il file, taglio 0 numerto di misure da tagliare prima che termalizzi la storia montecarlo, d = moltiplicatore per il numero di k (lunghezza di correlazione).
    //idec è il passo del metropolis fatto dal simulazione.h
    float beta, mean_mag, mean_ene, c=0, mag, ene;            /* c è il valore di C(k). mean_mag e mean_ene sono le medie
                                                                      contenute nel file di medie.txt. mag ene sono le energie per ogni passo del metropolis nel simulazione.h */
    float *dati=malloc(sizeof(float)*N);
    FILE *f, *g, *misure;
    
    /* Apertura dei file da cui leggere i valori di cui calcolare il C(k) e in cui salvare il valore ottenuto per C(k)*/
    char directory[200];
    sprintf(filename, "/Users/margherita/Desktop/UNIPI/QUINT_ANNO/Ising/Nlatt = %d/medie.txt", L);      // per creare il nome del file in base alla lunghezza del reticolo
    f=fopen(filename,"r");
    control_file(f);
    char directory[200];
    sprintf(g, "/Users/margherita/Desktop/UNIPI/QUINT_ANNO/Ising/Nlatt = %d/Correlazione(k).txt", L);
    g=fopen(g,"w");  //file in cui scriverò il C(k)
    control_file(g);
    x=fscanf(f, "%f  %f  %f", &mean_mag, &mean_ene, &beta); //scanfo le medie e il beta in questione
    char mis[200];
    sprintf(mis,"/Users/margherita/Desktop/UNIPI/QUINT_ANNO/Ising/Nlatt = %d/Risultati/misure%f.txt", beta);
    misure=fopen(mis,"r");
    control_file(mis);

    x=fscanf(misure, "%f  %f  %d  %f", &mag, &ene, &idec, &beta);
    while(x!=EOF){
        dati[i]=ene;
        x=fscanf(misure, "%f  %f  %d  %f", &mag, &ene, &idec, &beta);
        i++;

    }
    for(int t=1; t<5000; t++){
        for(int j=taglio; j<N-t*d; j++){
        c=(dati[j]-mean_ene)*(dati[j+t*d]-mean_ene)+c;
        }
        c=c/(N-t*d-taglio);
        fprintf(g, "%d  %f\n", t*d, c);
    }
    

    printf("c=%f mean=%f\n", c, mean_ene);
    free(dati);

    fclose(f);
    fclose(g);
    fclose(misure);


    
    return 0;
}


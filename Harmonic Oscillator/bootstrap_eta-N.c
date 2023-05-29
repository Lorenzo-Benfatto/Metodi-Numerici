#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include"/home/dario/Documents/UNI/Metodi/usefulstuff.h"
#include"/home/dario/Documents/UNI/Metodi/ran2.h"
#include"/home/dario/Documents/UNI/Metodi/listfunction.h"

int N_misure; //N = numero di misure----> lo leggo da file 

void bootstrap(FILE *f, char dir[50], float e, int enne, int scelta){
    printf("eta = %f\n", e);
    FILE *x2 ,*dx2, *ene;
    unsigned long s; //serve per salvarsi il numero casuale enorme che produco
    int i=0,j,k,M=100,x,bin=100,taglio=0;         /*M = numero di resampling, 
    x serve per scanfare i file, i e j indici ricorsivi, bin è il bin del bootstrap, 
    taglio è per levare le misure non termalizzate*/
    int V=N_misure-taglio;
    long int seed = 13;         // seed per il ran2()
    float ics2, dics2, fild /*cinetica*/;         // risultati del simulatore_boot.c e valore del beta corrente
    int iter;           // risultati del simulatore_boot.c
    float *dati_x2 = malloc(sizeof(long double) * (N_misure-taglio));
    float *dati_dx2 = malloc(sizeof(long double) * (N_misure-taglio));
    float *dati_ene = malloc(sizeof(long double) * (N_misure-taglio));
    

    x=fscanf(f, "%f  %f  %f  %d", &ics2, &dics2, &fild, /*&cinetica,*/ &iter);
    while (x!=EOF){
            dati_x2[i]=ics2;
            dati_dx2[i]=dics2;
            if(scelta!=1) dati_ene[i]=ics2/2 - dics2/(2*pow(e,2)) + 1/(2*e);
            x=fscanf(f, "%f  %f  %f  %d", &ics2, &dics2, &fild, /*&cinetica,*/ &iter);
            i++;
        }
    /* Questo file va aperto quando sappiamo già il valore di beta*/
    char filemisure[700];
    sprintf(filemisure,"/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/%s/Bootstrap/resample_binned_X2(eta=%.3f,N=%d).txt", dir, e, enne);
    x2=fopen(filemisure,"w");
    control_file(x2);
    
    sprintf(filemisure,"/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/%s/Bootstrap/resample_binned_DX2(eta=%.3f,N=%d).txt",dir, e, enne);
    dx2=fopen(filemisure,"w");
    control_file(dx2);
    if(scelta!=1){
        sprintf(filemisure,"/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/%s/Bootstrap/resample_binned_energy(eta=%.3f,N=%d).txt",dir, e, enne);
        ene=fopen(filemisure,"w");
        control_file(ene);
    }
    

    for(j=0;j<M;j++){
        for (k=taglio;k<N_misure/(bin+1);k++){
            s = ran2(&seed) * RAND_MAX;
            s = (int)s;
            s <<= 15;
            s ^= (int)(ran2(&seed) * RAND_MAX);
            s %= V+1;
            s=s+taglio;
            //printf("%ld \n",s);
            for(int h=0;h<bin+1;h++){
                if(s+h>N_misure-1){
                    fprintf(x2,"%lf ",dati_x2[s+h+1-N_misure]);
                    fprintf(dx2,"%lf ",dati_dx2[s+h+1-N_misure]);
                    if (scelta!=1) fprintf(ene,"%lf ",dati_ene[s+h+1-N_misure]);
                 //printf("sono dentro il primo if \n");
                }
                if(s+h<=N_misure-1){
                    fprintf(x2,"%lf ",dati_x2[s+h]);
                    fprintf(dx2,"%lf ",dati_dx2[s+h]);
                    if(scelta!=1) fprintf(ene,"%lf ",dati_ene[s+h]);
                 //printf("sono dentro il secondo if \n");
                }

            }
        }
            fprintf(x2,"\n");
            fprintf(dx2,"\n");
            if(scelta!=1)fprintf(ene,"\n");
        }
    

    free(dati_x2);
    free(dati_dx2);
    free(dati_ene);
    fclose(x2);
    fclose(dx2);
    if(scelta!=1)fclose(ene);
    return;
}


/*==== NOTA: Nel file di input.txt classico non viene specificata la lunghezza del reticolo. Quindi è necessario
crearne uno che la contenga (io l'ho chiamato input2.txt)*/ 
void read_file_input(){ /*Mi serve solo per leggere il numero di misure N e la larghezza del reticolo L,
 gli altri non mi servono a niente, quindi non gli do nemmeno un nome significativo*/
    FILE *in;
    in = fopen("/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/input.txt", "r");
    control_file(in);
    int x, idec, iflag, i_term; 
    float eta, d_metro;
    x=fscanf(in, "%f  %d  %d  %d  %d", &d_metro, &N_misure, &idec, &iflag, &i_term);
    fclose(in);
    return ;
}


int main(){

    read_file_input();  // funzione per leggere file con variabili di input (numero di misure, lunghezza del lato del reticolo, iflag per la partenza e campo esterno e idecorrel)

    char path[500];

    FILE *f;         // puntatore al file degli eta
    int p=0, L;         // p serve per scorrere il file degli eta e L è la lunghezza dell'array degli eta. 
    float ennepereta;   //è il prodotto di lunghezza path per valore di eta, è costante nel limite al continuo e sarà il primo elemento del file valori.txt nel caso 1
    float singoloeta;   //primo elemento del file valori.txt nel caso scelta = 0, eta fisso al variare degli N
    char dir[50];
    

    list *eta=NULL;           // lista contenente i valori degli eta, letti da file, per caso scela=1
    list *N=NULL;              // lista contenente i valori degli N, letti da file, per caso scela=0
    
    
    printf("######################################################## \n");
    printf("       Per bootstrappare il limite al continuo digiti 1       \n");
    printf("######################################################## \n");
    printf("   Per bootstrappare la variazione di temperatura digiti 0    \n");
    printf("######################################################## \n");

    int scelta;
    scanf("%d", &scelta);

    if(scelta==1){
        printf("############################################################# \n");
        printf("Ha scelto di bootstrappare il limite al continuo, buona giornata\n");
        printf("############################################################# \n");
        f=fopen("/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/valori_eta.txt","r");  //leggo il file valori.txt
        control_file(f);
        eta = scan_file(f,eta);         // lettura degli valori degli eta, con funzione ("listfunction.h")
        L=count(eta)-1;         // funzione che conta la lunghezza di una lista ("listfunction.h")
        ennepereta = val_posizione(0,eta);
        eta = move_to_position(1,eta);

        FILE *misure[L];          // puntatore per i file che salvano gli obs, per ciascun eta
        while(eta!=NULL){
            printf("Sono nel ciclo %d \n",p);
            char filemisure[500];
            float penne;
            penne=ennepereta/(eta->val);
            int enne;
            enne=(int)penne;
            sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/Eta/misure(N=%d)_eta=%.3f.txt", enne, eta->val); //it modifies each time the name of the file to etae created
            misure[p]=fopen(filemisure, "r");
            control_file(misure[p]);
            sprintf(dir,"Eta");
            printf("sono nella cartella %s \n", dir);
            bootstrap(misure[p], dir, eta->val, enne, scelta);

            printf("Ho letto il valore %f\n", eta->val);     

            fclose(misure[p]);
            printf("chiuso misure\n");
            eta=eta->next;
            p++;
        }
    }


// DOPO C'è DA METTERE SCELTA TRA LE OPZIONI DI HARMONIC_OSCILLATOR E FARGLI FARE COSE DIVERSE


    else {
        printf("######################################################## \n");
        printf("Ha scelto di variare la temperatura, buona giornata\n");
        printf("######################################################## \n");
        f=fopen("/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/valori_n.txt","r");  //leggo il file valori.txt
        control_file(f);
        N = scan_file(f,N);         // lettura degli valori degli eta, con funzione ("listfunction.h")
        L=count(N)-1;         // funzione che conta la lunghezza di una lista ("listfunction.h")
        singoloeta=val_posizione(0,N);
        N=move_to_position(1,N);
        FILE *misure[L];          // puntatore per i file che salvano gli obs, per ciascun eta
        while(N!=NULL){
            char filemisure[500];
            sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/N_variabile/misure(eta=%.3f)_N=%.3f.txt", singoloeta, N->val); //it modifies each time the name of the file to etae created
            misure[p]=fopen(filemisure, "r");
            control_file(misure[p]);

            sprintf(dir,"N_variabile");

            bootstrap(misure[p], dir, singoloeta, N->val, scelta);

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

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

int n,idec; //n = numero di misure----> lo leggo da file 


//# FUNZIONE PER FARE IL BOOTSTRAP
void bootstrap(FILE *f, float e, int enne,int tau, char string[10], int altro, int N_misure){
    printf("eta = %f\n", e);
    FILE *ckappa;
    unsigned long s; //serve per salvarsi il numero casuale enorme che produco
    int i=0,j,k,M=10,x,bin=1,taglio=0;         /*M = numero di resampling, 
    x serve per scanfare i file, i e j indici ricorsivi, bin è il bin del bootstrap, 
    taglio è per levare le misure non termalizzate*/
    int V=N_misure-taglio;
    long int seed = 13;         // seed per il ran2()
    float ck;         // risultati del simulatore_boot.c e valore del beta corrente
    int iter;           // risultati del simulatore_boot.c
    
    printf("%d", N_misure);
    float *dati= malloc(sizeof(long double) * (N_misure-taglio));
    printf("ok\n");
    printf("%d", N_misure);
    x=fscanf(f, "%f", &ck);

    while (x!=EOF){
            dati[i]=ck;
            x=fscanf(f, "%f", &ck);
            i++;
        }
    printf("ok\n");
    /* Questo file va aperto quando sappiamo già il valore di beta*/
    char filemisure[700];

    if (altro==1){

        sprintf(filemisure,"/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/Splitting/Bootstrap/resample_binned_%s(eta=%.1f,N=%d).txt", string,e, enne);


    }
    else {sprintf(filemisure,"/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/Splitting/Bootstrap/resample_binned_ck_%s(eta=%.1f,N=%d,k=%d).txt", string,e, enne, tau);}
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
                    fprintf(ckappa,"%lf ",dati[s+h+1-N_misure]);
                }
                if(s+h<=N_misure-1){
                    fprintf(ckappa,"%lf ",dati[s+h]);
                }

            }
        }
            fprintf(ckappa,"\n");
        }
    
    ckappa=fopen(filemisure,"w");
    control_file(ckappa);


    

    free(dati);
    fclose(ckappa);
    return;
}

// FUNZIONE PER LEGGERE FILE DI INPUT
/*==== NOTA: Nel file di input.txt classico non viene specificata la lunghezza del reticolo. Quindi è necessario
crearne uno che la contenga (io l'ho chiamato input2.txt)*/ 
void read_file_input(){ /*Mi serve solo per leggere il numero di misure N e la larghezza del reticolo L,
 gli altri non mi servono a niente, quindi non gli do nemmeno un nome significativo*/
    FILE *in;
    in = fopen("/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/input_split.txt", "r");
    control_file(in);
    int x, iflag, i_term; 
    float eta, d_metro;
    x=fscanf(in, "%f  %d  %d  %d  %d", &d_metro, &n, &idec, &iflag, &i_term);
    fclose(in);
    return ;
}


int main(){

    read_file_input();  // funzione per leggere file con variabili di input (numero di misure, lunghezza del lato del reticolo, iflag per la partenza e campo esterno e idecorrel)

    // NON SERVE char path[500];

    FILE *f;         // puntatore al file degli eta
    int N_misure;
    int p=0, L;         // p serve per scorrere il file degli eta e L è la lunghezza dell'array degli Nlatt. 
    float ennepereta;   //è il prodotto di lunghezza path per valore di eta, è costante nel limite al continuo e sarà il primo elemento del file valori.txt nel caso 1
    float singoloeta;   //primo elemento del file valori.txt nel caso scelta = 0, eta fisso al variare degli N
    // NON SERVE char dir[70];
    int altro;

    list *Nlatt=NULL;           // lista contenente i valori degli eta, letti da file, per caso scela=1


    f=fopen("/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/valori_split.txt","r");  //leggo il file valori.txt
    control_file(f);
    Nlatt = scan_file(f,Nlatt);         // lettura dei valori degli eta, con funzione ("listfunction.h")
    L=count(Nlatt)-1;         // funzione che conta la lunghezza di una lista ("listfunction.h")
    ennepereta = val_posizione(0,Nlatt);       // il primo valore è il prodotto di Nlatt*eta
    Nlatt = move_to_position(1,Nlatt);      // dopodiché salvo i valori successivi (Nlatt) 

    int max_k=20;       // massimo valore di k (per il C(k))
    
    FILE *misure[L];          // puntatore per i file che salvano gli obs, per ciascun eta
    while(Nlatt!=NULL){
        printf("Sono nel ciclo %d \n",p);
        char filemisure[700];
        float eta;
        eta=ennepereta/(Nlatt->val);
        printf("%.3f\n", eta);
        for(int k=0; k<max_k; k++){
            altro = 0;      
            int ennelatt;
            ennelatt=(int)Nlatt->val;
            //ho 2 file quasi uguali di cui fare il bootstrap, cambia solo un 2 nel nome=> faccio 2 volte bootstrap a stesso eta

            // PRIMA VOLTA
            char string[10]="";     // stringa per inserire il 2 nel nome
            sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/Splitting/Campo/C(k)/Ck_Y%s(eta=%.3f,N_Eta=%.0f,k=%d).txt", string, eta, ennepereta,k); //it modifies each time the name of the file to etae created
            misure[p]=fopen(filemisure, "r");
            control_file(misure[p]);
            printf("ok1\n");
            N_misure=n*idec;    //n è globale e anche idec
            
            bootstrap(misure[p], eta, ennelatt, k, string, altro, N_misure);

            // SECONDA VOLTA
            char string2[10]="2";       // stringa per inserire il 2 nel nome
            sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/Splitting/Campo/C(k)/Ck_Y%s(eta=%.3f,N_Eta=%.0f,k=%d).txt", string2, eta, ennepereta,k); //it modifies each time the name of the file to etae created
            misure[p]=fopen(filemisure, "r");
            control_file(misure[p]);
            printf("ok2\n");
            N_misure=n*idec;
            
            bootstrap(misure[p], eta, ennelatt, k, string2, altro , N_misure);

            //prima volta <Y>
            char string3[10]="<Y>";
            sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/Splitting/Campo/C(k)/%s(eta=%.3f,N_Eta=%.0f).txt",  string3, eta, ennepereta); //it modifies each time the name of the file to etae created
            misure[p]=fopen(filemisure, "r");
            control_file(misure[p]);
            printf("ok3\n");
            altro=1;

            bootstrap(misure[p], eta, ennelatt, k, string3, altro, n);

            //prima volta <Y2>
            char string4[10]="<Y2>";
            sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/Splitting/Campo/C(k)/%s(eta=%.3f,N_Eta=%.0f).txt", string4, eta, ennepereta); //it modifies each time the name of the file to etae created
            misure[p]=fopen(filemisure, "r");
            control_file(misure[p]);
            printf("ok4\n");
            altro=1;

            bootstrap(misure[p], eta, ennelatt, k, string4, altro, n);

            printf("Ho letto il valore %f\n", Nlatt->val);     

            fclose(misure[p]);
            printf("chiuso misure\n");
        }
        Nlatt=Nlatt->next;
        p++;
    }



    printf("############################################################# \n");
    printf("Ci auguriamo sia stata una piacevole esperienza, arrivederci!\n");
    printf("############################################################# \n");
    fclose(f);

    return 0;
}

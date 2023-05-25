#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include"/mnt/c/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/usefulstuff.h"
#include"/mnt/c/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/ran2.h"
#include"/mnt/c/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/listfunction.h"

int N_misure; //N = numero di misure----> lo leggo da file 

void bootstrap(FILE *f, char dir[50], float e, int enne){
    printf("eta = %f\n", e);
    FILE *x2 ,*dx2;
    unsigned long s; //serve per salvarsi il numero casuale enorme che produco
    int i=0,j,k,M=100,x,bin=100,taglio=0;         /*M = numero di resampling, 
    x serve per scanfare i file, i e j indici ricorsivi, bin è il bin del bootstrap, 
    taglio è per levare le misure non termalizzate*/
    int V=N_misure-taglio;
    long int seed = 13;         // seed per il ran2()
    float ics2, dics2, fild;         // risultati del simulatore_boot.c e valore del beta corrente
    int iter;           // risultati del simulatore_boot.c
    float *dati_x2 = malloc(sizeof(long double) * (N_misure-taglio));
    float *dati_dx2 = malloc(sizeof(long double) * (N_misure-taglio));
    

    x=fscanf(f, "%f  %f  %f  %d", &ics2, &dics2, &fild, &iter);
    while (x!=EOF){
            dati_x2[i]=ics2;
            dati_dx2[i]=dics2;
            x=fscanf(f, "%f  %f  %f  %d", &ics2, &dics2, &fild, &iter);
            i++;
        }
    /* Questo file va aperto quando sappiamo già il valore di beta*/
    char filemisure[700];
    sprintf(filemisure,"/mnt/c/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/%s/Bootstrap/resample_binned_X2(eta=%.3f,N=%d).txt", dir, e, enne);
    x2=fopen(filemisure,"w");
    control_file(x2);
    
    sprintf(filemisure,"/mnt/c/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/%s/Bootstrap/resample_binned_DX2(eta=%.3f,N=%d).txt",dir, e, enne);
    dx2=fopen(filemisure,"w");
    control_file(dx2);
    
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
                 //printf("sono dentro il primo if \n");
                }
                if(s+h<=N_misure-1){
                    fprintf(x2,"%lf ",dati_x2[s+h]);
                    fprintf(dx2,"%lf ",dati_dx2[s+h]);
                 //printf("sono dentro il secondo if \n");
                }

            }
        }
            fprintf(x2,"\n");
            fprintf(dx2,"\n");
        }
    

    free(dati_x2);
    free(dati_dx2);
    fclose(x2);
    fclose(dx2);
    return;
}


/*==== NOTA: Nel file di input.txt classico non viene specificata la lunghezza del reticolo. Quindi è necessario
crearne uno che la contenga (io l'ho chiamato input2.txt)*/ 
void read_file_input(){ /*Mi serve solo per leggere il numero di misure N e la larghezza del reticolo L,
 gli altri non mi servono a niente, quindi non gli do nemmeno un nome significativo*/
    FILE *in;
    in = fopen("/mnt/c/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/input.txt", "r");
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
    f=fopen("/mnt/c/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/valori.txt","r");  //leggo il file valori.txt
    control_file(f);

    int scelta;
    float primovalore;
    int enne;
    float eta;

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
            eta=y->val;
            penne=primovalore/eta;
            enne=(int)penne;
            sprintf(filemisure, "/mnt/c/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/Eta/misure(N=%d)_eta=%.3f.txt", enne, eta); //it modifies each time the name of the file to etae created
            sprintf(dir,"Eta");
        }
        else {
            eta=primovalore;
            enne=y->val;
            sprintf(filemisure, "/mnt/c/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/N_variabile/misure(eta=%.3f)_N=%.d.txt", eta, enne);
            sprintf(dir,"N_variabile");

        }
        misure[p]=fopen(filemisure, "r");
        control_file(misure[p]);

        bootstrap(misure[p], dir, eta, enne);
 
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

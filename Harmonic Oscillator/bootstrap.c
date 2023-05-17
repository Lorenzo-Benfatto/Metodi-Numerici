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

int L, N; //N = numero di misure, L = lunghezza reticolo ----> li leggo da file 

void bootstrap(FILE *f){
    FILE *x2 ,*dx2;
    unsigned long s; //serve per salvarsi il numero casuale enorme che produco
    int i=0,j,k,M=100,x,bin=100,taglio=0;         /*M = numero di resampling, 
    x serve per scanfare i file, i e j indici ricorsivi, bin è il bin del bootstrap, 
    taglio è per levare le misure non termalizzate*/
    int V=N-taglio;
    long int seed = 13;         // seed per il ran2()
    float ics2, dics2;         // risultati del simulatore_boot.c e valore del beta corrente
    int iter;           // risultati del simulatore_boot.c
    float *dati_x2 = malloc(sizeof(long double) * (N-taglio));
    float *dati_dx2 = malloc(sizeof(long double) * (N-taglio));
    

    x=fscanf(f, "%f  %f  %d", &ics2, &dics2, &iter);
    while (x!=EOF){
            dati_x2[i]=ics2;
            dati_dx2[i]=dics2;
            x=fscanf(f, "%f  %f  %d", &ics2, &dics2, &iter);
            i++;
        }
    /* Questo file va aperto quando sappiamo già il valore di beta*/
    char filemisure[500];
    sprintf(filemisure,"/mnt/c/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/Bootstrap/resample_binned_X2.txt");
    x2=fopen(filemisure,"w");
    control_file(x2);
    
    sprintf(filemisure,"/mnt/c/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/Bootstrap/resample_binned_DX2.txt");
    dx2=fopen(filemisure,"w");
    control_file(dx2);
    
    for(j=0;j<M;j++){
        for (k=taglio;k<N/(bin+1);k++){
            s = ran2(&seed) * RAND_MAX;
            s = (int)s;
            s <<= 15;
            s ^= (int)(ran2(&seed) * RAND_MAX);
            s %= V+1;
            s=s+taglio;
            //printf("%ld \n",s);
            for(int h=0;h<bin+1;h++){
                if(s+h>N-1){
                    fprintf(x2,"%lf ",dati_x2[s+h+1-N]);
                    fprintf(dx2,"%lf ",dati_dx2[s+h+1-N]);
                 //printf("sono dentro il primo if \n");
                }
                if(s+h<=N-1){
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
    fclose(f);
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
    x=fscanf(in, "%f  %f  %d  %d  %d  %d  %d", &eta, &d_metro, &N, &idec, &iflag, &i_term, &L  );
    fclose(in);
    return ;
}


int main(){
    read_file_input();  // funzione per leggere file con variabili di input (numero di misure, lunghezza del lato del reticolo, iflag per la partenza e campo esterno e idecorrel)

    FILE *input;

    char path[500];
    sprintf(path ,"/mnt/c/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/Oscillo/misure.txt"); 
    input = fopen(path, "r");
    control_file(input);

    bootstrap(input);

    return 0;
}

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


int L , N;

float betaok = 0.440;

void bootstrap(FILE *f, int M){
    FILE *g;
    unsigned long s;
    int i=0,j,k,x,bin,taglio=1;         //N = numero di misure, M = numero di resampling, x serve per scanfare i file, i e j indici ricorsivi, bin è il bin del bootstrap, taglio è per levare le misure non termalizzate
    int V=N-taglio;
    long int seed = 13;         // seed per il ran2()
    float mag, ene, beta;         // risultati del simulatore_boot.c e valore del beta corrente
    int iter;           // risultati del simulatore_boot.c
    float *dati = malloc(sizeof(long double) * (N-taglio));
    
    

    x=fscanf(f, "%f  %f  %d  %f", &mag, &ene, &iter, &beta);
    while (x!=EOF){
            dati[i]=mag;
            x=fscanf(f, "%f  %f  %d  %f", &mag, &ene, &iter, &beta);
            i++;
        }

    /* Questo file va aperto quando sappiamo già il valore di beta*/
    char filemisure[500];

    sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo1/Ising/Bootstrap/Definitivi/Nlatt=%d/Decorrelazione/ResampBoot/resample_binned(beta)%.3f(res)%d.txt", L, beta, M);

    g=fopen(filemisure,"w");
    control_file(g);
    
    bin = 1;
    for(j=0;j<M;j++){
        for (k=taglio;k<N/(bin+1);k++){
            s = ran2(&seed) * RAND_MAX;
            s = (int)s;
            s <<= 15;
            s ^= (int)(ran2(&seed) * RAND_MAX);
            s %= V+1;
            s=s+taglio;
            
            for(int h=0;h<bin+1;h++){
                if(s+h>N-1){
                 fprintf(g,"%lf ",dati[s+h+1-N]);
                }
                if(s+h<=N-1){
                 fprintf(g,"%lf ",dati[s+h]);
                }

            }
        }
        fprintf(g,"\n");
    }

    free(dati);
    fclose(f);
    fclose(g);
    return;
}


void read_file_input(){
    FILE *in;
    in = fopen("/home/dario/Documents/UNI/Metodi/Modulo1/Ising/input2.txt", "r");
    control_file(in);
    int x, a,id; 
    float b;
    x=fscanf(in, "%d  %d  %d  %f  %d  ", &a, &N, &id, &b, &L  );
    fclose(in);
    return ;
}


int main()
{
    read_file_input();// funzione per leggere file con variabili di input (nunero di misure, lunghezza del lato del reticolo, iflag per la partenza e campo esterno)

    FILE *input;
        
    for(int i=1; i<10; i++){
        int j=i*10;   
        char path[500];
        sprintf(path, "/home/dario/Documents/UNI/Metodi/Modulo1/Ising/Bootstrap/Definitivi/Nlatt=%d/Risultati/misure%.3f.txt", L, betaok);
        input = fopen(path, "r");
            
        control_file(input);
        
        bootstrap(input, j);
            
        
            
    }

    return(0);
}

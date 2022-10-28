#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include"usefulstuff.h"
#include"ran2.h"
#include"listfunction.h"


#define L 50

void bootstrap(FILE *f){
    FILE *g;
    unsigned long s;
    int i=0,j,k,N=10000,M=100,x,bin,taglio=1;         //N = numero di misure, M = numero di resampling, x serve per scanfare i file, i e j indici ricorsivi, bin è il bin del bootstrap, taglio è per levare le misure non termalizzate
    int V=N-taglio;
    long int seed = 13;         // seed per il ran2()
    float mag, ene, beta;         // risultati del simulatore_boot.c e valore del beta corrente
    int iter;           // risultati del simulatore_boot.c
    float *dati = malloc(sizeof(long double) * (N-taglio));
    int molt = 24;
    

    x=fscanf(f, "%f  %f  %d  %f  %d", &mag, &ene, &iter, &beta, &bin);
    while (x!=EOF){
            dati[i]=mag;
            x=fscanf(f, "%f  %f  %d  %f  %d", &mag, &ene, &iter, &beta, &bin);
            i++;
        }
    /* Questo file va aperto quando sappiamo già il valore di beta*/
    char filemisure[200];
    if(bin<100){
        sprintf(filemisure, "/Users/margherita/Desktop/UNIPI/QUINT_ANNO/Ising/Nlatt = %d/Bootstrap/DecorrBoot/resample_binned(beta)%.3f(idec)_%d.txt", L, beta, bin);
    }
    else sprintf(filemisure, "/Users/margherita/Desktop/UNIPI/QUINT_ANNO/Ising/Nlatt = %d/Bootstrap/DecorrBoot/resample_binned(beta)%.3f(idec)%d.txt", L, beta, bin);
    g=fopen(filemisure,"w");
    control_file(g);
    
    bin = bin*molt;
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
                 fprintf(g,"%lf ",dati[s+h+1-N]);
                 //printf("sono dentro il primo if \n");
                }
                if(s+h<=N-1){
                 fprintf(g,"%lf ",dati[s+h]);
                 //printf("sono dentro il secondo if \n");
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


int main()
{
    DIR *folder;
    FILE *input;
    struct dirent *entry;
    int files = 0;
    
    char directory[200];
    sprintf(directory, "/Users/margherita/Desktop/UNIPI/QUINT_ANNO/Ising/Nlatt = %d/Decorrel", L);
    folder = opendir(directory);
    if(folder == NULL)
    {
        perror("Oh Fra guarda che Decorrel non ci entro \n");
        return(1);
    }

    while( (entry=readdir(folder)) )
    {
        files++;
        
        printf("File %3d: %s\n", files, entry->d_name);

        if (!strcmp (entry->d_name, "."))
            continue;
        if (!strcmp (entry->d_name, ".."))
            continue;
        if (!strcmp (entry->d_name, ".DS_Store"))
            continue;
        
 
        char path[200];
        sprintf(path, "/Users/margherita/Desktop/UNIPI/QUINT_ANNO/Ising/Nlatt = %d/Decorrel/%s", L, entry->d_name); //it modifies each time the name of the file to be created
        printf("sto con lei che sta fopenando \n");
        input = fopen(path, "r");
        printf("Io e i miei fra odiamo i puntatori \n");
        if (input == NULL){
            
            fprintf(stderr, "Error : Porcodio\n");
            exit(-1);
        }
        printf("sto per strappare gli scarponi\n");
        bootstrap(input);
    }

    closedir(folder);

    return(0);
}

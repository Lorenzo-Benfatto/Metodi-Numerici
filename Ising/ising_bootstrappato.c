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


#define L 30

void bootstrap(FILE *f){
    FILE *g;
    unsigned long s;
    int i=0,j,k,N=1000,M=100,x,bin=4,taglio=1;         //N = numero di misure, M = numero di resampling, x serve per scanfare i file, i e j indici ricorsivi, bin è il bin del bootstrap, taglio è per levare le misure non termalizzate
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
    sprintf(filemisure, "/mnt/c/Users/aministratore/Documenti/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Bootstrap/Nuove_versioni/Nlatt = %d/Bootstrappo/resample_binned%.3f.txt", L, beta);
    g=fopen(filemisure,"w");
    control_file(g);
    
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
    
    char directory[500];
    sprintf(directory, "/mnt/c/Users/aministratore/Documenti/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Bootstrap/Nuove_versioni/Nlatt = %d/Risultati", L);
    folder = opendir(directory);
    if(folder == NULL)
    {
        perror("Oh Fra guarda che Risultati non ci entro \n");
        return(1);
    }

    while( (entry=readdir(folder)) )
    {
        files++;
        
        printf("File %3d: %s\n", files, entry->d_name);
/*
        if (!strcmp (entry->d_name, "."))
            continue;
        if (!strcmp (entry->d_name, ".."))
            continue;
        if (!strcmp (entry->d_name, ".DS_Store"))
            continue;
        
 */       
        char path[500];
        sprintf(path, "/mnt/c/Users/aministratore/Documenti/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Bootstrap/Nuove_versioni/Nlatt = %d/Risultati/%s", L, entry->d_name); //it modifies each time the name of the file to be created
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
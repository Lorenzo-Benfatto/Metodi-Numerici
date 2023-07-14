#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include"/home/dario/Documents/UNI/Metodi/ran2.h"
#include"/home/dario/Documents/UNI/Metodi/usefulstuff.h"
#include"/home/dario/Documents/UNI/Metodi/listfunction.h"

int N_misure; //N = numero di misure----> lo leggo da file 

void bootstrap(FILE *f, float m, int Nt, int Nx, int Ntm, int Nxm, char dir[100]){
    FILE *xene_mass ,*xene_spat, *xene_temp, *xene_density;
    unsigned long s; //serve per salvarsi il numero casuale enorme che produco
    int i=0,j,k,M=100,x,bin=100,taglio=0;         /*M = numero di resampling, 
    x serve per scanfare i file, i e j indici ricorsivi, bin è il bin del bootstrap, 
    taglio è per levare le misure non termalizzate*/
    int V=N_misure-taglio;
    long int seed = 13876387;         // seed per il ran2()
    float e_mass, e_spat, e_temp, e_density /*cinetica*/;         // risultati del simulatore_boot.c e valore del beta corrente
    int iter;           // risultati del simulatore_boot.c
    float *dati_mass = malloc(sizeof(long double) * (N_misure-taglio));
    float *dati_spat = malloc(sizeof(long double) * (N_misure-taglio));
    float *dati_temp = malloc(sizeof(long double) * (N_misure-taglio));
    float *dati_density = malloc(sizeof(long double) * (N_misure-taglio));

    x=fscanf(f, "%d  %f  %f  %f  %f", &iter, &e_mass, &e_spat, &e_temp, &e_density);

    while (x!=EOF){
            dati_mass[i] = e_mass;
            dati_spat[i] = e_spat;
            dati_temp[i] = e_temp;
            dati_density[i] = e_density;
            x=fscanf(f, "%d  %f  %f  %f  %f", &iter, &e_mass, &e_spat, &e_temp, &e_density);
            i++;
        }
    printf("ok\n");
    /* Questo file va aperto quando sappiamo già il valore di beta*/
    char filemisure[900];
    sprintf(filemisure,"/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/%s/Bootstrap/misure_boot_mass(Ntm=%d,Nxm=%d,Nt=%df,Nx=%d,m=%.3f).txt", dir, Ntm, Nxm, Nt, Nx, m);
    xene_mass=fopen(filemisure,"w");
    control_file(xene_mass);
    
    sprintf(filemisure,"/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/%s/Bootstrap/misure_boot_spat(Ntm=%d,Nxm=%d,Nt=%df,Nx=%d,m=%.3f).txt", dir, Ntm, Nxm, Nt, Nx, m);
    xene_spat=fopen(filemisure,"w");
    control_file(xene_spat);

    sprintf(filemisure,"/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/%s/Bootstrap/misure_boot_temp(Ntm=%d,Nxm=%d,Nt=%df,Nx=%d,m=%.3f).txt", dir, Ntm, Nxm, Nt, Nx, m);
    xene_temp=fopen(filemisure,"w");
    control_file(xene_temp);

    sprintf(filemisure,"/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/%s/Bootstrap/misure_boot_density(Ntm=%d,Nxm=%d,Nt=%df,Nx=%d,m=%.3f).txt", dir, Ntm, Nxm, Nt, Nx, m);
    xene_density=fopen(filemisure,"w");
    control_file(xene_density);
    

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
                    fprintf(xene_mass,"%lf ",dati_mass[s+h+1-N_misure]);
                    fprintf(xene_spat,"%lf ",dati_spat[s+h+1-N_misure]);
                    fprintf(xene_temp,"%lf ",dati_temp[s+h+1-N_misure]);
                    fprintf(xene_density,"%lf ",dati_density[s+h+1-N_misure]);
                 //printf("sono dentro il primo if \n");
                }
                if(s+h<=N_misure-1){
                    fprintf(xene_mass,"%lf ",dati_mass[s+h]);
                    fprintf(xene_spat,"%lf ",dati_spat[s+h]);
                    fprintf(xene_temp,"%lf ",dati_temp[s+h]);
                    fprintf(xene_density,"%lf ",dati_density[s+h]);
                 //printf("sono dentro il secondo if \n");
                }

            }
        }
            fprintf(xene_mass,"\n");
            fprintf(xene_spat,"\n");
            fprintf(xene_temp,"\n");
            fprintf(xene_density,"\n");
        }
    

    free(dati_mass);
    free(dati_temp);
    free(dati_spat);
    free(dati_density);
    fclose(xene_mass);
    fclose(xene_temp);
    fclose(xene_spat);
    fclose(xene_density);
    return;
}


/*==== NOTA: Nel file di input.txt classico non viene specificata la lunghezza del reticolo. Quindi è necessario
crearne uno che la contenga (io l'ho chiamato input2.txt)*/ 
void read_file_input(){ /*Mi serve solo per leggere il numero di misure N e la larghezza del reticolo L,
 gli altri non mi servono a niente, quindi non gli do nemmeno un nome significativo*/
    FILE *in;
    in = fopen("/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/input.txt", "r");
    control_file(in);
    int x, iflag, idec; 
    float extfield;
    x=fscanf(in, "%d  %d  %d  %f", &iflag, &N_misure, &idec, &extfield);
    fclose(in);
    return ;
}


int main(){

    read_file_input();  // funzione per leggere file con variabili di input (numero di misure, lunghezza del lato del reticolo, iflag per la partenza e campo esterno e idecorrel)
    FILE *f;         // puntatore al file degli Nt
    int p, L;         // p serve per scorrere il file degli eta e L è la lunghezza dell'array degli Nt . 
    float Ntm, Nxm;        //sono i primi due valori del file scanfato e sono Nt * m e Ns * m
    float singolom; // valore che leggo come terzo elem del file scanfato, serve solo nel caso di variazione di temperatura
    float m;  //gli assegnerò il valore della massa nel caso scelta=1, quando la massa non è fissa
    int Nx;     //gli assegnerò il valore della Nx ricavato a partire da Nt e m letti da file

    char valori[300];
    printf("######################################################## \n");
    printf("Per bootstrappare il limite al continuo (nt*m, ns*m fisso) digiti 1\n");
    printf("######################################################## \n");
    printf("Per bootstrappare la variazione di temperatura (Nt variabile, m fisso) digiti 0\n");
    printf("######################################################## \n");
    printf("######################################################## \n");
    printf("Per bootstrappare entrambe digiti 2\n");
    printf("######################################################## \n");

    int scelta; 
    scanf("%d", &scelta);       //scegli quale simulazione fare digitando da tastiera

    sprintf(valori,"/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/valori.txt"); //file degli Nt
    

    if(scelta==1 || scelta==2){
        printf("############################################################# \n");
        printf("Ha scelto di simulare il limite al continuo, buona giornata\n");
        printf("############################################################# \n");

        f=fopen(valori,"r");
        control_file(f);

        list *Nt=NULL;           // lista contenente i valori degli Nt, letti da file
    

        Nt = scan_file(f,Nt);         // lettura degli valori degli nt, con funzione ("listfunction.h")
        L=count(Nt)-3;         // funzione che conta la lunghezza di una lista ("listfunction.h")
        Ntm = val_posizione(0,Nt);
        Nxm = val_posizione(1,Nt);
        singolom = val_posizione(2,Nt);
        Nt = move_to_position(3,Nt);  //da qui in poi i valori della lista sono solo Nt
        
        FILE *misure[L];          // puntatore per i file che salvano gli obs, per ciascun Nt
        p=0;
        while(Nt!=NULL){
            char filemisure[500];
            //ricavo valore della massa e dell?'Nx da Ntm e Nt
            m=Ntm/(Nt->val);   
            Nx = (int)(Nxm/m);

            sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/Continuo/misure(Ntm=%.0f,Nxm=%.0f,Nt=%.0f,Nx=%d,m=%.3f).txt", Ntm, Nxm, Nt->val, Nx, m); //it modifies each time the name of the file to etae created
            misure[p]=fopen(filemisure, "r");
            control_file(misure[p]);
            char dir[100]="Continuo/";

            bootstrap(misure[p], m, (int)Nt->val, Nx, Ntm, Nxm, dir); 
             /*eseguo la simulazione per il valore di nt in causa. 
            Funzione di scalar2d.h Mi creerà automaticamente un file di misure contenente i valori delle osservabili che
            ci interessano e associato per ogni misura fatta. */
     
            printf("Ho terminato il valore %f\n", Nt->val);     

            fclose(misure[p]);
            Nt=Nt->next;
            p++;
        }
        fclose(f);
    }


//Questo secondo caso è analogo al primo solo che uso un nome diverso per il puntatore a lista perché nel 
//caso in cui scelga di fare entrambe le simulazioni altrimenti la lista parte da NULL e non gli piace

    if (scelta == 0 || scelta == 2){
        printf("######################################################## \n");
        printf("Ha scelto di variare la temperatura, buona giornata\n");
        printf("######################################################## \n");

        f=fopen(valori,"r");
        control_file(f);
        list *Nt=NULL;
        Nt = scan_file(f,Nt);         // lettura degli valori degli nt, con funzione ("listfunction.h")
        L=count(Nt)-3;         // funzione che conta la lunghezza di una lista ("listfunction.h")
        Ntm = val_posizione(0,Nt);
        Nxm = val_posizione(1,Nt);
        singolom = val_posizione(2,Nt);
        Nt = move_to_position(3,Nt);  //da qui in poi i valori della lista sono solo Nt


        p=0;
        FILE *misure[L];          // puntatore per i file che salvano gli obs, per ciascun eta
        while(Nt!=NULL){
            Nx = Nt->val;
            char filemisure[500];
            Ntm = Nt->val * singolom;
            Nxm = Ntm;
            sprintf(filemisure, "/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/Temperatura/misure(Ntm=%.0f,Nxm=%.0f,Nt=%.0f,Nx=%d,m=%.3f).txt", Ntm, Nxm,Nt->val, Nx, singolom); //it modifies each time the name of the file to etae created
            misure[p]=fopen(filemisure, "r");
            control_file(misure[p]);
            char dir[100]="Temperatura/";

            bootstrap(misure[p], singolom, (int)Nt->val, (int)Nx, Ntm, Nxm, dir); 
             /*eseguo la simulazione per il valore di eta in causa. 
            Funzione di oscillazione.h Mi creerà automaticamente un file di misure contenente i valori delle osservabili che
            ci interessano (y^2medio, Dy^2medio, ymedio) e eta associato per ogni misura fatta. */
            printf("Ho terminato il valore %f\n", Nt->val);

            fclose(misure[p]);
            Nt=Nt->next;
            p++;
        } 
        fclose(f);      
    }


    printf("############################################################# \n");
    printf("Ci auguriamo sia stata una piacevole esperienza, arrivederci!\n");
    printf("############################################################# \n");
    return 0;
}
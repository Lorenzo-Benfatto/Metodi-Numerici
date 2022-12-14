#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"ran2.h"

/* Programma per la simulazione del modello di ising bidimensionale con possibilita` di inserire un campo magnetico esterno */


/* ------------------------------------- DICHIARAZIONE DELLE VARIABILI GLOBALI ------------------------------------------- */
#define N 50
int nlatt = N, nvol = N*N; //dimensioni del reticolo (lunghezza del lato e volume)
int field[N][N]; //campo
float beta, extfield; //beta è il valore della riga considerata dell'array dei beta (beta_array)
float xmagn, xene;
int M = 10000; //per il generatore casuale tra 0 e 1
int npp[N], nmm[N]; //array per definire le posizioni dei primi vicini del lattice
long int seed;

// ! RICORDARE DI CAMBIARE ANCHE LA LUNGHEZZA DELL'ARRAY DEI BETA, NEL MAIN !


/* ------------------------------------- INIZIO SUBROUTINES ------------------------------------------- */


/* per ogni coordinata definisco il passo in avanti o indietro con le opportune condizioni al bordo*/
void geometry(){
    
    for(int i=0; i<nlatt; i++){
        npp[i] = i + 1;
        nmm[i] = i - 1;
    }
    
    npp[nlatt-1] = 0; //Condizioni al
    nmm[0] = nlatt-1; //bordo periodiche
    
    return;
        
}

/* -------------------------*/

/* Assegno la configurazione di partenza della catena di Markov */
void initialize_lattice(int iflag){
    
    float x;
    
    /* PARTENZA A FREDDO (tutti gli spin a 1, o -1, come se fosse T = 0) */
    if(iflag == 0){
        for(int i = 0; i<nlatt; i++){
            for(int j = 0; j<nlatt; j++){
                
                field[i][j] = 1;
            }
        }
    }
    /* PARTENZA A CALDO (spin random, come se fosse T = infinito) */
    if(iflag == 1){
        for(int i = 0; i<nlatt; i++){
            for(int j = 0; j<nlatt; j++){
                
                float x;
                seed = i;
                x = ran2(&seed);
                field[i][j] = 1;
                if(x<0.5) field[i][j] = -1;
            }
        }
    }
    
    // AGGIUNGERE IF PER RIPARTIRE DALLA CONFIGURAZIONE PRECEDENTE
    
    
    return;
}


/* -------------------------*/

/* Calcolo magnetizzazione media del reticolo */
void magnetization(float xmagn){
    
    xmagn = 0; // inizializzazioine xmagn
    
    for(int i = 0; i<nlatt; i++){            // faccio il loop su
        for(int j = 0; j<nlatt; j++){        // su tutto il reticolo
            
            xmagn = xmagn + field[i][j];     // e sommo su tutti i valori del campo
        }
    }
    xmagn = xmagn/nvol;                      // normalizzo dividendo per il volume
    return;
}

/* -------------------------*/

/* Energia media (= 0 per configurazione ordinata e campo esterno 0) */
void energy(float xene){
    
    int ip, im, jp, jm;
    float force;
    
    xene = 0;                             // inizializzazioine xene
    
    for(int i = 0; i<nlatt; i++){
        for(int j = 0; j<nlatt; j++){
            
            ip = npp[i];        // calcolo coordinate
            im = nmm[i];        // dei primi vicini
            jp = npp[j];        // del sito
            jm = nmm[j];
            
            force = field[i][jp] + field[i][jm] + field[ip][j] + field[im][j]; // somma dei 4 primi vicini
            xene = xene - 0.5*force*field[i][j]; // 1/2 per il conteggio giusto
            xene = xene - extfield*field[i][j]; // contributo campo esterno
        }
    }
    
    xene = xene/nvol;       // normalizzo dividendo per il volume
    return;
}

/* -------------------------*/

/* faccio aggiornamenti locali delle variabili di spin con metropolis, la variabile di spin di prova e` sempre quella opposta a quella attuale */
void update_metropolis(float beta){
    
    int i, j, ip, im, jp, jm;
    float force, phi, p_rat, x, a, b, acc = 0;
    
    for(int ivol=0; ivol<nvol; ivol++){     // loop su tutti i siti del reticolo
        
        /* sclego a caso un sito del reticolo */
        
        seed = ivol;
        
        i = ran2(&seed)*nlatt;                       // scelgo a caso un sito del reticolo
        
    
        j = ran2(&seed)*nlatt;
        
        /*
        i = rand()%nlatt;                   // genera direttamente un numero intero tra 0 e nlatt
        j = rand()%nlatt;
        */
        ip = npp[i];                        // calcolo le coordinate dei quattro primi vicini
        im = nmm[i];                        // del sito che ho scelto
        jp = npp[j];
        jm = nmm[j];
        
        force = field[i][jp] + field[i][jm] + field[ip][j] + field[im][j];
        force = beta*(force+extfield);
        
        phi = field[i][j];
        
        p_rat = exp(-2*phi*force);
        
    
        x = ran2(&seed);

        
        
        if(x<p_rat) {
            field[i][j] = - phi;
        }
        
    }

    return;
}


    /* ------------------------------------- FINE SUBROUTINES ------------------------------------------- */

    /* ------------------------------------- INIZIO MAIN ------------------------------------------- */

int main(void){
    
    FILE *f, *m, *b, *e, *l;
    int x, y, z; //variabili per leggere il file
    int iflag, measures, i_decorrel; //nomi dei parametri della simulazione
                                    //(iflag per partenza caldo(1), freddo(0),...| measures = n° misure|
                                    // i_decorrel = numero di passi della catena Markov tra una misura e l'altra)
    int i=0; //variabile per scorrere while
    int n=3; //lunghezza dell'array dei beta (! DA VARIARE)
    float extfield, beta_array[n]; //nomi dei parametri della simulazione (beta = 1/kT | extfield = campo esterno)
    float valore1=0.2, valore2=0.44, valore3=0.6; //valori interessanti di beta
    
    
    
    
    /* apertura dei file di input e di output */
    f=fopen("input.txt","r");
    if(f==NULL){
     perror("File non correttamente aperto");
     exit(1);
    }
    b=fopen("beta.txt","r");
    if(b==NULL){
     perror("File non correttamente aperto");
     exit(2);
    }
    m=fopen("data_mag.txt","w");
    if(m==NULL){
     perror("File non correttamente aperto");
     exit(3);
    }
    e=fopen("data_ene.txt","w");
    if(e==NULL){
     perror("File non correttamente aperto");
     exit(4);
    }
    
    /* lettura dei parametri della simulazione */
    x = fscanf(f, "%d  %d  %d  %f", &iflag, &measures, &i_decorrel, &extfield);
    y = fscanf(b, "%f", &beta);
    while (y!=EOF){
            beta_array[i]=beta;
            y=fscanf(b, "%f", &beta);
            i++;
        }
    
    /* OPERAZIONI PRELIMINARI */
    geometry(); //inizializza condizioni al bordo
    initialize_lattice(iflag); //inizializza configurazione iniziale
    
    for(int i=0; i<n; i++){ //si scorre l'array dei beta
        
        for(int iter=0; iter<measures; iter++){
            
            /* AGGIORNAMENTO CONFIGURAZIONE: i_decorrel spazza tutto il reticolo con l'algoritmo scelto */
            for(int idec=0; idec<i_decorrel; idec++){
                update_metropolis(beta_array[i]);
            }
            
            /* MISURA DELLE VARIABILI FISICHE */
            magnetization(xmagn);
            energy(xene);
            
            /* SCRIVO LE MISURE SU FILE PER POI EFFETTUARE L'ANALISI */
            fprintf(e, "%f ", xene);
            fprintf(m, "%f ", xmagn);
        }
        
        fprintf(e, "\n");
        fprintf(m, "\n");
        
        /* --------------------------- TERMINE SIMULAZIONE MONTECARLO --------------------------- */
        /* SALVO CONFIGURAZIONE PER POTER EVENTUALMENTE RIPARTIRE */
        
        
        if(beta_array[i]==valore1 || beta_array[i]==valore2 || beta_array[i]==valore3){
            if(beta_array[i]==valore1){
                l=fopen("beta1.txt","w");
            }
            else if(beta_array[i]==valore2){
                l=fopen("beta2.txt","w");
            }
            else if(beta_array[i]==valore3){
                l=fopen("beta3.txt","w");
            }
            else if(l==NULL){
             perror("File non correttamente aperto");
             exit(5);
            }
            for(int j=0; j<nlatt; j++){
                for(int k=0; k<nlatt; k++){
                    fprintf(l, "%d ", field[j][k]);
                    }
                fprintf(l,"\n");
                }
            fclose(l);
            }
        
        /* --------------- FINE DEL SALVATAGGIO DELLA CONFIGURAZIOINE ATTUALE --------------- */
        
        }
        
    
    fclose(m);
    fclose(e);
    fclose(f);
    fclose(b);
	
	return 0;
}



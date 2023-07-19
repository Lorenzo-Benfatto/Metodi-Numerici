/* CC    per far girare il codice dopo averlo compilato, e` necessario che
CC    esista un file "input" contenente i 5 numeri (3 interi e 2 reali) letti
CC    poche righe piu` sotto, ed un file "randomseed" contenente lo stato
CC    precedente del generatore random oppure il nuovo seme
CC    (un numero intero qualsiasi)

CC    inoltre se si mette come "iflag" di partenza un numero che non sia
CC    0 o 1, prova a leggere la configurazione di spin dal file "lattice"
CC    che quindi deve gia` esistere */

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


/* VARIABILI MACRO */
#define Nlatt 10000
#define _USE_MATH_DEFINES


/* VARIABILI GLOBALI */
float xene_mass = 0, xene_spat = 0, xene_temp = 0, xene_density=0;
int nx, nt;
float mass, extfield, mass2, mass2p4;
float field[Nlatt][Nlatt];
int npp[Nlatt][2], nmm[Nlatt][2];


int i_flag, i_decorrel, measures;
long int seed = 43456789;

void geometry(){
    
/* !! le funzioni npp ed nmm sono costruite come dei vettori
!! di interi, in modo da non essere ricalcolate ogni volta
!! e rendere tutto piu` efficiente, prendono in input una
!! coordinata e restituiscono la coordinata in avanti o
!! indietro, tenendo conto delle opportune condizioni

!! le direzioni possono avere lunghezze diverse quindi mi servono
!! 2 tabelle diverse */
    
    for(int i = 0; i<nx; i++){
        npp[i][0] = i+1;
        nmm[i][0] = i-1;
    }
    
    npp[nx-1][0] = 0;             // RIAGGIUSTO IL CALCOLO AI BORDI PER TENERE
    nmm[0][0] = nx-1;             // CONTO DELLE CONDIZIONI AL BORDO PERIODICHE
    
    for(int i = 0; i<nt; i++){
        npp[i][1] = i+1;
        nmm[i][1] = i-1;
    }
    
    npp[nt-1][1] = 0;             // RIAGGIUSTO IL CALCOLO AI BORDI PER TENERE
    nmm[0][1] = nt-1;             // CONTO DELLE CONDIZIONI AL BORDO PERIODICHE
    
    return;
    
}


void initialize_lattice(int iflag){
    
    //ASSEGNO LA CONFIGURAZIONE DI PARTENZA DELLA CATENA DI MARKOV
    float x;
    
    // PARTENZA A FREDDO (tutti gli spin a 1, o -1, come se fosse T = 0)
    if(iflag == 0){
        for(int ix = 0; ix<nx; ix++){
            for(int it = 0; it<nt; it++){
                
                field[ix][it] = 0;
            }
            
        }
    }
    // PARTENZA A CALDO (spin random, come se fosse T = infinito)
    if(iflag == 1){
        for(int ix = 0; ix<nx; ix++){
            for(int it = 0; it<nt; it++){
                //float x;
                //seed = i;
                x = 1-2*ran2(&seed);
                //printf("%lf", x);
                field[ix][it] = x;
                //printf("%f\n", field[i]);
                
            }
        }
    }
    
    // AGGIUNGERE IF PER RIPARTIRE DALLA CONFIGURAZIONE PRECEDENTE

    return;
    
}

void energy(){
    int nvol =nx*nt;

    xene_mass = 0;
    xene_spat = 0;
    xene_temp = 0;
    xene_density = 0;
    
    float force_s, force_t, phi;
    
    // Loop su tutti i siti del reticolo
    for(int ix = 0; ix<nx; ix++){
        for(int it = 0; it<nt; it++){
            
            phi = field[ix][it];
            force_s = field[npp[ix][0]][it];
            force_t = field[ix][npp[it][1]];
            
            xene_mass = xene_mass + mass2*phi*phi; //pow(phi,2);
            xene_spat = xene_spat - 2*phi*force_s + 2*phi*phi; //*pow(phi,2);
            xene_temp = xene_temp - 2*phi*force_t + 2*phi*phi; //pow(phi,2);
        }
    }
    float Vol;
    Vol=(float)nvol;
    xene_mass = xene_mass/Vol;   // normalizzo -> densita` di energia massa
    xene_spat = xene_spat/Vol;   // normalizzo -> densita` di energia potenziale
    xene_temp = xene_temp/Vol;  // normalizzo -> densita` di energia cinetica
    xene_density= (xene_mass + xene_spat - xene_temp);


// differenza tra le osservabili

    return;
    
}


void update_heatbath(){
    
    float force, sigma2, y, x, aver, phi;
    
    for(int ix = 0; ix<nx; ix++){
        for(int it = 0; it<nt; it++){
            
            aver=0;
            force = 0;
            phi = field[ix][it];
            force   = force   + field[npp[ix][0]][it];
            force   = force   + field[nmm[ix][0]][it];
            force   = force   + field[ix][npp[it][1]];
            force   = force   + field[ix][nmm[it][1]];
            
            sigma2 = 1/mass2p4;  // variance of the gaussian distribution
                                                   //is 1/(m^2 + 4)

            aver = force*sigma2;   // average of the gaussian distribution
                                                   // is force/(m^2 + 4)
                             
 //            write(*,*) mass2p4, aver, force
            x = sqrt(sigma2)*sqrt(-2*log(ran2(&seed))); //
            y = x*cos(2*M_PI*ran2(&seed)) + aver ; //  BOX MULLER ALGORITHM

                
            field[ix][it] = y;
            
        }
    }
    
    return;
}


void update_overrelax(){
    
    float force, phi, aver;
        
    for(int ix = 0; ix<nx; ix++){
        for(int it = 0; it<nt; it++){
            
            force = 0;
            phi = field[ix][it];
            force   = force   + field[npp[ix][0]][it];
            force   = force   + field[nmm[ix][0]][it];
            force   = force   + field[ix][npp[it][1]];
            force   = force   + field[ix][nmm[it][1]];
            
            aver = force/mass2p4;  // average of the gaussian distribution
                                                   // is force/(m^2 + 4)
                             
            field[ix][it] = 2*aver - phi;
            
            
        }
    }
    
    return;
}

void Scalar_2D(FILE* mis,int Nt, int Nx, float mass){
    
    nx=Nx;
    nt=Nt;

    FILE *input;
    int x;
    int iter=0;
    

//  apertura file da dove leggere i parametri della simulazione
    input = fopen("/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/input.txt","r");

    control_file(input);
    
    x= fscanf(input, "%d  %d  %d  %f", &i_flag, &measures, &i_decorrel, &extfield/*, &i_term*/);
    
    mass2   = pow(mass,2);
    mass2p4 = pow(mass,2) + 4;
    
    
    geometry();
    
/*
    initialize_lattice(i_flag);

    for(int term=0; term<i_term; term++){
        update_heatbath();
        update_overrelax();
        update_overrelax();
        update_overrelax();
        update_overrelax();

    }

*/
    for(iter = 0; iter<measures; iter++){
        
        // AGGIORNAMENTO CONFIGURAZIONE: i_decorrel spazzate di tutto il reticolo
        for(int j = 0; j<i_decorrel; j++){
            
            update_heatbath();
            update_overrelax();
            update_overrelax();
            update_overrelax();
            update_overrelax();
            
        }
        
        
        //   MISURA DELLE OSSERVABILI FISICHE
        energy();  // xene mass

        
        fprintf(mis,"%d   %lf    %lf   %lf  %lf \n", iter, xene_mass, xene_spat, xene_temp, xene_density); //prendo misure a questa configurazione
        //printf("%d   %lf    %lf   %lf  %lf \n", iter, xene_mass, xene_spat, xene_temp, xene_density);
    }
    return;
}

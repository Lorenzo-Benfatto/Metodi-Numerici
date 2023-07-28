#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"/home/dario/Documents/UNI/Metodi/ran2.h"
#include"//home/dario/Documents/UNI/Metodi/usefulstuff.h"
#include"/home/dario/Documents/UNI/Metodi/listfunction.h"
/* Programma per la simulazione dell'oscillatore armonico*/

#define Nmax 100000
int Nlatt;
long int seed = 43456789;//33456789;
float d_metro; //eta=a*omega = parametro reticolo * pulsazione;  d_metro = parametro del metropolis = 2*sqrt(eta)
int iflag, measures, i_decorrel, i_term;  //vedi ising. i_term = passo di termalizzazione
int npp[Nmax], nmm[Nmax]; //array per definire le posizioni dei primi vicini del lattice
float field[Nmax];  //array in cui salviamo le Nlatt posizioni del percorso
unsigned long acc=0;  //intero che mi calcola quante volte ho accettato il metropolis, per poi calcolare l'accettanza



/* per ogni coordinata definisco il passo in avanti o indietro con le opportune condizioni al bordo*/
void geometry(){
    
    for(int i=0; i<Nlatt; i++){
        npp[i] = i + 1;
        nmm[i] = i - 1;
    }
    
    npp[Nlatt-1] = 0; //Condizioni al
    nmm[0] = Nlatt-1; //bordo periodiche
    
    return;
        
}

/* --------------- REFUSO DAL MODELLO DI ISING ------------------*/

/* Assegno la configurazione di partenza della catena di Markov */


void initialize_lattice(int iflag){
    float x;
    
    // PARTENZA A FREDDO (tutti gli spin a 1, o -1, come se fosse T = 0)
    if(iflag == 0){
        for(int i = 0; i<Nlatt; i++){
                
            field[i] = 0;
            
        }
    }
    // PARTENZA A CALDO (spin random, come se fosse T = infinito)
    if(iflag == 1){
        for(int i = 0; i<Nlatt-1; i++){
            //float x;
            //seed = i;
            x = 1-2*ran2(&seed);
            field[i] = x;
            //printf("%f\n", field[i]);
        }
        field[Nlatt-1]=field[0];
    }
    
    // AGGIUNGERE IF PER RIPARTIRE DALLA CONFIGURAZIONE PRECEDENTE
    
    
    return;
}



/*funzione per avanzare col metropolis e modificare la configurazione*/

void update_metropolis(float eta){
    float c1, c2; //sono solo shortcut per scrivere 1/eta e altra funz di eta
    int ip, im; //coordinate dei 2 primi vicini
    float force, phi, phi_prova;  //force = forza del campo intorno; phi = valore attuale del campo; phi_prova = valore di prova del campo
    float p_rat, x; // p_rat = valore di energia con cui confrontare x che è estratto a caso e mi dirà se accettare il passo o no
    
    c1 = 1/eta;
    c2 = 1/eta + eta/2;

    /*loop su tutti i siti, qui il sito non è scelto a caso ma faccio una spazzata
    iterativa su tutti i siti, si può dimostrare che va bene lo stesso per il bilancio dettagliato,
    ma meno banale da provare*/
    
    for(int i = 0; i<Nlatt; i++){
        ip = npp[i];
        im = nmm[i];
        force = field[ip] + field[im];

        phi = field[i];
        phi_prova = phi + 2*d_metro*(0.5 - ran2(&seed));  // Il secondo termine serve per traslare nell'intervallo [-d_metro, d_metro]. Calcolo del phi di prova per trovare la probabilità e decidere se accettare il passaggio o no
        p_rat= c1 * phi_prova * force - c2*pow(phi_prova,2);
        p_rat= p_rat - c1*phi*force + c2*pow(phi,2);
        //printf("%f\n", p_rat);
        x = log(ran2(&seed));  //METRO-TEST!
        
        if(x<p_rat){
          field[i]=phi_prova;  //test accettanza, se p_rat>1 accetto
          acc++;
        }
        
    }
    
    return ;
}

//funzione che prende le misure delle osservabili che ci interessano

double * measure(double obs[3]){
    obs[0]=0;
    obs[1]=0;
    obs[2]=0;
    for(int i=0; i<Nlatt; i++){
        obs[0]=obs[0] + pow(field[i],2);
        obs[1] = obs[1] + pow(field[i]-field[npp[i]],2);
        obs[2] =obs[2] + field[i];
    }

    obs[0] = obs[0]/(float)Nlatt;  //media sul singolo path di y**2 sommato su tutti i path
    obs[1] = obs[1]/(float)Nlatt;  //media sul singolo path di Delta y**2 sommato su tutti i path
    obs[2] = obs[2]/(float)Nlatt;  //media sul singolo path di field

    //printf("%lf  %lf\n", obs[0], obs[1]);

    return obs;
}



/*=================================== SIMULAZIONE =============================================*/

// y è Nlatt o eta e viene letto come lista dopo primovalore dal file valori.txt

void Harmonic_metropolis(float y, FILE *misure, int scelta, float primovalore){
    FILE * lat, *input; // file in cui stampo il field (lat) e da cui prendo i valori iniziali ( init )
    int x; //per leggere l'init.txt; l è l'Nlatt che qui non serve a niente
    //OPERAZIONI PRELIMINARI
    float eta;
    input = fopen("/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/input.txt","r");
    //printf("1\n");
    control_file(input);
    //printf("4\n");
    x= fscanf(input, "%f  %d  %d  %d  %d", &d_metro, &measures, &i_decorrel, &iflag, &i_term);
    //printf("5\n");
    //////////////////////////////////////////////////////////////
    //Da scommentare nel caso si voglia stampare il path ad ogni update_metropolis

    /*char path[200];
    sprintf(path,"/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/Path/lattice_eta=%.1f.txt",eta);
    lat = fopen(path,"w" );
    //printf("6\n");
    control_file(lat);
    //printf("7\n");*/
    //////////////////////////////////////////////////////////////

    if(scelta==1){
        Nlatt=y;
        eta=primovalore/Nlatt;
        if(eta<0.01) d_metro=0.15;
        else if (eta<0.03 && eta>=0.01) d_metro=0.25;
        else if (eta<0.05 && eta>=0.03) d_metro=0.3;
        else if(eta<0.09 && eta>=0.05) d_metro=0.5;
        else if(eta>=0.09 && eta<0.15) d_metro=0.8;
        else if(eta>=0.15 && eta<0.45) d_metro=1;
        else if(eta>=0.45) d_metro=1.5;
        //d_metro=eta*20;
        Nlatt=(int)Nlatt;
        
    }

// DALLE VARIE PROVE TROVO UN'ACCETTENZA INTORNO AL 37% SE DELTA = 0.3 PER NLATT = 300,500,700. POTREI PROVARE A VEDERE SE MIGLIORA, METTENDO 0.25 ANCHE PER QUESTI CASI. MENTRE PER NLATT = 200 E DELTA = 0.25 HO 43% E PER NLATT = 100, 500, 700, 1000 E DELTA = 0.3 HO 37%
    else {
        Nlatt=y;
        eta=primovalore;
        if (eta<=0.005) d_metro=0.08;
        else if(eta<=0.01 && eta>0.005) d_metro=0.15;
        else if (eta<0.03 && eta>0.01) d_metro=0.25;
        else if (eta<0.05 && eta>=0.03) d_metro=0.3;
        else if(eta<0.09 && eta>=0.05) d_metro=0.5;
        else if(eta>=0.09 && eta<0.15) d_metro=0.8;
        else if(eta>=0.15 && eta<0.45) d_metro=1;
        else if(eta>=0.45) d_metro=1.5;
    }

    initialize_lattice(iflag);
    geometry();
    //printf("8\n");
    //SESSIONE ALL'EQUILIBRIO con MISURE

    printf("%f",eta);
    for (int i = 1; i<i_term+1; i++){
        update_metropolis(eta);
    }

    acc=0;
    double obs[3]={0,0,0};
    double *results;
    for (int iter=0; iter<measures; iter++){
        // AGGIORNAMENTO CONFIGURAZIONE
        for(int idec=0; idec<i_decorrel; idec++ ){
            update_metropolis(eta);
            }
        results=measure(obs);
        //printf("%lf  %lf  %d\n", obs[0], obs[1], iter);
        fprintf(misure,"%lf  %lf %lf  %d\n", results[0], results[1], results[2], iter); //prendo misure a questa configurazione
    }

    float accettanza = 0;
    unsigned long long num_updates = 0;
    num_updates=i_decorrel*(Nlatt);//(Nlatt-2);
    num_updates = measures*num_updates;
    accettanza=(float)acc*100/measures;
    accettanza=(float)accettanza/i_decorrel;
    accettanza=(float)accettanza/Nlatt;

    printf("\n \n");

    printf("delta=%f, eta=%f, Nlatt=%d, accettanza=%.2f percento,\n numero di update = %llu, numero di accettati %lu\n", d_metro, eta, Nlatt, accettanza, num_updates, acc);
    printf("misure: %d, idec: %d, Nlatt: %d \n", measures, i_decorrel, Nlatt);
    //fclose(lat);
    fclose(input);
    return;

}

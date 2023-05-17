#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"/home/dario/Documents/UNI/Metodi/ran2.h"
#include"/home/dario/Documents/UNI/Metodi/usefulstuff.h"
#include"/home/dario/Documents/UNI/Metodi/listfunction.h"
/* Programma per la simulazione dell'oscillatore armonico*/

#define N 10
int Nlatt=N;
long int seed = 13;
float eta, d_metro; //eta=a*omega = parametro reticolo * pulsazione;  d_metro = parametro del metropolis = 2*sqrt(eta) 
int iflag, measures, i_decorrel, i_term;  //vedi ising. i_term = passo di termalizzazione
int npp[N], nmm[N]; //array per definire le posizioni dei primi vicini del lattice
float field[N];


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

void update_metropolis(){
    float c1, c2; //sono solo shortcut per scrivere 1/eta e altra funz di eta
    int ip, im; //coordinate dei 2 primi vicini
    float force, phi, phi_prova;  //force = forza del campo intorno; phi = valore attuale del campo; phi_prova = valore di prova del campo
    float p_rat, x; // p_rat = valore di energia con cui confrontare x che è estratto a caso e mi dirà se accettare il passo o no
    int var;

    c1 = 1/eta;
    c2 = 1/eta + eta/2;

    /*loop su tutti i siti, qui il sito non è scelto a caso ma faccio una spazzata 
    iterativa su tutti i siti, si può dimostrare che va bene lo stesso per il bilancio dettagliato, 
    ma meno banale da provare*/
    
    for(int i = 1; i<Nlatt-1; i++){
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
          var=1;
      	}
        else {
        	var=0;
        }        
    }
    
    return;
}

//funzione che prende le misure delle osservabili che ci interessano

double * measure(double obs[3]){

    for(int i=0; i<Nlatt; i++){
        obs[0]=obs[0] + pow(field[i],2);
        obs[1] = obs[1] + pow(field[i]-field[npp[i]],2);
        obs[2] =obs[2] + field[i];
    }

    obs[0] = obs[0]/Nlatt;  //media sul singolo path di y**2 sommato su tutti i path
    obs[1] = obs[1]/Nlatt;  //media sul singolo path di Delta y**2 sommato su tutti i path
    obs[2] = obs[2]/Nlatt;  //media sul singolo path di field

    //printf("%lf  %lf\n", obs[0], obs[1]);

    return obs;
}


/*=================================== SIMULAZIONE =============================================*/

void Harmonic_metropolis(float eta, FILE *misure){
    FILE * lat, *input; // file in cui stampo il field (lat) e da cui prendo i valori iniziali ( init )
    int x, l; //per leggere l'init.txt; l è l'Nlatt che qui non serve a niente
    //OPERAZIONI PRELIMINARI
    input = fopen("/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/input.txt","r");
    printf("1\n");
    control_file(input);
    printf("2\n");
//   misure = fopen("/mnt/c/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-3/Nuova_run/Oscillo/misure(eta=%f).txt","w" );
//    printf("3\n");
//   control_file(misure);
    printf("4\n");
    x= fscanf(input, "%f  %d  %d  %d  %d  %d", &d_metro, &measures, &i_decorrel, &iflag, &i_term, &l);
    printf("5\n");
    lat = fopen("/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/lattice.txt","w" );
    printf("6\n");
    control_file(lat);
    printf("7\n");

    initialize_lattice(iflag);
    geometry();
    printf("8\n");
    //SESSIONE ALL'EQUILIBRIO con MISURE
    double obs[3]={0,0,0};
    for (int iter=0; iter<measures; iter++){
        // AGGIORNAMENTO CONFIGURAZIONE
        for(int idec=0; idec<i_decorrel; idec++ ){
            update_metropolis();
            //printf("si %d\n", idec);
            for (int i = 0; i < Nlatt; i++){  //stampo su file il field ma non so a cosa serva e cosa si debba vedere
                fprintf(lat, "%f  ", field[i]);}
            
            fprintf(lat, "\n");
            }
        obs[0] = measure(obs)[0];
        obs[1] = measure(obs)[1];
        obs[2] = measure(obs)[2];
        //printf("%lf  %lf  %d\n", obs[0], obs[1], iter);
        fprintf(misure,"%lf  %lf %lf  %d\n", obs[0], obs[1], obs[2], iter); //prendo misure a questa configurazione
    }

    //fclose(lat);
    fclose(misure);
    fclose(input);
    return;

}
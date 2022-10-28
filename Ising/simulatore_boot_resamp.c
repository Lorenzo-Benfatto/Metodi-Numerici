#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"listfunction.h"
#include"simu_boot_resamp.h"
#include "usefulstuff.h"


/* IN QUESTO CODICE VIENE ESEGUITA LA SIMULAZIONE MONTECARLO PER VARI VALORI DI BETA, RIPORTATI NEL FILE "beta.txt".
PER OGNI VALORE VIENE SALVATA LA CONFIGURAZIONE FINALE DEL RETICOLO NEI FILE "betai.txt" E NEL FILE "medie.txt"
SONO RIPORTATE LE MEDIE DELLA MAGNETIZZAZIONE E DELL'ENERGIA PER CIASCUN VALORE DI BETA */

int main(void){
    FILE *f,*g, *h;         // puntatori ai file dei beta e delle medie
    int p=0, L, s=0, L1;         // p serve per scorrere il file dei beta e L è la lunghezza dell'array dei beta. s serve a scorrere i file di beta di cui voglio stampare il lattice
    //float beta_interesting[3]={0.395,0.44,0.5};   //array contenente i beta di cui voglio vedere il lattice. Assicurarsi che i valori inseriti siano presenti nel file beta_init.txt

    f=fopen("beta.txt","r");
    control_file(f);
    char filemedie[200];
    sprintf(filemedie, "/Users/margherita/Desktop/UNIPI/QUINT_ANNO/Ising/Nlatt = %d/medie.txt",N);
    g=fopen(filemedie,"w");
    control_file(g);
    h=fopen("decorr.txt","r");
    control_file(h);
    
    list_int *i_decorrel=NULL;
    i_decorrel = scan_file_interi(h,i_decorrel);
    L1 = count_int(i_decorrel);
    FILE *decorr[L1];

    list *b=NULL;           // lista contenente i valori dei beta, letti da file
    b = scan_file(f,b);         // lettura dei valori dei beta, con funzione ("listfunction.h")
    L=count(b);         // funzione che conta la lunghezza di una lista ("listfunction.h")
    FILE *misure[L]; //*beta[3],           // puntatore per i file che salvano la configurazione, per ciascun beta
    
    while(b!=NULL){
        
            while(i_decorrel!=NULL){
            
                char filemisure[200];
                sprintf(filemisure, "/Users/margherita/Desktop/UNIPI/QUINT_ANNO/Ising/Nlatt = %d/Decorrel/misure(beta)%.3f(idec)%d.txt", N, b->val, i_decorrel->val); //it modifies each time the name of the file to be created
                misure[p]=fopen(filemisure, "w");
                control_file(misure[p]);

                simulazione(b->val, misure[p], i_decorrel->val); /* eseguo la simulazione per il valore di beta in causa. Funzione di simulazione_boot.h
                Mi creerà automaticamente un file di misure contenente valore di magnetizz, energia e beta associato
                per ogni misura fatta*/
                // fprintf(g," %f %f %f \n",medie[0],medie[1],b->val);  //stampo medie di magn e energia con beta associato in file medie.txt
                
                /*if (b->val==beta_interesting[0]||b->val==beta_interesting[1]||b->val==beta_interesting[2]){ //faccio i file contenenti i lattice interessanti

                    printf("%f\n", b->val);
                       char filename[200];
                    sprintf(filename, "/Users/margherita/Desktop/UNIPI/QUINT_ANNO/Ising/Nlatt = %d/Lattice/beta%.3f.txt", N, b->val); //it modifies each time the name of the file to be created
                      beta[s]=fopen(filename, "w");
                       control_file(beta[s]);

                       for(int j=0; j<nlatt; j++){
                           for(int k=0; k<nlatt; k++){
                               fprintf(beta[s], "%d ", field[j][k]);
                        }
                        fprintf(beta[s],"\n");
                      }
                     fclose(beta[s]);
                     s++;
                  }*/
                 
                fclose(misure[p]);
                 printf("ho fatto l'i_decorrel numero %d, stai sciallo \n", p);
                p++;
                i_decorrel = i_decorrel->next;
        }
        b=b->next;
        
    }
    printf("Tutto a posto \n");
    fclose(f);
    fclose(g);
    fclose(h);
    return 0;
}

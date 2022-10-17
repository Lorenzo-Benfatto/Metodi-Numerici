#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"funzioni.h"
#include"simulazione.h"

int main(void){
	FILE *f,*g;
	float *x;
	int p=0, L;
	f=fopen("beta.txt","r");
	if(f==NULL){
	 perror("File di merda");
	 exit(1);
	}
	g=fopen("medie.txt","w");
	if(f==NULL){
	 perror("File di merda");
	 exit(1);
	}
	list *b=NULL;
	b=scan_file(f,b);
	L=count(b);
	FILE *beta[L];
	while(b!=NULL){
		simulazione(b->val);
		fprintf(g," %f %f %f \n",medie[0],medie[1],b->val);
        	char filename[10];
        	if(b->val==0.36||b->val==0.40||b->val==0.42||b->val==0.44||b->val=0.46||b->val=0.53){
       	 			sprintf(filename, "beta%d.txt", p); //it modifies each time the name of the file to be created
      	 		}
      	 		beta[p]=fopen(filename, "w");
       			if(beta[p]==NULL){ //check if the file is opened correctly
       	     	perror("File non correttamente aperto");
       	     	exit(p);
       	    	}
       	     	for(int j=0; j<nlatt; j++){
       	          	for(int k=0; k<nlatt; k++){
       	          	 	fprintf(beta[p], "%d ", field[j][k]);
        	       	}
        	      	fprintf(beta[p],"\n");
      	  		}
     	   	  fclose(beta[p]);   
     		}
		b=b->next;
		p++;
	}
	printf("Tutto a posto \n");
	fclose(f);
	fclose(g);
	return 0;
}

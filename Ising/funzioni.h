#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

//definisco il tipo lista che contiene elemento numerico float più puntatore a blocchetto successivo 
typedef struct elem {
	float val;
	struct elem *next;
}list;

/*Da qui una serie di funzioni utili per lavorare con le liste.
Ognuna di esse, eccetto le ultime quattro, restituisce un putatore di tipo lista, e scorre gli elementi della lista stessa
attraverso un punttatore di tipo lista che è chiamato tmp sempre*/


/*========================= FUNZIONI PER AGGIUNGERE ELEMENTI ALLA LISTA =================================*/
//funzione per aggiungere un elemento in testa alla lista
list *agg_testa(float n, list *p){ // argomenti sono il valore dell'elemento da aggiungere e il puntatore alla testa lista
	if(p != NULL){
		list *tmp = p; //salvo in tmp l'indirizzo del secondo elem della lista
		p=(list*)malloc(sizeof(list)); // alloco nuovo spazio da aggiungere alla lista, costituirà il nuovo primo elem
		p->val=n; 						//assegno primp elem
		p->next=tmp;				//così lo colloco in testa, dato che punta al secondo elemento
	}
	else{ //se la lista è vuota aggiungo semplicemente un elemento
		p=(list*)malloc(sizeof(list));
		p->val=n;
		p->next=NULL;
	}
	return p;
}

/*funzione per aggiungere un elemento in coda alla lista. 
Sfrutta la funzione scritta sopra per aggiungere in testa, ma lo fa al puntatore next dell'ultimo elem della lista*/
list *agg_coda(float n, list *p){ // argomenti sono il valore dell'elemento da aggiungere e il puntatore alla testa lista
	if(p != NULL){
		list *tmp = p;
		while(tmp->next!=NULL){ //questo ciclo mi serve per arrivare in fondo alla lista
			tmp=tmp->next;
		}
		tmp->next=agg_testa(n, NULL);
	}
	else{ // se la lista è vuota aggiungo semplicemente un elemento
		p=agg_testa(n, p);
	}
	return p;
}

/*funzione per aggiungere un elemento in una posizione specifica della lista.
Anche'essa sfrutta la agg_testa, ma prima, come in agg_coda, deve puntare all'elemento successivo al punto in cui si vuole 
aggiungere*/
list* agg_posizione(float n, int posizione, list* testa){

	if(posizione == 0 || testa == NULL){
		testa = agg_testa(n, testa);
	}
	else if(posizione > 0){
		int i=1;
		list* tmp = testa;
		while(i < posizione && tmp->next != NULL){
			tmp = tmp->next;
			i++;
		}
		tmp->next= agg_testa(n, tmp->next);
	}
	return testa;
}


/*=================================== FUNZIONI PER RIMUOVERE ELEMENTI DALLA LISTA ==============================*/

//funzione per rimuovere il primo elemento della lista
list* rm_testa(list* testa){
	if(testa != NULL){
		list* daEliminare = testa; //salvo primo elem in un puntatore ausiliario, da freeare successivamente
		testa = testa->next; // la nuova testa di lista sarà il secondo blocco della vecchia
		free(daEliminare);
	}

	return testa;
}

//funzione per rimuovere l'ultimo elemento della lista
list* rm_coda(list* testa){
	if(testa != NULL){
		if(testa->next != NULL){

			list* attuale = testa;					//così riesco a scorrere la lista fino al fondo
			list* daEliminare = testa->next;

			while(daEliminare->next != NULL){
				attuale = daEliminare;
				daEliminare = daEliminare ->next; // alla fine l'ultimo elem sta in daEliminare, mentre il penultimo in attuale
			}
	    	attuale->next = rm_testa(daEliminare); //così rimuovo l'ultimo elem considerando una mini lista fatta da 
	    	//due elementi: attuale che punta a daEliminare
		}
		else{
			testa = rm_testa(testa);
		}
	}

	return testa;
}


//funzione per rimuovere un elemneto in posizione specifica, che gli si passa come parametro della funz
//Sfrutta sempre la rm_testa, portandosi a puntare all'elem immediatamente precedente a quello da eliminare
list* rm_posizione(int posizione, list* testa){


	if(posizione == 0 || testa -> next == NULL){
		testa = rm_testa(testa);
	}
	else if(posizione > 0){
		int i=1;

		list* attuale = testa;
		list* daEliminare = testa ->next;

		while(i < posizione && daEliminare->next!= NULL){
                        attuale = daEliminare;
			daEliminare = daEliminare ->next;
			i++;
		}
		attuale ->next= rm_testa( daEliminare ->next);

	}
	return testa;
}

/*========================================== FUNZIONI CHE NON RESTITUISCONO PUNTATORE A LISTA ==============*/

//stampa la lista su terminale
//Non uso un puntatore ausiliario per scorrere la lista, poiché non mi serve tenermi salvato l'indirizzo della testa
void print_lista(list * testa){
	if(testa != NULL){
		printf("TESTA -> ");
		while(testa != NULL){
			printf("%f -> ",testa->val);
			testa = testa->next;
		}
	    printf("NULL \n");
	}
	else {
		printf("LISTA VUOTA \n");
	}
}


//funzione per stampare la lista su file
//presuppone che il file sia già stato aperto nel main in cui viene chiamata
//Non uso un puntatore ausiliario per scorrere la lista, poiché non mi serve tenermi salvato l'indirizzo della testa
void print_su_file(FILE *f, list * testa){ // come argomento ha anche il puntatore al file su cui stampare
	if(testa != NULL){
		while(testa != NULL){
			fprintf(f, "%f -> ",testa->val);
			testa = testa->next;
		}
	}
}


//cerca un elemento specifico, utile ad esempio per le rm_posizione o agg_posizione
// Non uso un puntatore ausiliario per scorrere la lista, tanto ritorna un intero
int cerca(float n,list* testa){
	int posizione = -1;
	int trovato = 0;
	int i=0;
	while(testa != NULL && trovato == 0){
		if(testa->val== n){ //se lo trovo subito ho fatto
			posizione = i;
			trovato = 1;
		}
		i++;
		testa = testa ->next;
	}
	return posizione;
}


//funzione per contare il numero di elementi della lista. Anche qui scorro direttamente con testa
int count(list* testa){
	int i=0;
	while(testa != NULL){
		i++;
		testa = testa -> next;
	}
	return i;
}



//funzione per aggiungere alla lista elementi letti da terminale e li aggiunge in coda. La funzione si blocca non appena 
//verrà digitato il valore "0" che non potrà quindi essere contenuto nella lista. Modificare a piacimento in caso di necessità
list *scan_term(list *d){
	float elem;
	int control=1;
	while (control!=0){
		printf("Digitare valore:");
		scanf("%f", &elem);
		if (elem==0) {
			control=0;
		}
		else{
			d=agg_coda(elem, d);			
			print_lista(d);
		}
	}
	return d;
}	

/*funzione per aggiungere alla lista elementi letti da file (che contenga
 una sola colonna) e li aggiunge in coda. Il file dovrà esser già stato aperto nel main 
 in cui viene chiamata tale funzione*/

list *scan_file(FILE *f,list *d){
	float elem;
	int x;
	x=fscanf(f, "%f", &elem);
	while (x!=EOF){
			d=agg_coda(elem, d);
			x=fscanf(f, "%f ", &elem);
		}
//	print_lista(d);
	return d;
}	

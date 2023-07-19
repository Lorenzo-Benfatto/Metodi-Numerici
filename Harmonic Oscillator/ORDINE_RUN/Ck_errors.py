# PROGRAMMA PER FARE PLOT E FIT DELL'ENERGIA TOTALE IN FUNZIONE DI 1/BETA*OMEGA = 1/n*eta
import pylab
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import os


# Definizione della cartella principale
numero = 30
#scelta = [0.2, 0.4, 0.6, 0.8]
root_dir = f"/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/"
dati_dir = root_dir + "Splitting/Campo/"

# Definizione della cartella delle misure
mis_dir = root_dir + "Splitting/Campo/C(k)/"

# Definisco la lunghezza dell'array dei k
max_k = 20

# Scorro i file dei dati bootstrappati
for filename in sorted(os.listdir(dati_dir)):
    f = os.path.join(dati_dir, filename)

    if f.find('txt')!=-1:

        # Salvare valore di eta corrente
        inizio = f.find('(Eta=') + 5
        fine = inizio + 5
        eta=float(f[inizio:fine])
        print(f"Valore di eta: {eta}")

        # Salvare valore di Neta corrente
        Neta=''
        inizio1 = f.find('NEta=') + 6
        fine1 = inizio1 + 2
        string = f[inizio1:fine1]
        for element in string:
            if (element.isdigit() == True):         # controlla se l'elemento nella stringa è un numero
                Neta = Neta + element

        # Seleziono il campo

        if ((f.find('po(')!=-1) or (f.find('po2(')!=-1) ):

            x=np.loadtxt(f, unpack=False)

            righe = np.shape(x)[0]  #Numero dei dati (anche delle colonne della matrice)
            N = np.shape(x)[1]  #Numero di cammini


            media = np.mean(x, axis=0)  # Array della media a tempo fisso per cammini diversi (cambio colonna)
            meanCk = np.zeros(max_k)
            kappa = np.zeros(max_k)

            print(f"media: {len(media)}, shape x: {np.shape(x)}")
            
        # Calcolo del Ck
            for k in range(0,max_k):
                Ck = np.zeros(righe)

                for i in range(righe):
                    for j in range(N-k):
                        Ck[i] = (x[i][j])*(x[i][j+k]) + Ck[i]

                    Ck[i] = Ck[i]/(N-k)     #Divido per numero di cose sommmate

                kappa[k]=int(k)
                meanCk[k] = np.mean(Ck)

        # Calcolo della funzione connessa

                # Salvataggio dati in un file txt
                if (f.find('po(')!=-1):

                    nomefile = mis_dir + f"Ck_Y(eta={eta:.3f},N_Eta={numero},k={k}).txt"

                else:
                    nomefile = mis_dir + f"Ck_Y2(eta={eta:.3f},N_Eta={numero},k={k}).txt"

                np.savetxt(nomefile, Ck, delimiter="\t")
            
            if (f.find('po(')!=-1):

                nfile = mis_dir + f"<Y>(eta={eta:.3f},N_Eta={numero}).txt"

            else:
                nfile = mis_dir + f"<Y2>(eta={eta:.3f},N_Eta={numero}).txt"
            
            np.savetxt(nfile, media, delimiter="\t")

# PROGRAMMA PER FARE PLOT E FIT DELL'ENERGIA TOTALE IN FUNZIONE DI 1/BETA*OMEGA = 1/n*eta
import pylab
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import os
from uncertainties import ufloat

# Definizione della cartella principale
numero = 20
scelta = [0.571, 0.667]
root_dir = f"/Users/margherita/Desktop/UNIPI/QUINT_ANNO/OscillatoreArmonico/N_ETA_fisso/N_Eta={numero}/Bootstrap/"

# Definizione della cartella delle misure
mis_dir = "/Users/margherita/Desktop/UNIPI/QUINT_ANNO/OscillatoreArmonico/Splitting/"

# Definisco la lunghezza dell'array dei k e il numero di resampling da prendere
max_k = 20
res_num = 10

# Scorro i file dei dati bootstrappati
for filename in os.listdir(root_dir):
    f = os.path.join(root_dir, filename)

    # Salvare valore di eta corrente
    inizio = f.find('eta=') + 4
    fine = inizio +5
    eta=float(f[inizio:fine])

    # Seleziono il campo

    if (eta in scelta) & ((f.find('Y')!=-1) or (f.find('_X2')!=-1) ):

        x=np.loadtxt(f, max_rows=res_num, unpack=False)

        righe = np.shape(x)[0]  #Numero di resampling
        N = np.shape(x)[1] #Numero dei dati (anche delle colonne della matrice di resampling)


        media = np.mean(x, axis=1)  # Array della media di ogni resampling
        meanCk = np.zeros(max_k)
        stdCk = np.zeros(max_k)
        kappa = np.zeros(max_k)


        for k in range(0,max_k):
            print(k)
            Ck = np.zeros(righe)

            for i in range(righe):
                for j in range(N-k):
                    Ck[i] = (x[i][j]-media[i])*(x[i][j+k]-media[i]) + Ck[i]

                Ck[i] = Ck[i]/(N-k)     #Divido per numero di cose sommmate

            kappa[k]=int(k)
            meanCk[k] = np.mean(Ck)
            stdCk[k] = np.std(Ck)


        # Salvataggio dati in un file txt
        if (f.find('Y')!=-1):

            nomefile = mis_dir + f"Ck_Y(eta={eta},N_Eta={numero}).txt"

        else:
            nomefile = mis_dir + f"Ck_Y2(eta={eta},N_Eta={numero}).txt"

        full_array = np.stack([meanCk, stdCk, kappa], axis=1) # Unire gli array per salvarli


        np.savetxt(nomefile, full_array, delimiter="\t", header="media\std", comments='#')

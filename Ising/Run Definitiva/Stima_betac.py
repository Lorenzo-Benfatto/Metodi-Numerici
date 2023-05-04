# A program to plot the means of the rebinned bootstrap

import pylab
import numpy as np
from datetime import datetime
import matplotlib.pyplot as plt
import os
from scipy.interpolate import interp1d

startTime = datetime.now()

# Inizializzo l'array delle correlazioni


# Dati reticolo e resampling
M = 100  #numero di resampling

directory = fr"/home/dario/Documents/UNI/Metodi/Modulo1/Ising/Bootstrap/Definitivi"

#Creazione di una figura unica per mettere a confronto lo strain dei 3 interferometri
fig_m, ax_m = plt.subplots()


#Inizio ciclo per scorrere gli nlatt
for root, dirs, files in os.walk(directory):
    for i in range(len(dirs)):
    #if os.path.isdir(dirs) == True:
        index = dirs[i].find('Nlatt')
        if index !=-1:
            inizio = index + 6
            fine = index + 8
            Nlatt = (dirs[i])[inizio:fine]
            Nlatt=int(Nlatt)
            print(Nlatt)



            # Definizione volume reticolo
            vol= Nlatt**2
            
            

        
            #######################################     MAGNETIZZAZIONE        ######################################################
            '''Magnetizzazione'''

            # Inizializzo l'array della magnetizzazione, dei beta e del binder
            beta = []
            binder =[]
            binder_err = []

            # assign directory
            #directory = r"C:\home\dario\Documents\UNI\Metodi\Modulo1\Ising\Bootstrap\Nlatt=30\Bootstrappo\Magnetizzazione"
            directory = fr"/home/dario/Documents/UNI/Metodi/Modulo1/Ising/Bootstrap/Definitivi/Nlatt={Nlatt}/Bootstrappo/Magnetizzazione"  #mettere magnetizzzazione ed energia, che vengono fuori da ising_bootstrappato.c, che viene da simulatore_boot.c


            # iterate over files in that directory
            for filename in os.listdir(directory):
                f = os.path.join(directory, filename)
                # checking if it is a file
                if os.path.isfile(f):
                    if f.find('txt')!=-1:
                        # i resampling sono scritti per riga
                        x=np.loadtxt(f, unpack=False)

                        # salvataggio del valore di beta a partire dalla stringa
                        inizio = f.find('0.')
                        fine = inizio + 5
                        beta = np.append(beta, float(f[inizio:fine]))

            
                        '''media dell'energia/magnetizzazione al quadrato e errore associato (serve sia per calore specifico che per binder)'''
                        m_magne2=np.mean(x**2,axis=1)
                        
                        '''media dell'energia alla quarta e errore associato (serve per il binder)'''
                        m_magne4=np.mean(x**4,axis=1)
                        
                           #calcolo cumulante di binder
                        binder_res = m_magne4/(m_magne2**2)     # binder per ogni resampling a beta fisso
                        binder = np.append(binder, np.mean(binder_res))       # binder al variare di beta
                        binder_err_res = np.std(binder_res, ddof=1)       # deviazione standard per singolo resampling
                        binder_err = np.append(binder_err, binder_err_res)
                        
            # Conversione delle liste ottenute in array di numpy
            beta = np.array(beta)
            binder=np.array(binder)
            binder_err = np.array(binder_err)
            
            
            #Funzione di interpolazione per trovare l'intersezione dei binder per Nlatt diversi
            f = interp1d(beta, binder)
            
            
            # Grafico della magnetizzazione bootstrappata in funzione dei beta
            ax_m.errorbar(beta, binder, binder_err, linestyle = '', marker ='.',label=f'Nlatt = {Nlatt}')
            beta, binder = zip(*sorted(zip(beta, binder)))
            ax_m.plot(beta, f(beta))
            #ax_m.plot(beta, binder, linestyle='--')
print(beta)
ax_m.title.set_text('MODELLO DI ISING 2D \n Cumulante di Binder')
ax_m.set_xlabel(r'$\beta$')
ax_m.set_ylabel('Binder')
ax_m.grid()            
ax_m.minorticks_on()
ax_m.legend()
plt.show()
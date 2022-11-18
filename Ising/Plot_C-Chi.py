# A program to plot the means of the rebinned bootstrap

import pylab
import numpy as np
from datetime import datetime
import matplotlib.pyplot as plt
import os

from datetime import datetime

startTime = datetime.now()

# Inizializzo l'array delle correlazioni
beta = []

# ogni valore dei segueti array cambia con beta
err_ene = []   # errore energia
C_err = []  # errore calore specifico
binder_err=[]   # errore binder

C = []      # lista calore specifico
binder = []  # è il cumulante di binder che ci serve per stimare il beta critico
m=[]       # media energia

# Dati reticolo e resampling
M = 100  #numero di resampling
Nlatt = 100
vol= Nlatt**2

# assign directory
directory = fr"\\wsl.localhost\Ubuntu\home\dario\Documents\Metodi\Modulo1\Ising\Bootstrap\Nlatt={Nlatt}\Bootstrappo\Magnetizzazione"



# iterate over files in that directory
for filename in os.listdir(directory):
    f = os.path.join(directory, filename)
    # checking if it is a file
    if os.path.isfile(f):
        if f.find('txt')!=-1:
            # i resampling sono scritti per riga
            x=np.loadtxt(f, unpack=False)
            #faccio deviazione standard delle medie
            x = np.abs(x)


            # salvataggio del valore del numero di resampling a partire dalla stringa
            inizio = f.find('nned')
            if(inizio!=-1):
                inizio = inizio + 4
                fine = inizio + 5

            b=float(f[inizio:fine])
            beta = np.append(beta, b)

            m_ene=np.mean(x,axis=1) # array con la media delle energie per ogni ricampionamento a beta fisso


            '''media dell'energia e errore associato'''
            enemedia=np.mean(m_ene)     # media delle medie dei ricampionamenti a beta fisso
            m=np.append(m,enemedia)     # array delle medie cambiando beta
            #erre = np.sqrt(2)*np.sqrt(np.sum(m_ene**2)/M - (np.sum(m_ene)**2)/M**2)
            erre = np.sqrt(2)*np.std(m_ene, ddof=1)    # errore sulla media di tutti i ricampionamenti a beta fisso

            err_ene = np.append(err_ene, erre)      # errore sulle medie al variare dei beta

            '''media dell'energia al quadrato e errore associato (serve sia per calore specifico che per binder)'''
            m_ene2=np.mean(x**2,axis=1)

            '''media dell'energia alla quarta e errore associato (serve per il binder)'''
            m_ene4=np.mean(x**4,axis=1)

            #Calcolo calore specifico e incertezza
            C_res = vol * (m_ene2 - m_ene**2)       # calore specifico per ogni resampling a beta fisso
            C = np.append(C, np.mean(C_res))        # array del calore specifico al variare di beta
            C_err_res = np.std(C_res, ddof=1)       # deviazione standard per singolo resampling
            C_err = np.append(C_err, C_err_res)

##========== NOTA BENE: il binder va fatto sulla magnetizzazione e basta ===================
            #calcolo cumulante di binder
            binder_res = m_ene4/(m_ene2**2)     # binder per ogni resampling a beta fisso
            binder = np.append(binder, np.mean(binder_res))       # binder al variare di beta
            binder_err_res = np.std(binder_res, ddof=1)       # deviazione standard per singolo resampling
            binder_err = np.append(binder_err, binder_err_res)

            print(len(binder))

# Conversione delle liste ottenute in array di numpy
beta = np.array(beta)
C = np.array(C)
binder=np.array(binder)

C_err = np.array(C_err)
binder_err = np.array(binder_err)

'''
QUI VA FATTO GRAFICO PER I DIVERSI RETICOLI GUARDANDO QUANDO SI SI INTERSECANO LE CURVE PER IL BINDER AL VARIARE DI BETA (VISTO DAL GENERALE)
# Uso il binder per trovare il valore di beta più vicino al beta critico, perché so che il binder tende a 1 vicino al beta critico

beta_C = beta[np.argmin(binder-1)]
print(beta_C)
'''
# #salvo su file i dati che poi fitterò

# Saving the array in a text file
#string = f'calore(Nlatt={Nlatt}).txt'
filename = fr'\\wsl.localhost\Ubuntu\home\dario\Documents\Metodi\Modulo1\Ising\Bootstrap\Nlatt={Nlatt}\calore(Nlatt={Nlatt}).txt'#'+ string

with open(filename, 'w') as f:
    for a,b,c,d,e in zip(C, C_err, binder, binder_err, beta):
        print("%f  %f  %f  %f  %f" % (a, b, c, d, e), file = f)

f.close()


## Grafico del Calore specifico bootstrappato in funzione dei beta

plt.figure(1)
plt.title('MODELLO DI ISING 2D \n Calore specifico al variare del beta')
plt.xlabel(r'$\beta$')
plt.ylabel(r'Calore specifico')
plt.grid()
plt.errorbar(beta, C, C_err, marker ='.', linestyle = '')
#plt.legend()
plt.minorticks_on()

plt.show()

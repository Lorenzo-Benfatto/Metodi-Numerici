# A programto plot the means of the rebinned bootstrap

import pylab
import numpy as np
from datetime import datetime
import matplotlib as plt
import os

from datetime import datetime

startTime = datetime.now()


M=100 #Numero di resampling effettuati nel bootstrap
 

# Lettura del file dei beta (scritti per colonna)
beta=np.loadtxt("beta.txt", unpack=True) 

# Inizializzo l'array della magnetizzazione
mag = np.zeros(len(beta))

# assign directory
directory = '/Users/margherita/Desktop/UNIPI/QUINT_ANNO/Ising/Nlatt = 10/Bootstrap'
 
# iterate over files in that directory
for filename in os.listdir(directory):
    f = os.path.join(directory, filename)
    # checking if it is a file
    if os.path.isfile(f):
        if f.find('txt')!=-1:
            print(f)
            # i resampling sono scritti per riga
            x=np.loadtxt(f, unpack=False)
            # faccio la media tutta la matrice e la salvo nel file mag
            mag=np.append(mag, np.mean(np.abs(x)))
            # salvataggio del valore di beta a partire dalla stringa
            inizio = f.find('0.')
            fine = inizio + 4
            beta = np.append(beta, f[inizio:fine])
        
beta = np.asarray(beta)            
            
# Grafico della magnetizzazione bootstrappata in funzione dei beta

pylab.figure(1)
pylab.title('MODELLO DI ISING 2D \n Magnetizzazione intorno alla transizione')
pylab.xlabel(r'$\beta$')
pylab.ylabel('M')
pylab.errorbar(beta, mag, linestyle = '', color = 'red', marker ='.')
pylab.show()


'''
x=np.loadtxt('resample_binned.txt', unpack=False)  #Funziona male col binned bootstrap (cumulante sbagliato)

a=np.mean(x,axis=1)
b=np.mean(a)



y=np.linspace(1,M,M)

pylab.figure(1)
pylab.rc('font',size=10)
pylab.title('Fluttuazione dei punti')
pylab.xlabel('')
#pylab.ylabel('')
#pylab.hist(x, 50)
pylab.errorbar(y,a-b, linestyle = '', color = 'red', marker ='*')
pylab.hlines(0,1,M,linestyle='--',color='black')
pylab.minorticks_on()

pylab.figure(2)
pylab.rc('font',size=10)
pylab.title('Fluttuazione dei Binder')
pylab.xlabel('')
#pylab.ylabel('')
#pylab.hist(x, 50)
pylab.errorbar(y,binderino-binder, linestyle = '', color = 'blue', marker ='o')
pylab.hlines(0,1,M,linestyle='--',color='black')
pylab.minorticks_on()

print(datetime.now() - startTime)


pylab.show()
'''
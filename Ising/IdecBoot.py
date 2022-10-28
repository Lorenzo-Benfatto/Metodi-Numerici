# A programto plot the means of the rebinned bootstrap

import pylab
import numpy as np
from datetime import datetime
import matplotlib as plt
import os

from datetime import datetime

startTime = datetime.now()

M=100 #Numero di resampling effettuati nel bootstrap 

# Inizializzo l'array delle correlazioni
idecorrel = []
err = []

# assign directory
directory = '/Users/margherita/Desktop/UNIPI/QUINT_ANNO/Ising/Nlatt = 50/Bootstrap/DecorrBoot'
 
# iterate over files in that directory
for filename in os.listdir(directory):
    f = os.path.join(directory, filename)
    # checking if it is a file
    if os.path.isfile(f):
        if f.find('txt')!=-1:
            print(f)
            # i resampling sono scritti per riga
            x=np.loadtxt(f, unpack=False)
            #faccio deviazione standard delle medie 
            x = np.abs(x)
            a=np.mean(x,axis=1)
            b=np.mean(a)

            err = np.append(err, np.sqrt(np.sum(a**2)/M - (np.sum(a)/M)**2))
            # salvataggio del valore di beta a partire dalla stringa
            inizio = f.find('ec)_')
            if(inizio!=-1):
                inizio = inizio + 4
                fine = inizio + 2
            else:
                inizio = f.find('ec)') +3
                fine = inizio +3
                
            idecorrel = np.append(idecorrel, int(f[inizio:fine]))

       
idecorrel = np.array(idecorrel)
idecorrel = idecorrel *24
print(idecorrel)
err = np.array(err)
print(err)
 
# Grafico della magnetizzazione bootstrappata in funzione dei beta

pylab.figure(1)
pylab.title('MODELLO DI ISING 2D \n Errore bootstrap in funzione del binning')
pylab.xlabel('correlazione')
pylab.ylabel(r'$\sigma$')
pylab.errorbar(idecorrel, err, linestyle = '', color = 'red', marker ='.')
pylab.minorticks_on()
pylab.show()


# A programto plot the means of the rebinned bootstrap

import pylab
import numpy as np
from datetime import datetime
import matplotlib.pyplot as plt
import os

from datetime import datetime

startTime = datetime.now()

# Inizializzo l'array delle correlazioni
resamp = []
err = []

# assign directory
directory = r"\\wsl.localhost\Ubuntu\home\dario\Documents\Metodi\Modulo1\Ising\Bootstrap\Nlatt=20\Decorrelazione\ResampBoot"

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
            #x = np.abs(x)




            # salvataggio del valore del numero di resampling a partire dalla stringa
            inizio = f.find('res)')
            if(inizio!=-1):
                inizio = inizio + 4
                fine = inizio + 3
                if(('.' in f[inizio:fine]) == True):
                    fine = fine - 1
                    if(('.' in f[inizio:fine]) == True):
                        fine = fine - 1

            M=int(f[inizio:fine])
            a=np.mean(x,axis=1)
            b=np.mean(a)

            err = np.append(err, np.sqrt(np.sum(a**2)/M - (np.sum(a)/M)**2))
            resamp = np.append(resamp, M)


resamp = np.array(resamp)
print(resamp)
err = np.array(err)
print(err)

# Grafico della magnetizzazione bootstrappata in funzione dei beta

pylab.figure(1)
pylab.title('MODELLO DI ISING 2D \n Errore Magnetizzazione bootstrap in funzione del numero di resampling')
pylab.xlabel('Numero di resampling')
pylab.ylabel(r'$\sigma$')
pylab.errorbar(resamp, err, linestyle = '', marker ='.')
#pylab.legend()
pylab.minorticks_on()

pylab.show()


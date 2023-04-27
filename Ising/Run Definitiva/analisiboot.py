# A programto plot the means of the rebinned bootstrap

import pylab
import numpy as np
from datetime import datetime
import matplotlib.pyplot as plt
import os

from datetime import datetime

startTime = datetime.now()


#M=100 #Numero di resampling effettuati nel bootstrap



# Inizializzo l'array della magnetizzazione
mag = []
beta = []

# assign directory
#directory = r"C:\home\dario\Documents\UNI\Metodi\Modulo1\Ising\Bootstrap\Nlatt=30\Bootstrappo\Magnetizzazione"
directory = r"/home/dario/Documents/UNI/Metodi/Modulo1/Ising/Bootstrap/Nlatt=30/Bootstrappo/Energia"  #mettere magnetizzzazione ed energia, che vengono fuori da ising_bootstrappato.c, che viene da simulatore_boot.c


# iterate over files in that directory
for filename in os.listdir(directory):
    f = os.path.join(directory, filename)
    # checking if it is a file
    if os.path.isfile(f):
        if f.find('txt')!=-1:
            # i resampling sono scritti per riga
            x=np.loadtxt(f, unpack=False)
            # faccio la media tutta la matrice e la salvo nel file mag
            mag=np.append(mag, np.mean(np.abs(x)))
            # salvataggio del valore di beta a partire dalla stringa
            inizio = f.find('0.')
            fine = inizio + 5
            beta = np.append(beta, float(f[inizio:fine]))


beta = np.array(beta)
print(beta)
mag = np.array(mag)
print(mag)

# Grafico della magnetizzazione bootstrappata in funzione dei beta

plt.figure(1)
plt.title('MODELLO DI ISING 2D \n Magnetizzazione intorno alla transizione')
plt.xlabel(r'$\beta$')
plt.ylabel('M')
plt.errorbar(beta, mag, linestyle = '', color = 'red', marker ='.')
plt.minorticks_on()
plt.show()


'''
x=np.loadtxt('resample_binned.txt', unpack=False)  #Funziona male col binned bootstrap (cumulante sbagliato)

a=np.mean(x,axis=1)
b=np.mean(a)



y=np.linspace(1,M,M)

plt.figure(1)
plt.rc('font',size=10)
plt.title('Fluttuazione dei punti')
plt.xlabel('')
#plt.ylabel('')
#plt.hist(x, 50)
plt.errorbar(y,a-b, linestyle = '', color = 'red', marker ='*')
plt.hlines(0,1,M,linestyle='--',color='black')
plt.minorticks_on()

plt.figure(2)
plt.rc('font',size=10)
plt.title('Fluttuazione dei Binder')
plt.xlabel('')
#plt.ylabel('')
#plt.hist(x, 50)
plt.errorbar(y,binderino-binder, linestyle = '', color = 'blue', marker ='o')
plt.hlines(0,1,M,linestyle='--',color='black')
plt.minorticks_on()

print(datetime.now() - startTime)


plt.show()
'''

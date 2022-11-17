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
err_ene = []
err_ene2 = []
C_err = []
C = []
er=[]
er2=[]
# assign directory
directory = r"\\wsl.localhost\Ubuntu\home\dario\Documents\Metodi\Modulo1\Ising\Bootstrap\Nlatt=100\Bootstrappo\Energia"

M = 100  #numero di resampling
vol= 100**2
m=[]
m2=[]
# iterate over files in that directory
for filename in os.listdir(directory):
    f = os.path.join(directory, filename)
    # checking if it is a file
    if os.path.isfile(f):
        if f.find('txt')!=-1:
            # i resampling sono scritti per riga
            x=np.loadtxt(f, unpack=False)
            print(np.shape(x))
            #faccio deviazione standard delle medie
            x = x


            # salvataggio del valore del numero di resampling a partire dalla stringa
            inizio = f.find('nned')
            if(inizio!=-1):
                inizio = inizio + 4
                fine = inizio + 5

            b=float(f[inizio:fine])
            beta = np.append(beta, b)

            m_ene=np.mean(x,axis=1)

            enemedia=np.mean(m_ene)
            m=np.append(m,enemedia)
            #erre = np.sqrt(2)*np.sqrt(np.sum(m_ene**2)/M - (np.sum(m_ene)**2)/M**2)
            erre = np.sqrt(2)*np.std(m_ene, ddof=1)

            err_ene = np.append(err_ene, erre)

            m_ene2=np.mean(x**2,axis=1)
            ene2media=np.mean(m_ene2)
            m2=np.append(m2,ene2media)
            erre2 = np.sqrt(np.sum(m_ene2**2)/M - np.sum(m_ene2/M)**2)
            err_ene2=np.append(err_ene2, erre2)

            C = np.append(C, (ene2media - enemedia**2)*vol)
           # C_err = np.append (C_err, np.sqrt(erre**2+erre2**2)/vol)
            print(len(C_err))

beta = np.array(beta)
C = np.array(C)
print(np.shape(m))
print(np.shape(m2))

con = np.stack((m, m2), axis=1)

C_err =np.sqrt(err_ene**2+err_ene2**2+2*(np.cov(con)[1][0])/M)

C_err = np.array(C_err)

print(C_err)

## Grafico del Calore specifico bootstrappato in funzione dei beta

plt.figure(1)
plt.title('MODELLO DI ISING 2D \n Calore specifico al variare del beta')
plt.xlabel(r'$\beta$')
plt.ylabel(r'Calore specifico')
plt.grid()
plt.errorbar(beta, C, C_err, marker ='o', linestyle = '')
#plt.legend()
plt.minorticks_on()

plt.show()

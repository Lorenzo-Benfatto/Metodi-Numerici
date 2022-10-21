# A programto plot the means of the rebinned bootstrap

import pylab
import numpy as np
from datetime import datetime
import matplotlib as plt


M=100 #Numero di resampling effettuati nel bootstrap

x=np.loadtxt('resample_binned.txt', unpack=False) 

a=np.mean(x,axis=1)
b=np.mean(a)

err = np.sqrt(np.sum(a**2)/M - (np.sum(a)/M)**2)

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



print(f"Media: {b} +- {err}, idecorrel = 100")
print(f"Errore percentuale {(err/b)*100}%")

pylab.show()
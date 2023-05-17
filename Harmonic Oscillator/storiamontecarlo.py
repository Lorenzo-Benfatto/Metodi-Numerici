#Un programma per vedere come fluttuano le medie e i cumulanti di Binder sui vari batch resemplati di un rebinned bootstrap. Mangia un matricione con M righe (colonne?) ognuna delle quali ha per colonna (riga) i dati ripescati.

import pylab
import numpy as np
from datetime import datetime
import matplotlib as plt

from datetime import datetime

M=100 #numero di resampling (non ricordo più se è il numero di righe o di colonne)
Nlatt=10


field2,Dfield2,field,passo=np.loadtxt(fr'C:\Users\aministratore\Documents\Università\Magistrale\Metodi Numerici\Modulo-3\Nuova_run\Oscillo/misure.txt', unpack=True) #la matrice in questione

y=np.linspace(1,M,M)

pylab.figure(1)
pylab.rc('font',size=10)
pylab.title('Storia Montecarlo Campi')
pylab.xlabel('Passi')
pylab.ylabel('Valore Montecarlo')
pylab.errorbar(passo,field, linestyle = '-', color = 'red', marker ='.', markersize=1)
pylab.minorticks_on()
'''
pylab.figure(2)
pylab.rc('font',size=10)
pylab.title('Storia Montecarlo Fluttuazioni')
pylab.xlabel('Passi')
pylab.ylabel('Valore Montecarlo')
pylab.errorbar(passo,Dfield2, linestyle = '-', color = 'red', marker ='.', markersize=1)
pylab.minorticks_on()
'''


pylab.show()
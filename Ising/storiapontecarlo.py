#Un programma per vedere come fluttuano le medie e i cumulanti di Binder sui vari batch resemplati di un rebinned bootstrap. Mangia un matricione con M righe (colonne?) ognuna delle quali ha per colonna (riga) i dati ripescati.

import pylab
import numpy as np
from datetime import datetime
import matplotlib as plt

from datetime import datetime

M=100 #numero di resampling (non ricordo più se è il numero di righe o di colonne)

mag,ene,passo=np.loadtxt('misure.txt', unpack=True) #la matrice in questione

y=np.linspace(1,M,M)

pylab.figure(1)
pylab.rc('font',size=10)
pylab.title('Storia Montecarlo Magnetizzazione')
pylab.xlabel('Passi')
pylab.ylabel('Valore Montecarlo')
pylab.errorbar(passo,abs(mag), linestyle = '-', color = 'red', marker ='.', markersize=1)
pylab.minorticks_on()

pylab.figure(2)
pylab.rc('font',size=10)
pylab.title('Storia Montecarlo Energia')
pylab.xlabel('Passi')
pylab.ylabel('Valore Montecarlo')
pylab.errorbar(passo,ene, linestyle = '-', color = 'red', marker ='.', markersize=1)
pylab.minorticks_on()

print("Tempo di esecuzione: ", datetime.now() - startTime)


pylab.show()
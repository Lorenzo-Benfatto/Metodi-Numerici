# A program that shows the gaussian distribution simulated via the metrogauss.c program.
# It also prints out the acceptance.

import pylab
import numpy as np
from datetime import datetime

x=np.loadtxt(fr'C:\Users\aministratore\Documents\Università\Magistrale\Metodi Numerici\Modulo-3\Nuova_run\Oscillazione.txt', unpack=True)

acc=np.sum(x)*100/len(x)

pylab.figure(1)
pylab.rc('font',size=10)
pylab.title('Accettanza')
pylab.xlabel('')
pylab.ylabel('')
pylab.hist(x, density=True)
pylab.minorticks_on()
'''
pylab.figure(2)
pylab.rc('font',size=10)
pylab.title('Storia Montecarlo')
pylab.xlabel('Passi')
pylab.ylabel('Valore Montecarlo')
pylab.errorbar(y,x, linestyle = '-', color = 'red', marker ='.', markersize=1)
pylab.minorticks_on()

pylab.figure(3)
pylab.rc('font',size=10)
pylab.title('Accettanza')
pylab.xlabel('')
pylab.ylabel('')
pylab.hist(z, 50)
pylab.minorticks_on()
'''
print("Accettanza= ",np.sum(x)*100/len(x))

pylab.show()
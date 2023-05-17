# A program that shows the gaussian distribution simulated via the metrogauss.c program.
# It also prints out the acceptance.

import pylab
import numpy as np
from datetime import datetime

y2,Dy2,field,iter=np.loadtxt(fr'C:\Users\aministratore\Documents\Università\Magistrale\Metodi Numerici\Modulo-3\Nuova_run\Oscillo\misure.txt', unpack=True)


pylab.figure(1)
pylab.rc('font',size=10)
pylab.title('Istogramma del campo (cammini)')
pylab.xlabel('')
pylab.ylabel('')
pylab.hist(field, 40, density=True)
pylab.minorticks_on()


pylab.show()
# A program that plots the data simulated according to the Box-Muller algorithm

import pylab
import numpy as np
from datetime import datetime

startTime = datetime.now()

x,y=np.loadtxt('gaussian.txt', unpack=True)

pylab.figure(1)
pylab.rc('font',size=10)
pylab.title('Distribuzione uniforme in [0,1]')
pylab.xlabel('')
pylab.ylabel('')
pylab.hist(x, 50)
pylab.minorticks_on()
print(datetime.now() - startTime)
pylab.show()

pylab.figure(2)
pylab.rc('font',size=10)
pylab.title('Gaussiana con Box-Muller')
pylab.xlabel('')
pylab.ylabel('')
pylab.hist(y, 50)
pylab.minorticks_on()
print(datetime.now() - startTime)
pylab.show()

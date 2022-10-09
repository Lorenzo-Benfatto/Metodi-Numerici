# A programto plot the means of the rebinned bootstrap

import pylab
import numpy as np
from datetime import datetime
import matplotlib as plt

from datetime import datetime

startTime = datetime.now()

import winsound
frequency = 3500  # Set Frequency To 2500 Hertz
duration = 2000  # Set Duration To 1000 ms == 1 second

M=50

x=np.loadtxt('resample.txt', unpack=False)

a=np.mean(x,axis=1)
b=np.mean(a)

x4=np.mean(x**4,axis=1)
x2=np.mean(x**2,axis=1)
binderino=x4/(x2**2)
binder=np.mean(binderino)
print("Cumulante di Binder mediato sugli ensemble: ", binder)

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

pylab.figure(2)
pylab.rc('font',size=10)
pylab.title('Fluttuazione dei Binder')
pylab.xlabel('')
#pylab.ylabel('')
#pylab.hist(x, 50)
pylab.errorbar(y,binderino-binder, linestyle = '', color = 'blue', marker ='o')
pylab.hlines(0,1,M,linestyle='--',color='black')
pylab.minorticks_on()

print(datetime.now() - startTime)
#winsound.Beep(frequency, duration)

pylab.show()

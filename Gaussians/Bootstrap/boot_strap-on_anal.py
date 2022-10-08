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

M=1000

x=np.loadtxt('resample_binned.txt', unpack=False)

a=np.mean(x,axis=1)
b=np.mean(a)
y=np.linspace(1,M,M)


pylab.figure(1)
pylab.rc('font',size=10)
pylab.title('scatter plot')
pylab.xlabel('')
#pylab.ylabel('')
#pylab.hist(x, 50)
pylab.errorbar(y,a-b, linestyle = '', color = 'red', marker ='*')
pylab.hlines(0,1,M,linestyle='--',color='magenta')
pylab.minorticks_on()
winsound.Beep(frequency, duration)

print(datetime.now() - startTime)

pylab.show()

# A program to plot the means of the rebinned bootstrap

import pylab
import numpy as np
from datetime import datetime
import matplotlib as plt

from datetime import datetime

startTime = datetime.now()

#import winsound
#frequency = 3500  # Set Frequency To 2500 Hertz
#duration = 2000  # Set Duration To 1000 ms == 1 second

M=100

x=pylab.loadtxt(fr"C:\Users\aministratore\Documents\Università\Magistrale\Metodi Numerici\Modulo-3\Nuova_run\Bootstrap\resample_binned_X2.txt", unpack=False)

a=np.mean(x,axis=1)
b=np.mean(a)
print('mean=', b)
devstd=np.std(a,ddof=1)
print('standard deviation=', devstd)

# x4=np.mean(x**4,axis=1)
# x2=np.mean(x**2,axis=1)
# binderino=x4/(x2**2)
# binder=np.mean(binderino)
# bindererr=np.sqrt(np.sum(binderino**2)/M-(np.sum(binderino)/M)**2)
# print("Cumulante di Binder mediato sugli ensemble: ", binder)
# print("errore:", bindererr)

y=np.linspace(1,M,M)

c=a-b*np.ones(len(a))

pylab.figure(1)
pylab.rc('font',size=10)
pylab.title('Fluttuazione dei punti')
pylab.xlabel('')
#pylab.ylabel('')
#pylab.hist(x, 50)
pylab.errorbar(c,y, linestyle = '', color = 'red', marker ='*')
pylab.hlines(0,1,M,linestyle='--',color='black')
pylab.minorticks_on()


pylab.show()
# A program that shows the gaussian distribution simulated via the metrogauss.c program.
# It also prints out the acceptance.

import pylab
import numpy as np
from datetime import datetime

#y2,Dy2,field,iter=np.loadtxt(fr'C:\Users\aministratore\Documents\Università\Magistrale\Metodi Numerici\Modulo-3\Nuova_run\Oscillo\misure.txt', unpack=True)

field, iter=np.loadtxt(fr'C:\Users\aministratore\Documents\Università\Magistrale\Metodi Numerici\Modulo-3\Nuova_run\FunzioneDondaFondamentale\Valoriperisto.txt', unpack=True)

N=111
pylab.figure(1)
pylab.rc('font',size=10)
pylab.title('Istogramma del campo (cammini)')
pylab.xlabel('')
pylab.ylabel('')
pylab.hist(field, 150,color='Blue', density=True)
pylab.minorticks_on()

x=np.linspace(-3,3,10000)
'''
A=1.7
B=0
C=0.05
'''

A=1/(np.pi**(1/4))
B=0
C=1


ground=A*np.exp(-((x+B)**2)/(2*C))
pylab.errorbar(x,abs(ground)**2, marker ='', color='black', linestyle = '-')

pylab.show()
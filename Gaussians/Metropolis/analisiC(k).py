# A program that calculates the exponential constant for the C(k) via a best fit.
# It then computes a better estimate for standard deviation of the gaussian distribution
# simulated with the Metropolis algorithm.

import pylab
import numpy as np
from scipy.optimize import curve_fit

#Importo i dati
x,y=np.loadtxt('analis.txt', unpack=True)

#Fit
def f(x,tau):
    return np.exp(-x/tau)

#Valori iniziali
init = 700

popt, pcov=curve_fit(f, x, y, init, absolute_sigma=False)#absolute_sigma va cambiato in base ai casi

ndof=len(x)-1
print('popt:', popt)
print('dpopt:', np.sqrt(pcov.diagonal()))


#Plot dei dati nel grafico
pylab.figure(1)

pylab.rc('font',size=10)

pylab.title('Funzione di autocorrelazione')

pylab.xlabel('k')

pylab.ylabel('C(k)')

# plot curva best fit (con dummy array)
xx=np.linspace(min(x),max(x),2000)
pylab.plot(xx,f(xx,*popt), color='deepskyblue')

#plot dei dati
pylab.errorbar(x,y, linestyle = '', color = 'red', marker ='.', markersize=1)

#Calcolo esatto di tau int
print("Valore preciso di tau int", np.sum(x))

#Deviazione standard del campione
err=np.sqrt(np.sum((y-np.mean(y))**2)/(len(y)*(len(y)-1)))
print(err)
err = err*np.sqrt((1+2*popt[0]))
print(err)



pylab.minorticks_on()





pylab.show()

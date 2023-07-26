import pylab
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import os

## assegno cartella principale
root_dir =fr"C:\Users\aministratore\Documents\Università\Magistrale\Metodi Numerici\Modulo-3\Nuova_run\\"

# Lettura del file valori.txt per capire se sta facendo il limite al continuo o varia la T a eta fisso
filename = root_dir + "valori_eta.txt"
primovalore = np.loadtxt(filename, unpack = True)

nomefile =fr"C:\Users\aministratore\Documents\Università\Magistrale\Metodi Numerici\Modulo-3\Nuova_run\N_ETA_fisso\N_Eta=15\Bootstrap\bootstrap.txt"

y2,err_y2,dy2,err_dy2,y,err_y,ene,err_ene,eta,Nlatt=np.loadtxt(nomefile, unpack=True)



## Fit del termine potenziale

# Funzione di fit
def f(x,a,b):
    return a*x**1/2+b

# Valori iniziali
init = (0.65,0.5)

# Ciclo per minimizzare il ci quadro
popt, pcov=curve_fit(f, eta, y2, init, err_y2)


ndof=len(eta)-1
chi2=(((y2-f(eta, *popt))/err_y2)**2).sum()
print('Passo zero')
print('popt:', popt)
print('dpopt:', np.sqrt(pcov.diagonal()))
print('chi2=%f, \nndof=%f' %(chi2, ndof))
dxy=err_y2
dx = np.zeros(len(eta))
for i in range(0, 3, 1):
    dxy=np.sqrt(dxy**2+dx**2)
    popt, pcov=curve_fit(f, eta, y2, popt, dxy)
    chi2=(((y2-f(eta, *popt))/dxy)**2).sum()
    print('Passo %d' % i)
    print('popt:', popt)
    print('dpopt:', np.sqrt(pcov.diagonal()))
    print('chi2=%f, \nndof=%f' %(chi2, ndof))


print('\n\n\n')


## Grafico dell'energia potenziale bootstrappata in funzione del valore di eta

# Dummy array per disegnare la funzione di plot
xx = np.linspace(min(eta), max(eta), 1000)

'''
# Plot del termine cinetico rinormalizzato
partenza=2
y2=y2[partenza:]
eta=eta[partenza:]
#print(f'Nlatt {Nlatt}')
Nlatt=Nlatt[partenza:]
err_y2=err_y2[partenza:]
'''

dy2=-dy2/(2*eta**2)
dy2_norm=dy2+1/(2*eta)
err_dy2=err_dy2/(2*eta**2)

#y2=-np.divide(y2, 2*eta**2)
#y2_norm = np.add(1/(2*eta), y2)
#print(Nlatt)


# Figura
plt.figure(1)
plt.title('OSCILLATORE ARMONICO \n Termine cinetico')
plt.xlabel(r'$\eta$')
plt.ylabel(r'$\frac{<\Delta y^2>}{2}$')
plt.grid()

#plt.plot( xx, f(xx, *popt), color='red')
plt.errorbar(eta, dy2, err_dy2, marker ='.', linestyle = '')
plt.minorticks_on()

plt.figure(2)
plt.title('OSCILLATORE ARMONICO \n Termine cinetico normalizzato')
plt.xlabel(r'$\eta$')
plt.ylabel(r'$\frac{- <\Delta y^2>}{2 \eta^2} \eta^{N/2}$')
plt.grid()

plt.errorbar(eta, dy2_norm, err_dy2, marker ='.', linestyle = '')
plt.minorticks_on()
plt.show()
import pylab
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import os


## assegno cartella principale
nomefile =fr"C:\Users\aministratore\Documents\Università\Magistrale\Metodi Numerici\Modulo-3\Nuova_run\N_ETA_fisso\N_Eta=15\Bootstrap\bootstrap.txt"

root_dir=fr"C:\Users\aministratore\Documents\Università\Magistrale\Metodi Numerici\Modulo-3\Nuova_run\N_ETA_fisso\\"

y2,err_y2,dy2,err_dy2,y,err_y,ene,err_ene,eta,Nlatt=np.loadtxt(nomefile, unpack=True)


## Fit del termine potenziale

# Funzione di fit
def f(x,a,b):
    return a*x**2+b

# Valori iniziali
init = (-0.65,0.5)

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


# Figura
plt.subplot(211)
plt.title(r'OSCILLATORE ARMONICO \n Termine potenziale')
plt.xlabel(r'$\eta$')
plt.ylabel(r'$\frac{<y^2>}{2}$')
plt.grid()

plt.plot( xx, f(xx, *popt), color='red')
plt.errorbar(eta, y2, err_y2, marker ='.', linestyle = '')
plt.minorticks_on()

# Residui normalizzati
plt.subplot(212)
r = (y2-f(eta,*popt))/dxy
plt.errorbar( eta,r, linestyle='', marker='.')
plt.title('Residui normalizzati')
plt.xlabel(r'$\eta$')
plt.ylabel('(dati - modello)/errore')
plt.grid()

plt.show()

# Salvataggio dei dati in un file (risultato del fit, N*eta)
filename = root_dir + 'ene_continuo.txt'
print(filename)
with open(filename, 'a') as g:
    g.write(f'{(Nlatt[0]*eta[0])}   {popt[0]}   {popt[1]}   {(np.sqrt(pcov.diagonal()))[0]} {(np.sqrt(pcov.diagonal()))[1]} {chi2}  {ndof} \n')

# PROGRAMMA PER FARE PLOT E FIT DELL'ENERGIA TOTALE IN FUNZIONE DI 1/BETA*OMEGA = 1/n*eta
import pylab
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import os

label = 'Continuo'     # Scelta per limite al continuo ('Continuo') o cambio di temperatura ('Temperatura')

# Cartelle
root_dir = "/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/"
dati_dir = root_dir + label + "/Bootstrap/"

# Array che mi servono
Nt = []
massa = []
epsilon = []
depsilon = []

# Scorro file della densità di energia nella cartella del bootstrap
for filename in sorted(os.listdir(dati_dir)):
        f = os.path.join(dati_dir, filename)
        if ((f.find('txt')!=-1) and (f.find('density')!=-1)):

            # Salvataggio del valore di Nt
            n_t = ''
            inizio1 = f.find('Nt=') + 3
            fine1 = inizio1 + 3
            string1 = f[inizio1:fine1]
            for element in string1:
                if(element.isdigit()==True):
                    n_t= n_t + element

            n_t = float(n_t)
            Nt = np.append(Nt, n_t)

            # Salvataggio del valore di m
            m = ''
            inizio = f.find(',m=') + 3
            fine = inizio + 5
            string = f[inizio:fine]
            for element in string:
                if(element.isdigit()==True or ('.' in element) == True):
                    m = m + element

            m = float(m)
            massa = np.append(massa, m)

            # Carico i dati
            x=np.loadtxt(f, unpack=False)

            print(np.shape(x))

            media = np.mean(x, axis = 1) # Media di un resampling (riga)

            epsilon = np.append(epsilon, np.mean(media))

            depsilon = np.append(depsilon, np.std(media))

## Fit con una parabola

# outlier
idx_max = np.argmax(epsilon)
outlier = 0, idx_max, idx_max-1, idx_max-2
#outlier = idx_max, idx_max-1, idx_max-2, idx_max-4, idx_max-6, idx_max-8, 0, 3
x = np.delete(massa,outlier)
y = np.delete(epsilon,outlier)
ennet = np.delete(Nt,outlier)
y = (y*ennet**2)/2
dy = (np.delete(depsilon,outlier)*ennet**2)/2

# Funzione di fit
def funz(ics,a,b):
    return a*ics**2+b

# Valori iniziali
init = (1,np.pi/6)

# Ciclo per minimizzare il chi quadro


popt, pcov=curve_fit(funz, x, y, init, dy)


ndof=len(x)-len(init)
chi2=(((y-funz(x, *popt))/dy)**2).sum()
print('Passo zero')
print('popt:', popt)
print('dpopt:', np.sqrt(pcov.diagonal()))
print('chi2=%f, \nndof=%f' %(chi2, ndof))
dxy=dy

dx = np.zeros(len(x))
for i in range(0, 3, 1):
    dxy=np.sqrt(dxy**2+dx**2)
    popt, pcov=curve_fit(funz, x, y, popt, dxy)
    chi2=(((y-funz(x, *popt))/dxy)**2).sum()
    print('Passo %d' % i)
    print('popt:', popt)
    err = np.sqrt(pcov.diagonal())
    print('dpopt:', err)
    print('chi2=%f, \nndof=%f' %(chi2, ndof))


print('\n\n\n')

# Plot dell'energy density in funzione della massa

# Titolo ed etichette per gli assi
title = 'Energy density in funzione della massa'
labelx = 'm'
labely = r'$\epsilon$'

#    Dummy array per plot con la funzione di fit
xx = np.linspace(min(x), max(x), 1000)

plt.figure()
plt.title(title)
plt.xlabel(labelx)
plt.ylabel(labely)


plt.errorbar(x, y, dy, marker ='.', linestyle = '')
plt.plot(xx, funz(xx, *popt), color='red')
plt.minorticks_on()
plt.show()
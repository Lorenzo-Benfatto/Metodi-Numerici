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

# # Array che mi servono
# Nt = []
# massa = []
# epsilon = []
# depsilon = []

# # Scorro file della densità di energia nella cartella del bootstrap
# #commentare e scommentare tutte le cose "_NO_RELAX" a seconda di ciò che si vuole vedere
# for filename in sorted(os.listdir(dati_dir)):
#         f = os.path.join(dati_dir, filename)
#         if ((f.find('txt')!=-1) and (f.find('density')!=-1) ):# and (f.find('_NO_RELAX')!=-1)):

#             # Salvataggio del valore di Nt
#             n_t = ''
#             inizio1 = f.find('Nt=') + 3
#             fine1 = inizio1 + 3
#             string1 = f[inizio1:fine1]
#             for element in string1:
#                 if(element.isdigit()==True):
#                     n_t= n_t + element

#             n_t = float(n_t)
#             Nt = np.append(Nt, n_t)

#             # Salvataggio del valore di m
#             m = ''
#             inizio = f.find(',m=') + 3
#             fine = inizio + 6
#             string = f[inizio:fine]
#             for element in string:
#                 if(element.isdigit()==True or ('.' in element) == True):
#                     m = m + element

#             m = float(m)
#             massa = np.append(massa, m)

#             # Carico i dati
#             x=np.loadtxt(f, unpack=False)

#             print(np.shape(x))

#             media = np.mean(x, axis = 1) # Media di un resampling (riga)

#             epsilon = np.append(epsilon, np.mean(media))

#             depsilon = np.append(depsilon, np.std(media))
            
            
# #stampo su file tutti i valori che mi interessa plottare
# zippo = zip(Nt, epsilon, depsilon, massa)
# nfile = root_dir + label+ f"/limcontinuo.txt"#_NO_RELAX.txt"
# zippo=list(zippo)
# #print(zippo)
            
# np.savetxt(nfile, zippo, delimiter="\t", header='Nt/ene_density/dene_density/massa')


#############################################################################################################
##############################      ANALISI DATI           ##################################################
#############################################################################################################


nfile = root_dir + label + f"/limcontinuo.txt"
Nt, epsilon, depsilon, massa = np.loadtxt(nfile, unpack=True)

#il "_noOR" sta per simulazione senza Over-Relaxation
nfile_noOR = root_dir + label + f"/limcontinuo_NO_RELAX.txt"  #il "_noOR" sta per simulazione senza Over-Relaxation
Nt_noOR, epsilon_noOR, depsilon_noOR, massa_noOR = np.loadtxt(nfile, unpack=True)
## Fit con una parabola

#calcolo densità di energia su temperatura al quadrato
epsilon = 0.5*epsilon*Nt**2
depsilon = 0.5*depsilon*Nt**2
epsilon_noOR = 0.5*epsilon_noOR*Nt_noOR**2
depsilon_noOR = 0.5*depsilon_noOR*Nt_noOR**2

# outlier
y=epsilon
x=massa
dy=depsilon
taglio=3  #numero di dati che voglio tagliare per m grosso
for i in range (taglio): 
    outlier = np.argmax(y)
    x = np.delete(x,outlier)
    y = np.delete(y,outlier)
    dy = np.delete(dy,outlier)

ordine=np.argsort(massa)
altri_outlier=ordine[0],ordine[1] #anche i dati per m piccola non mi piacciono
x = np.delete(x,altri_outlier)
y = np.delete(y,altri_outlier)
dy = np.delete(dy,altri_outlier)


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
xx = np.linspace(0, max(x)+0.018, 1000)

plt.figure(1)
plt.title(title)
plt.xlabel(labelx)
plt.ylabel(labely)


plt.errorbar(x, y, dy, marker ='.', linestyle = '', label='dati', color='blue')
massa=np.sort(massa)
epsilon_2=np.take(epsilon,ordine)
depsilon_2=np.take(depsilon,ordine)
plt.errorbar(massa[len(massa)-taglio:], epsilon_2[len(massa)-taglio:], depsilon_2[len(massa)-taglio:], marker ='.', linestyle = '', label='outliers', color='black')
plt.plot(xx, funz(xx, *popt), color='red',label='best-fit')
plt.minorticks_on()
plt.title('Densità di energia in funzione della massa adimensionale')
plt.legend()

plt.figure(2)
plt.errorbar(massa_noOR,epsilon_noOR, depsilon_noOR, marker ='*', linestyle = '', label='Senza Over-Relaxation', color = 'red')
plt.title('Densità di energia in funzione della massa adimensionale, senza Over-Relaxation')
plt.minorticks_on()
plt.show()
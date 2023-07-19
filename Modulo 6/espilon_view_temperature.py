# PROGRAMMA PER FARE PLOT E FIT DELL'ENERGIA TOTALE IN FUNZIONE DI 1/BETA*OMEGA = 1/n*eta
import pylab
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import os

label = 'Temperatura'

# Cartelle
root_dir = "/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/"
dati_dir = root_dir + label + "/Bootstrap/"


# Array che mi servono
Nt = []
massa = []
epsilon = []
depsilon = []
O1 = []
dO1 = []
O2 = []
dO2 = []
O3 = []
dO3 = []
ene = []
dene = []

# Scorro file della densità di energia nella cartella del bootstrap. I file sono letti in ordine di nome
# quindi in ordine di Ntm, che è il primo valore che trovo nel nome. Ma questo porrta ad avere anch ei file ordinati in ordine crescente di Nt

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

            # Carico i dati della densità di energia
            x=np.loadtxt(f, unpack=False)


            media = np.mean(x, axis = 1) # Media di un resampling (riga)

            epsilon = np.append(epsilon, np.mean(media))

            depsilon = np.append(depsilon, np.std(media))

            print(f'DENTRO AL FOR: e = {np.mean(media)}, Nt = {n_t}')
'''
        if ((f.find('txt')!=-1) and (f.find('mass')!=-1)):

            x=np.loadtxt(f, unpack=False)

            media = np.mean(np.abs(x), axis = 1) # Media di un resampling (riga)

            O1 = np.append(O1, np.mean(media))

            dO1 = np.append(dO1, np.std(media))

        if ((f.find('txt')!=-1) and (f.find('spat')!=-1)):

            x=np.loadtxt(f, unpack=False)

            media = np.mean(np.abs(x), axis = 1) # Media di un resampling (riga)

            O2 = np.append(O2, np.mean(media))

            dO2 = np.append(dO2, np.std(media))

        if ((f.find('txt')!=-1) and (f.find('temp')!=-1)):

            x=np.loadtxt(f, unpack=False)

            media = np.mean(np.abs(x), axis = 1) # Media di un resampling (riga)

            O3 = np.append(O3, np.mean(media))

            dO3 = np.append(dO3, np.std(media))
'''
# Sottraggo il valore di epsilon corrispondente a Nt più alto (T più bassa), perché non sappiamo il valore zero dell'energia. Quindi l'ultimo
'''
idx = np.argmax(Nt)
print(idx, Nt[idx])

print(f'PRIMA: {epsilon}')
#epsilon.sort()
epsilon[::-1].sort()

print(f'DOPO: {epsilon}')
'''


#depsilon = np.sqrt((depsilon)**2+ (depsilon[-1])**2)
'''
# Calcolo dell'energia con la somma delle osservabili singole

ene = (O1 + O2 - O3)*1/2
ene = (ene - ene[idx])*Nt**2
dene = np.sqrt(dO1**2+dO2**2+dO3**2)*Nt**2
'''
# Cambio variabile per comodità
x1 = Nt
y1 = epsilon
dy1 = depsilon
#y1 = ene[:stop]
#dy1 = dene[:stop]

## Plot dell'energy density in funzione di Nt

# Titolo ed etichette per gli assi
title = 'Energy density in funzione di Nt'
labelx = r'$Nt$'
labely = r'$\epsilon$'


plt.figure()
plt.title(title)
plt.xlabel(labelx)
plt.ylabel(labely)

plt.errorbar(Nt, epsilon, dy1, marker ='.', linestyle = '', color = 'black')

plt.minorticks_on()

# Normalizzazione dell'energy density
epsilon[-2]=epsilon[-2]+0.00003
epsilon[-4]=epsilon[-4]+0.00003
epsilon = (epsilon - epsilon[np.argmax(Nt)])

# Cambio variabile per comodità
stop = len(epsilon)
x2 = (1/(Nt[:stop]*massa[0]))
y2 = 0.5*(epsilon[:stop])*Nt**2
dy2 = 0.5*(depsilon[:stop])*Nt**2

zippo=zip(x1, y1, dy1, x2, y2, dy2)
zippo=list(zippo)
nfile= dati_dir = root_dir + label + f"/energy_density.txt"
np.savetxt(nfile, zippo, delimiter='\t',  header='#Nt / density / delta(sx) / (1/Ntm) / density*Nt**2 / Delta(sx)')

######################################################################################
##########################  RUNNA QUESTO SE VUOI I GRAFICI ###########################
#######################################################################################

nfile= dati_dir = root_dir + label + f"/energy_density.txt"
x1, y1, dy1, x2, y2, dy2 = np.loadtxt(nfile,unpack=True)

## Plot dell'energy density in funzione di 1/Nt*m e moltiplicata per Nt**2

# Titolo ed etichette per gli assi
title = r'Energy density in funzione di $\frac{1}{Nt*m}$'
labelx = r'$\frac{1}{Nt*m}$'
labely = r'$\frac{\epsilon}{T^2}$'


plt.figure()
plt.title(title)
plt.xlabel(labelx)
plt.ylabel(labely)


plt.errorbar(x2, y2, dy2, marker ='.', linestyle = '')
#plt.errorbar(x, y1, marker ='.', linestyle = '', color = 'green')
xx = np.linspace(min(x2), max(x2), 1000)
plt.plot(xx, np.pi/6*np.ones(len(xx)), linestyle = '-')
plt.minorticks_on()
plt.show()
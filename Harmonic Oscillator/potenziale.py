import pylab
import numpy as np
import matplotlib.pyplot as plt
import os
frmo scipy.optimize import curve_fit

## Inizializzazione array
eta = []
y2 = []
err_y2 = []


## assegno cartella principale
root_dir = "/Users/margherita/Desktop/UNIPI/QUINT_ANNO/OscillatoreArmonico/"

# Lettura del file valori.txt per capire se sta facendo il limite al continuo o varia la T a eta fisso
filename = root_dir + "valori.txt"
primovalore = np.loadtxt(filename, unpack = True)

#se primovalore < 9 varia T, altrimenti limite al continuo. Quindi assegno il nome della cartella delle misure di conseguenza

if(primovalore[0]<9):
    dir = 'N_variabile'     #Scelta tra Eta o N_variabile
else:
    dir = 'Eta'

# Adesso cerco la cartella del bootstrap
directory = root_dir + f"{dir}/Bootstrap"



## iterate over files in that directory
for filename in os.listdir(directory):
    f = os.path.join(directory, filename)
        if f.find('DX2')!=-1:
            continue
        else:
            # i resampling sono scritti per riga. Leggo il valore di tutti i resampling del campo (è una matrice)
            x=np.loadtxt(f, unpack=False)

            # salvataggio del valore di eta a partire dalla stringa
            inizio = f.find('eta=') + 4
            fine = inizio +5

            n=float(f[inizio:fine])
            eta = np.append(eta, n)

            # Analisi della variabile bootstrappata
            m_y2=np.mean(x,axis=1) # array con la media delle energie per ogni ricampionamento a eta fisso
            y2_media=np.mean(m_y2)     # media delle medie dei ricampionamenti a eta fisso (ovvero media di tutta la matrice)
            y2=np.append(y2,y2_media)     # array delle medie cambiando eta

            erre = np.sqrt(2)*np.std(m_y2, ddof=1)    # errore sulla media di tutti i ricampionamenti a eta fisso

            err_y2 = np.append(err_y2, erre**2)      # errore sulle medie al variare degli eta

## Conversione dlele liste ottenute in array di numpy
eta = np.array(eta)
y2 = np.array(y2)
err_y2 = np.array(err_y2)


## Fit del termine potenziale

# Funzione di fit
def f(x,a,b):
    return a*x**2+b

# Valori iniziali
init = (-0.45,0.5)

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
    popt, pcov=curve_fit(f, eta, y2, popt, err_y2)
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

plt.show()
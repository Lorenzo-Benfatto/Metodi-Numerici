import pylab
import numpy as np
import matplotlib.pyplot as plt
import os
from scipy.optimize import curve_fit
from scipy.interpolate import interp1d
import string
import matplotlib as mpl

# Cartella principale
root_dir = "/Users/margherita/Desktop/UNIPI/QUINT_ANNO/OscillatoreArmonico/Splitting/"

# Valore di N*eta
Neta = 30

# Dichiarazione delle liste da utilizzare
eta = []
E_Y = []
E_Y2 = []
dE_Y = []
dE_Y2 = [] 

#  Dichiarazione delle figure

fig1, ax1 = plt.subplots()
fig2, ax2 = plt.subplots()


# Indice per scorere l'array degli eta per y^2
idx = 0

# iterate over files in directory
for filename in sorted(os.listdir(root_dir)):
    f = os.path.join(root_dir, filename)
    # checking if it is a file
    if (os.path.isfile(f) and (f.find(').txt')!=-1)):
        
        if (f.find('CK')!=-1):
            
            e=''

            inizio1 = f.find('(eta=') + 5
            fine1 = inizio1 + 4
            
            string1 = f[inizio1:fine1]
            for element in string1:
                if(element.isdigit()==True or ('.' in element) == True):
                    e=e + element
             
            e=float(e)
            eta = np.append(eta, e)

        # Salvataggio dei dati dal file
        
        k, ck, dck, ck2, dck2 = np.loadtxt(f, unpack=True)

        k=k*eta[idx]
        
        # Dummy array per disegnare la funzione di plot
        xx = np.linspace(min(k), max(k), 1000)
        
        # Funzione di fit
        def funz(x,a,b):
            return a*np.exp(-x*b)
        
        ## FIT DEI CK
        # Valori iniziali per il Ck del campo
        init = (1,1/1.5)
        
        # Ciclo per minimizzare il chi quadro
        
        print(f"Eta: {eta[idx]}")
        
        popt, pcov=curve_fit(funz, k, ck, init, dck)
        
        
        ndof=len(k)-len(init)
        chi2=(((ck-funz(k, *popt))/dck)**2).sum()
        print('Passo zero')
        print('popt:', popt)
        print('dpopt:', np.sqrt(pcov.diagonal()))
        print('chi2=%f, \nndof=%f' %(chi2, ndof))
        dxy=dck

        dx = np.zeros(len(k))
        for i in range(0, 3, 1):
            dxy=np.sqrt(dxy**2+dx**2)
            popt, pcov=curve_fit(funz, k, ck, popt, dxy)
            chi2=(((ck-funz(k, *popt))/dxy)**2).sum()
            print('Passo %d' % i)
            print('popt:', popt)
            err = np.sqrt(pcov.diagonal())
            print('dpopt:', err)
            print('chi2=%f, \nndof=%f' %(chi2, ndof))
        
        
        print('\n\n\n')
        
        
        
     
      ## FIGURA PER IL CAMPO

        # salvataggio dei parametri di best fit
        E_Y = np.append(E_Y, popt[1])
        dE_Y = np.append(dE_Y, err[1])

        # Plot dei C(k) per il campo
        
        ax1.set_title(fr'OSCILLATORE ARMONICO \n C(k) per <y>, per $\eta N$ = {Neta}')
        ax1.set_xlabel('k')
        ax1.set_ylabel(r'$C(k)$')
        ax1.errorbar(k, ck, dck, linestyle = '', marker ='.', label = fr'$\eta$ = {e}')
        ax1.plot(xx, funz(xx, *popt))
        ax1.grid()

        ax1.legend()
        ax1.minorticks_on()
        
    
        ## FIT DEL CK PER IL CAMPO AL QUADRATO
        # Valori iniziali per il Ck del campo
        init = (0.5,1/5)
        
        # Ciclo per minimizzare il chi2
        popt, pcov=curve_fit(funz, k, ck2, init, dck)
        
        
        ndof=len(k)-len(init)
        chi2=(((ck2-funz(k, *popt))/dck2)**2).sum()
        print('Passo zero')
        print('popt:', popt)
        print('dpopt:', np.sqrt(pcov.diagonal()))
        print('chi2=%f, \nndof=%f' %(chi2, ndof))
        dxy=dck2

        dx = np.zeros(len(k))
        for i in range(0, 3, 1):
            dxy=np.sqrt(dxy**2+dx**2)
            popt, pcov=curve_fit(funz, k, ck2, popt, dxy)
            chi2=(((ck-funz(k, *popt))/dxy)**2).sum()
            print('Passo %d' % i)
            print('popt:', popt)
            err = np.sqrt(pcov.diagonal())
            print('dpopt:', err)
            print('chi2=%f, \nndof=%f' %(chi2, ndof))
        
        
        print('\n\n\n')
        
        # salvataggio dei parametri di best fit
        E_Y2 = np.append(E_Y2, popt[1])
        dE_Y2 = np.append(dE_Y2, err[1])


        
        ax2.set_title(fr'OSCILLATORE ARMONICO \n C(k) per <y^2>, per $\eta N$ = {Neta}')
        ax2.set_xlabel('k')
        ax2.set_ylabel(r'$C(k)$')
        ax2.errorbar(k, ck2, dck2, linestyle = '', marker ='.', label = fr'$\eta$ = {eta[idx]}')
        ax2.plot(xx, funz(xx, *popt))
        ax2.grid()

        ax2.legend()
        ax2.minorticks_on()
            
        idx = idx + 1

## FIT LINEARE DEI PARAMETRI DI BEST FIT

# Funzione di fit
def gunz(x,m,q):
    return m+q*x

x = eta**2

# Dummy array per disegnare la funzione di plot
xx = np.linspace(min(x), max(x), 1000)

for i in range(2):

    if (i==0):
        y = E_Y
        dy = dE_Y
        label = r'$\frac{E_1 - E_0}{\hbar \omega}$'
        title = 'Splitting tra fondamentale e primo eccitato'
    else:
        y = E_Y2
        dy = dE_Y2
        label = r'$\frac{E_2 - E_0}{\hbar \omega}$'
        title = 'Splitting tra fondamentale e secondo eccitato'
        
    
    
    # Valori iniziali
    init = (2,-0.1)
    
    # Ciclo per minimizzare il chi quadro
    popt, pcov=curve_fit(gunz, x, y, init, dy)
    
    
    ndof=len(x)-len(init)
    chi2=(((y-gunz(x, *popt))/dy)**2).sum()
    print('Passo zero')
    print('popt:', popt)
    print('dpopt:', np.sqrt(pcov.diagonal()))
    print('chi2=%f, \nndof=%f' %(chi2, ndof))
    dxy=dy
    
    dx = np.zeros(len(x))
    for i in range(0, 3, 1):
        dxy=np.sqrt(dxy**2+dx**2)
        popt, pcov=curve_fit(gunz, x, y, popt, dxy)
        chi2=(((y-gunz(x, *popt))/dxy)**2).sum()
        print('Passo %d' % i)
        print('popt:', popt)
        err = np.sqrt(pcov.diagonal())
        print('dpopt:', err)
        print('chi2=%f, \nndof=%f' %(chi2, ndof))
    
    
    print('\n\n\n')
    
    
    # Figura
    plt.figure()
    plt.title(title)
    plt.xlabel(r'$\eta^2$')
    plt.ylabel(label)
    
    plt.plot(xx, gunz(xx, *popt), color='red')
    plt.errorbar(x, y, dy, marker ='.', linestyle = '')
    plt.minorticks_on()
        
    
    
    

            
plt.show()
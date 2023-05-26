
import pylab
import numpy as np
import matplotlib.pyplot as plt
import os
from scipy.optimize import curve_fit
from scipy.interpolate import interp1d
import string
import matplotlib as mpl

#Scelta dei colori da usare nel grafico
mpl.rcParams['axes.prop_cycle'] = mpl.cycler(color=["#0000FF", "#069AF3","#008000",'#9ACD32','#DA70D6',"#FF00FF", "#FF0000", "#FFA500"])

# Cartella principale
root_dir = "/Users/margherita/Desktop/UNIPI/QUINT_ANNO/OscillatoreArmonico/Splitting"

# Dichiarazione delle liste da utilizzare
eta = []
Nlatt = []
tau = []

#  Dichiarazione delle figure

fig1, ax1 = plt.subplots()
fig2, ax2 = plt.subplots()



# iterate over files in directory
for filename in os.listdir(root_dir):
    f = os.path.join(root_dir, filename)
    # checking if it is a file
    if os.path.isfile(f):
        if f.find('txt')!=-1:
            
            inizio = f.find('N=') + 2
            fine = inizio + 4
            
            # Ciclo per salvare i valori di N anche se hanno numero di cifre diverse
            N = ''
            string = f[inizio:fine]
            for element in string:
                if (element.isdigit() == True):         # controlla se l'elemento nella stringa è un numero
                    N = N + element
    
            N=float(N)
            Nlatt = np.append(Nlatt, N)
                
            inizio = f.find('eta=') + 4
            fine = inizio + 5
            
            # Ciclo per salvare i valori di N anche se hanno numero di cifre diverse
            e = ''
            string = f[inizio:fine]
            for element in string:
                if (element.isdigit() == True or ('.' in element) == True):         # controlla se l'elemento nella stringa è un numero
                    e = e + element

            e=float(e)
            eta = np.append(eta, e)
            
            if((e<0.4) & (e>0.02)):

                # Salvataggio dei dati dal file
                
                k, ck_y, ck_y2 = np.loadtxt(f, unpack=True)
            
                ## FIGURA PER IL CAMPO
                
                # Interpolazione e intersezione con la retta 1/e
                f = interp1d(k, ck_y)
                x = np.linspace(np.min(k), np.max(k), 10000)
                idx = np.argmin(np.abs(f(x)-np.exp(-1)))
                tau=np.append(tau, (x[idx]))
                print(tau)
                
                
                # Plot dei C(k) per il campo
                
                ax1.set_title(rf'MODELLO DI ISING 2D \n C(k) per <y>, per $\eta N$ = 20')
                ax1.set_xlabel('k')
                ax1.set_ylabel(r'$C(k)$')
                ax1.scatter(k, ck_y, linestyle = '--', marker ='.', label = fr'$\eta$ = {e}')
                y= np.ones(len(x))*np.exp(-1)
                ax1.plot(x,y)
    
                ax1.legend()
                ax1.minorticks_on()
                
         
                
    
        
                ## FIGURA PER IL CAMPO AL QUADRATO
                
        
                ax2.set_title(rf'MODELLO DI ISING 2D \n C(k) fper <y^2>, per $\eta N$ = 20')
                ax2.set_xlabel('k')
                ax2.set_ylabel(r'$C(k)$')
    

    
                # Interpolazione e intersezione con la retta 1/e
                f = interp1d(k, ck_y2)
                x = np.linspace(np.min(k), np.max(k), 10000)
                idx = np.argmin(np.abs(f(x)-np.exp(-1)))
                tau=np.append(tau, (x[idx]))
                print(tau)
                
                
                # Plot dei C(k) per il campo al quadrato
                ax2.scatter(k, ck_y2, linestyle = '--', marker ='.', label = fr'$\eta$ = {e}')
                y= np.ones(len(x))*np.exp(-1)
                ax2.plot(x,y)
            
                ax2.legend()
                ax2.minorticks_on()
                
             
    
   
            
plt.show()

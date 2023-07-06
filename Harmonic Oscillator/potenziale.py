import pylab
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import os


## assegno cartella principale
root_dir = "/home/dario/Documents/UNI/Metodi/Modulo2/Oscillatore/"
direc = 'N_ETA_fisso/'

#Cancella file in cui scrivere i risultati, altrimenti appende
file_path = root_dir + direc + '/ene_continuo.txt'
if os.path.isfile(file_path):
    os.remove(file_path)
    
for it in os.scandir(direc):
    if it.is_dir():
        subdir = root_dir + it.path + "/Bootstrap" 
        print(subdir)
        
        ## Inizializzazione array
        eta = []
        y2 = []
        err_y2 = []
        Nlatt = []

        ## iterate over files in that directory
        for filename in os.listdir(subdir):
            f = os.path.join(subdir, filename)
            if f.find('_X2')==-1:
                continue
            else:
                # i resampling sono scritti per riga. Leggo il valore di tutti i resampling del campo (è una matrice)
                print(f)
                x=np.loadtxt(f, unpack=False)
                

                # salvataggio del valore di eta a partire dalla stringa
                inizio = f.find('eta=') + 4
                fine = inizio +5
                
                n=float(f[inizio:fine])
                eta = np.append(eta, n)
                
                # Salvataggio del valore di N
                # Ciclo per salvare i valori di N anche se hanno numero di cifre diverse
                N = ''
                string = f[fine+3:fine+6]
                for element in string:
                    if (element.isdigit() == True):         # controlla se l'elemento nella stringa è un numero
                        N = N + element
        
                N=float(N)
                Nlatt = np.append(Nlatt, N)

                # Analisi della variabile bootstrappata
                m_y2=np.mean(x,axis=1) # array con la media delle energie per ogni ricampionamento a eta fisso
                y2_media=np.mean(m_y2)     # media delle medie dei ricampionamenti a eta fisso (ovvero media di tutta la matrice)
                y2=np.append(y2,y2_media)     # array delle medie cambiando eta

                erre = np.std(m_y2, ddof=1)    # errore sulla media di tutti i ricampionamenti a eta fisso

                err_y2 = np.append(err_y2, erre)      # errore sulle medie al variare degli eta

        ## Conversione delle liste ottenute in array di numpy
        eta = np.array(eta)
        y2 = np.array(y2)
        err_y2 = np.array(err_y2)
        Nlatt = np.array(Nlatt)

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
        
        # Salvataggio dei dati in un file (risultato del fit, N*eta)
        nomefile = root_dir + direc + '/ene_continuo.txt'
        print(nomefile)
        with open(nomefile, 'a') as g:
            g.write(f'{(Nlatt[0]*eta[0])}   {popt[0]}   {popt[1]}   {(np.sqrt(pcov.diagonal()))[0]} {(np.sqrt(pcov.diagonal()))[1]} {chi2}  {ndof} \n')


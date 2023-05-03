# A program to plot the means of the rebinned bootstrap

import pylab
import numpy as np
from datetime import datetime
import matplotlib.pyplot as plt
import os

from datetime import datetime

startTime = datetime.now()

# Inizializzo l'array delle correlazioni


# Dati reticolo e resampling
M = 100  #numero di resampling

directory = fr"/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Run Definitiva"

#Creazione di una figura unica per mettere a confronto lo strain dei 3 interferometri
fig_c, ax_c = plt.subplots(figsize=(10,7))
fig_m, ax_m = plt.subplots()
fig_s, ax_s = plt.subplots()

#Inizio ciclo per scorrere gli nlatt
for root, dirs, files in os.walk(directory):
    for i in range(len(dirs)):
    #if os.path.isdir(dirs) == True:
        index = dirs[i].find('Nlatt')
        if index !=-1:
            inizio = index + 6
            fine = index + 8
            Nlatt = (dirs[i])[inizio:fine]
            Nlatt=int(Nlatt)
            print(Nlatt)



            # Definizione volume reticolo
            vol= Nlatt**2
            #inizializzo il beta
            beta = []
            
            ##############################################    CALORE SPECIFICO     ############################################


            # ogni valore dei segueti array cambia con beta
            err_ene = []   # errore energia
            C_err = []  # errore calore specifico
            binder_err=[]   # errore binder

            C = []      # lista calore specifico
            m=[]       # media energia


            # assign directory
            directory = fr"/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Run Definitiva/Nlatt={Nlatt}/Bootstrappo/Energia"


            # iterate over files in that directory
            for filename in os.listdir(directory):
                f = os.path.join(directory, filename)
                # checking if it is a file
                if os.path.isfile(f):
                    if f.find('txt')!=-1:
                        # i resampling sono scritti per riga
                        x=np.loadtxt(f, unpack=False)
                        #faccio deviazione standard delle medie
                        x = np.abs(x)

                       
                        # salvataggio del valore del numero di resampling a partire dalla stringa
                        inizio = f.find('nned')
                        if(inizio!=-1):
                            inizio = inizio + 4
                            fine = inizio + 5
                            b=float(f[inizio:fine])
                        beta = np.append(beta, b)

                        m_ene=np.mean(x,axis=1) # array con la media delle energie per ogni ricampionamento a beta fisso


                        '''media dell'energia e errore associato'''
                        enemedia=np.mean(m_ene)     # media delle medie dei ricampionamenti a beta fisso
                        m=np.append(m,enemedia)     # array delle medie cambiando beta
                        #erre = np.sqrt(2)*np.sqrt(np.sum(m_ene**2)/M - (np.sum(m_ene)**2)/M**2)
                        erre = np.sqrt(2)*np.std(m_ene, ddof=1)    # errore sulla media di tutti i ricampionamenti a beta fisso

                        err_ene = np.append(err_ene, erre)      # errore sulle medie al variare dei beta

                        '''media dell'energia al quadrato e errore associato (serve sia per calore specifico che per binder)'''
                        m_ene2=np.mean(x**2,axis=1)

                        '''media dell'energia alla quarta e errore associato (serve per il binder)'''
                        m_ene4=np.mean(x**4,axis=1)

                        #Calcolo calore specifico e incertezza
                        C_res = vol * (m_ene2 - m_ene**2)       # calore specifico per ogni resampling a beta fisso
                        C = np.append(C, np.mean(C_res))        # array del calore specifico al variare di beta
                        C_err_res = np.std(C_res, ddof=1)       # deviazione standard per singolo resampling
                        C_err = np.append(C_err, C_err_res)


            # Conversione delle liste ottenute in array di numpy
            beta = np.array(beta)
            C = np.array(C)
           # binder=np.array(binder)

            C_err = np.array(C_err)
           # binder_err = np.array(binder_err)

            '''
            QUI VA FATTO GRAFICO PER I DIVERSI RETICOLI GUARDANDO QUANDO SI SI INTERSECANO LE CURVE PER IL BINDER AL VARIARE DI BETA (VISTO DAL GENERALE)
            # Uso il binder per trovare il valore di beta più vicino al beta critico, perché so che il binder tende a 1 vicino al beta critico
            beta_C = beta[np.argmin(binder-1)]
            print(beta_C)
            '''
            # #salvo su file i dati che poi fitterò

            # Saving the array in a text file
            #string = f'calore(Nlatt={Nlatt}).txt'
            filename1 = fr"/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Run Definitiva/Nlatt={Nlatt}/Bootstrappo/calo(Nlatt={Nlatt}).txt+ string"

            with open(filename1, 'w') as f:
                for a,b,c in zip(C, C_err, beta):
                    print("%f  %f  %f" % (a, b, c), file = f)

            f.close()


            ## Grafico del Calore specifico bootstrappato in funzione dei beta


            ax_c.errorbar(beta, C, C_err*2, marker ='.', linestyle = '', label=f'Nlatt = {Nlatt}')


            ##############################################   SUSCETTIVITà    ######################################################
            '''SUSCETTIVITà'''

            # ogni valore dei segueti array cambia con beta
            err_magne = []   # errore magnetizzazione
            Susc_err = []  # errore suscettività
            binder_err=[]   # errore binder

            Susc = []      # lista suscettività
            binder = []  # è il cumulante di binder che ci serve per stimare il beta critico
            magne=[]       # media magnetizzazione


            # assign directory
            directory = fr"/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Run Definitiva/Nlatt={Nlatt}/Bootstrappo/Magnetizzazione"



            # iterate over files in that directory
            for filename in os.listdir(directory):
                f = os.path.join(directory, filename)
                # checking if it is a file
                if os.path.isfile(f):
                    if f.find('txt')!=-1:
                        # i resampling sono scritti per riga
                        x=np.loadtxt(f, unpack=False)
                        #faccio deviazione standard delle medie
                        x = np.abs(x)

                        m_magne=np.mean(x,axis=1) # array con la media delle energie per ogni ricampionamento a beta fisso


                        '''media della magnetizzazione e errore associato'''
                        magnemedia=np.mean(m_magne)     # media delle medie dei ricampionamenti a beta fisso
                        magne=np.append(magne,magnemedia)     # array delle medie cambiando beta
                        #erre = np.sqrt(2)*np.sqrt(np.sum(m_magne**2)/M - (np.sum(m_magne)**2)/M**2)
                        erre = np.sqrt(2)*np.std(m_magne, ddof=1)    # errore sulla media di tutti i ricampionamenti a beta fisso

                        err_magne = np.append(err_magne, erre)      # errore sulle medie al variare dei beta

                        '''media della magnetizzazione al quadrato e errore associato (serve sia per calore specifico che per binder)'''
                        m_magne2=np.mean(x**2,axis=1)

                        '''media della magnetizzazione alla quarta e errore associato (serve per il binder)'''
                        m_magne4=np.mean(x**4,axis=1)

                        #Suscettività e incertezza
                        Susc_res = vol * (m_magne2 - m_magne**2)       # suscettività per ogni resampling a beta fisso
                        Susc = np.append(Susc, np.mean(Susc_res))        # array della suscettività al variare di beta
                        Susc_err_res = np.std(Susc_res, ddof=1)       # deviazione standard per singolo resampling
                        Susc_err = np.append(Susc_err, Susc_err_res)

            ##========== NOTA BENE: il binder va fatto sulla magnetizzazione e basta ===================
                        #calcolo cumulante di binder
                        binder_res = m_magne4/(m_magne2**2)     # binder per ogni resampling a beta fisso
                        binder = np.append(binder, np.mean(binder_res))       # binder al variare di beta
                        binder_err_res = np.std(binder_res, ddof=1)       # deviazione standard per singolo resampling
                        binder_err = np.append(binder_err, binder_err_res)

                        print(len(binder))

            # Conversione delle liste ottenute in array di numpy
            beta = np.array(beta)
            Susc = np.array(Susc)
           # binder=np.array(binder)

            Susc_err = np.array(Susc_err)
           # binder_err = np.array(binder_err)

            '''
            QUI VA FATTO GRAFICO PER I DIVERSI RETICOLI GUARDANDO QUANDO SI SI INTERSECANO LE CURVE PER IL BINDER AL VARIARE DI BETA (VISTO DAL GENERALE)
            # Uso il binder per trovare il valore di beta più vicino al beta critico, perché so che il binder tende a 1 vicino al beta critico
            beta_Susc = beta[np.argmin(binder-1)]
            print(beta_Susc)
            '''
            # #salvo su file i dati che poi fitterò

            # Saving the array in a text file
            filename2 = fr"/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Run Definitiva/Nlatt={Nlatt}/Bootstrappo/sushi(Nlatt={Nlatt}).txt+ string"

            with open(filename2, 'w') as f:
                for a,b,c,d,e in zip(Susc, Susc_err, binder, binder_err, beta):
                    print("%f  %f  %f  %f  %f" % (a, b, c, d, e), file = f)

            f.close()


            ## Grafico del Suscettività bootstrappata in funzione dei beta



            ax_s.errorbar(beta, Susc, Susc_err*2, marker ='.', linestyle = '',label=f'Nlatt = {Nlatt}')
        
            #######################################     MAGNETIZZAZIONE        ######################################################
            '''MAgnetizzazione'''

            # Inizializzo l'array della magnetizzazione
            mag = []
            beta = []
            err_mag = []
            m=[]

            # assign directory
            #directory = r"C:\home\dario\Documents\UNI\Metodi\Modulo1\Ising\Bootstrap\Nlatt=30\Bootstrappo\Magnetizzazione"
            directory = fr"/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Run Definitiva/Nlatt={Nlatt}/Bootstrappo/Magnetizzazione"  #mettere magnetizzzazione ed energia, che vengono fuori da ising_bootstrappato.c, che viene da simulatore_boot.c


            # iterate over files in that directory
            for filename in os.listdir(directory):
                f = os.path.join(directory, filename)
                # checking if it is a file
                if os.path.isfile(f):
                    if f.find('txt')!=-1:
                        # i resampling sono scritti per riga
                        x=np.loadtxt(f, unpack=False)
                        # faccio la media tutta la matrice e la salvo nel file mag
                        mag=np.append(mag, np.mean(np.abs(x)))
                        # salvataggio del valore di beta a partire dalla stringa
                        inizio = f.find('0.')
                        fine = inizio + 5
                        beta = np.append(beta, float(f[inizio:fine]))

                        '''ora mi salvo i valori di magne medi con errore per fare poi il fit'''
                        m_mag=np.mean(x,axis=1) # array con la media delle energie per ogni ricampionamento a beta fisso


                        '''media dell'energia/magnetizzazione e errore associato'''
                        magmedia=np.mean(m_mag)     # media delle medie dei ricampionamenti a beta fisso
                        m=np.append(m,magmedia)     # array delle medie cambiando beta
                        #erre = np.sqrt(2)*np.sqrt(np.sum(m_ene**2)/M - (np.sum(m_ene)**2)/M**2)
                        erre = np.sqrt(2)*np.std(m_mag, ddof=1)    # errore sulla media di tutti i ricampionamenti a beta fisso

                        err_mag = np.append(err_mag, erre)      # errore sulle medie al variare dei beta

                        '''media dell'energia/magnetizzazione al quadrato e errore associato (serve sia per calore specifico che per binder)'''
                        m_mag2=np.mean(x**2,axis=1)

            # Saving the array in a text file
            #string = f'calore(Nlatt={Nlatt}).txt'
            filename = fr"/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Run Definitiva/Nlatt={Nlatt}/Bootstrappo/magne(Nlatt={Nlatt}).txt+ string"

            with open(filename, 'w') as f:
                for a,b,c in zip(mag, err_mag, beta):
                    print("%f  %f  %f" % (a, b, c), file = f)

            f.close()


            beta = np.array(beta)
            print(beta)
            mag = np.array(mag)
            print(mag)

            # Grafico della magnetizzazione bootstrappata in funzione dei beta
            ax_m.errorbar(beta, mag, linestyle = '', marker ='.',label=f'Nlatt = {Nlatt}')
            
            
ax_c.title.set_text('MODELLO DI ISING 2D \n Calore specifico al variare del beta')
ax_c.set_xlabel(r'$\beta$')
ax_c.set_ylabel(r'Calore specifico')
ax_c.grid()
ax_c.minorticks_on()
ax_c.legend()

ax_s.title.set_text('MODELLO DI ISING 2D \n Suscettività al variare del beta')
ax_s.set_xlabel(r'$\beta$')
ax_s.set_ylabel(r'Suscettività')
ax_s.grid()
ax_s.minorticks_on()
ax_s.legend()

ax_m.title.set_text('MODELLO DI ISING 2D \n Magnetizzazione intorno alla transizione')
ax_m.set_xlabel(r'$\beta$')
ax_m.set_ylabel('M')
ax_m.grid()            
ax_m.minorticks_on()
ax_m.legend()
plt.show()

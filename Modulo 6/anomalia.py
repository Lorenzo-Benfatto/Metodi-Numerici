# PROGRAMMA PER FARE PLOT E FIT DELL'ENERGIA TOTALE IN FUNZIONE DI 1/BETA*OMEGA = 1/n*eta
import pylab
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import os
from scipy import integrate

label = 'Temperatura'

# Cartelle
root_dir = "/home/dario/Documents/UNI/Metodi/Modulo3/Scalar2D/"
dati_dir = root_dir + label + "/Bootstrap/"


# Array che mi servono
Nt = []
massa = []
ene_mass = []
dene_mass = []

# Scorro file della densità di energia nella cartella del bootstrap. I file sono letti in ordine di nome
# quindi in ordine di Ntm, che è il primo valore che trovo nel nome. Ma questo porrta ad avere anch ei file ordinati in ordine crescente di Nt

for filename in sorted(os.listdir(dati_dir)):
        f = os.path.join(dati_dir, filename)
        if ((f.find('txt')!=-1) and (f.find('mass')!=-1)):
            print(f)

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


            media = np.mean(x, axis = 1) # Media di un resampling (riga)

            ene_mass = np.append(ene_mass, np.mean(media))

            dene_mass = np.append(dene_mass, np.std(media))

            #print(f'Nt = {n_t}, media = {np.mean(media)}')

# Sottraggo il valore di ene_mass corrispondente a Nt più alto (T più bassa), perché
# non sappiamo il valore zero dell'energia. Quindi l'ultimo

ene_mass = (ene_mass-ene_mass[-1])
#ene_mass = (ene_mass)

dene_mass = np.sqrt((dene_mass)**2+ (dene_mass[-1])**2)

# Cambio variabile per comodità

x = 1/(Nt*(massa[0]))
y = np.multiply(ene_mass,Nt**2)
z = np.multiply(ene_mass,(Nt**3)*massa[0])

dy = dene_mass*Nt**2
dz = dy*Nt*massa


#stampo su file tutti i valori che mi interessa plottare
zippo = zip(z, dz, y, dy, x)
nfile = root_dir + label+ f"/anomalia.txt"
zippo=list(zippo)
#print(zippo)
            
np.savetxt(nfile, zippo, delimiter="\t", header='pressione/dpressione/funz_integ/dfunz_integ/ascissa')



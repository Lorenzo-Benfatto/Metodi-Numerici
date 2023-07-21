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

######################################################################################
##########################  LETTURA DEI DATI DA FILE ###########################
#######################################################################################

n_file = root_dir + label+ f"/anomalia.txt"
z, dz, y, dy, x = np.loadtxt(n_file, unpack=True)

nfile = root_dir + label + f"/energy_density.txt"
x1, y1, dy1, tsum, edenssut2, dedenssut2 = np.loadtxt(nfile,unpack=True)

######################################################################################
##########################  CALCOLO DELL'INTEGRALE ###########################
#######################################################################################

# Devo invertire l'ordine di x per fare l'integrale, perché attualmente è ordinata in modo decrescente.
# Ma quando integro (cioé la plotto), è in ordine crescente
ascissa = np.sort(x)
print(f'ascissa: {ascissa} \n\n')

idx = np.argsort(x)

sorted_z = np.take(z, idx)      # funzione per ordinare array secondo la lista di indici data

print(f'z originale : {z} \n z sorted : {sorted_z}\n\n')

# Creazione di due array shiftati per la x per fare integrazione senza for
ascissa0 = ascissa[:-1]  # taglio l'ultimo valore dell'array
ascissa1= ascissa[1:]   # taglio il primo valore dell'array

#INTEGRAZIONE
#Trovo la lunghezza dell'intervallo tra due punti successivi
w = np.zeros(len(ascissa)-1)
integ=np.zeros(len(ascissa)-1)
dinteg=np.zeros(len(ascissa)-1)  #errore
integrale=np.zeros(len(ascissa)-1)
dintegrale=np.zeros(len(ascissa)-1)  #errore

# METODO INTEGRALE CON CICLO FOR

for i in range(0,len(ascissa)-1):
    
    w[i] = (ascissa[i+1]-ascissa[i])
    integ[i]=(w[i]*z[i])       #rettangoli
    #integ[i]=(w[i]*(sorted_z[i]+sorted_z[i+1]))*0.5  #metodo dei trapezi
    dinteg[i]=w[i]*dz[i]  #errore statistico
    if(i>0):    
        integrale[i] = integrale[i-1] + integ[i]
        dintegrale[i] = np.sqrt((dintegrale[i-1])**2 + (dinteg[i])**2)

# Creazione di due array shiftati per la x per fare integrazione senza for
sorted_z0 = sorted_z[:-1]  # taglio l'ultimo valore dell'array
sorted_z1= sorted_z[1:]   # taglio il primo valore dell'array

# METODO SENZA FOR PER INTEGRARE
delta = ascissa1 - ascissa0
integ2 = delta*0.5*(sorted_z1+sorted_z0)     # devo togliere l'ultimo valore di z per avere lung giusta
integrale2 = np.cumsum(integ2)
dint2=dz[::-1]
dinteg2=delta*dint2[:-1]  #errore statistico
dintegrale2= np.sqrt(np.cumsum(dinteg2**2))  #dinteg     #errore con somma in quadratura

print(f' con il for: {integrale} +- {dintegrale} \n\n senza for: {integrale2} +- {dintegrale2}')



######################################################################################
##########################  RUNNA QUESTO SE VUOI I GRAFICI ###########################
#######################################################################################

 
## Plot dell'energy density in funzione di 1/Nt*m e moltiplicata per Nt**2

# Titolo ed etichette per gli assi

plt.figure(1)

title = r'Energy density in funzione di $\frac{1}{Nt*m}$'
labelx = r'$\frac{1}{Nt*m}$'
labely = r'$\frac{ene_mass}{T^2}$'

plt.title(title)
plt.xlabel(labelx)
plt.ylabel(labely)

#Ho messo gli errori nei plot

idx = np.argsort(x)

sorted_y = np.take(y, idx) 
sorted_dy = np.take(dy, idx)
sorted_z = np.take(z, idx) 
sorted_dz = np.take(dz, idx)

ene_density=integrale2+sorted_y[1:]
dene_density=np.sqrt(dintegrale2**2+sorted_dy[1:]**2)   #errore con somma in quadratura
print(dene_density)

#plt.errorbar(ascissa, sorted_y, sorted_dy, marker ='*', linestyle = '', label = 'Integrand term')
plt.errorbar(x, y, dy, marker ='.', linestyle = '', label = 'Anomaly')
# plt.errorbar(x, y, dy, marker ='.', linestyle = '', label = 'Integrand term')
plt.errorbar(x, z, dz, marker ='.', linestyle = '', label = 'Integrand term')
plt.errorbar(ascissa1, ene_density, dene_density, marker ='.', linestyle = '', label = 'Energy Density')
#plt.errorbar(ascissa1, integrale, dintegrale, marker ='.', linestyle = '', label = 'Pressure (integral con for)')
plt.errorbar(ascissa1, integrale2, dintegrale2, marker ='.', linestyle = '', label = 'Pressure (integral senza for)')
xx = np.linspace(min(x), max(x), 1000)
plt.plot(xx, np.pi/6*np.ones(len(xx)), linestyle = '-')
plt.legend(loc='best')
plt.minorticks_on()

title = r'Pressione'
labelx = r'$\frac{1}{Nt*m}$'
labely = r'$\frac{ene_mass}{T^2}$'


plt.figure(2)

title = r'Energy Density (confronto con metodo anomalia)'
labelx = r'$\frac{1}{Nt*m}$'
labely = r'$\frac{ene_mass}{T^2}$'

plt.title(title)
plt.xlabel(labelx)
plt.ylabel(labely)

#Ho messo gli errori nei plot

plt.errorbar(ascissa1, ene_density, dene_density, marker ='.', linestyle = '', label = 'Metodo anomalia')
plt.errorbar(tsum, edenssut2, dedenssut2, marker ='.', linestyle = '', label = 'Metodo diretto')
#plt.errorbar(ascissa, integrale, dintegrale, marker ='.', linestyle = '', label = 'Anomalia di traccia')
xx = np.linspace(min(x), max(x), 1000)
plt.plot(xx, np.pi/6*np.ones(len(xx)), linestyle = '-')
plt.legend()
plt.minorticks_on()

plt.show()

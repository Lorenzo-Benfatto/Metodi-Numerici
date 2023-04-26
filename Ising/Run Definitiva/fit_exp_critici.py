import pylab
import numpy as np
from datetime import datetime
import matplotlib.pyplot as plt
import os
from scipy.optimize import curve_fit

# Dati reticolo
Nlatt = 30
vol = Nlatt**2

# Poiché i dati salvati nel file non sono ordinati per beta crescenti, prima li chiamiamo come variabili temporanee e il nome lo assegniamo (con la i?) dopo averli riordinati

#C_tmp, C_err_tmp, beta_tmp=np.loadtxt(rf"\\wsl.localhost\Ubuntu\home\dario\Documents\Metodi\Modulo1\Ising\Bootstrap\Nlatt={Nlatt}\suscettivita{Nlatt}.txt", unpack= True)
C_tmp, C_err_tmp, binder, binder_err, beta_tmp=np.loadtxt(rf"/home/dario/Documents/UNI/Metodi/Modulo1/Ising/Bootstrap/Nlatt={Nlatt}/Bootstrappo/magne(Nlatt={Nlatt}).txt", unpack= True)


# Definizione del beta critico (valore numerico da trovare con plot di binder in funzione di L)
beta_C = 0.435

# Codice per riscrivere gli array ordinati
indici = np.argsort(beta_tmp)   # funzione che restituisce gli indici dell'array vecchio in quello riordinato


for i in range(len(indici)):
    beta[i] = beta_tmp[indici[i]]
    C[i] = C_tmp[indici[i]]
    C_err[i] = C_err_tmp[indici[i]]


# Tagliamo le variabili per fare il fit (quindi prendendo da quando scende la curva)
inizio = np.argmax(C)

t = (np.abs(beta - beta_C))    # temperatura ridotta

# inizio = inizio + 13
x=t[inizio:]
y=C[inizio:]
dy=C_err[inizio:]
dx = np.zeros(len(x))

# outlier=4
# x=np.delete(x,outlier)
# y=np.delete(y,outlier)
# dy=np.delete(dy,outlier)
# dx=np.delete(dx,outlier)

def f(x,a,b,c):
    return a*x**(b)+c

init=(1,0,0)
popt, pcov=curve_fit(f, x, y, init, dy)
# m_fit, q_fit=popt
# dm_fit, dq_fit=np.sqrt(pcov.diagonal())
# print('primo fit m=%f +- %f' %(m_fit, dm_fit ) )


ndof=len(x)-len(init)
chi2=(((y-f(x, *popt))/dy)**2).sum()
print('passo zero')
print('popt:', popt)
print('dpopt:', np.sqrt(pcov.diagonal()))
print('chi2=%f, \nndof=%f' %(chi2, ndof))
dxy=dy
for i in range(0, 3, 1):
    dxy=np.sqrt(dxy**2+dx**2)
    popt, pcov=curve_fit(f, x, y, popt, dxy)
    chi2=(((y-f(x, *popt))/dxy)**2).sum()
    print('passo %d' % i)
    print('popt:', popt)
    print('dpopt:', np.sqrt(pcov.diagonal()))
    print('chi2=%f, \nndof=%f' %(chi2, ndof))


print('\n\n\n')


# Grafico della funzione di fit

xx=np.linspace(min(x), max(x)+0.0005, 1000)
plt.figure(2)
plt.subplot(211)
plt.errorbar(x, y, dy, dx,linestyle = '', color = 'black', marker = '.')
plt.plot( xx, f(xx, *popt), color='red')
plt.xscale('log')
plt.ylabel('Calore specifico')
plt.title('MODELLO DI ISING 2D \n Suscettività dopo il beta critico' )
plt.grid(color='gray')
plt.minorticks_on()


#residui normalizzati
plt.subplot(212)
r = (y-f(x,*popt))/dxy
plt.errorbar( x,r, linestyle='', marker='o')
plt.title('Residui normalizzati')
plt.xlabel(r'$ |t| = \beta-\beta_C$')
plt.ylabel('(dati - modello)/errore')
plt.grid(color='gray')
plt.show()

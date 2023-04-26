
import pylab
import numpy as np
from datetime import datetime
import matplotlib.pyplot as plt
import os
from scipy.optimize import curve_fit

# Dati reticolo
Nlatt = 40
vol = Nlatt**2

# Poiché i dati salvati nel file non sono ordinati per beta crescenti, prima li chiamiamo come variabili temporanee e il nome lo assegniamo (con la i?) dopo averli riordinati

#C_tmp, C_err_tmp, beta_tmp=np.loadtxt(rf"\\wsl.localhost\Ubuntu\home\dario\Documents\Metodi\Modulo1\Ising\Bootstrap\Nlatt={Nlatt}\suscettivita{Nlatt}.txt", unpack= True)
m_tmp,err_mag_tmp, beta_tmp=np.loadtxt(rf"C:\Users\aministratore\Documents\Università\Magistrale\Metodi Numerici\Modulo-1\Programmi_C\Ising\Bootstrap\Nuove_versioni\Nlatt = 40\Bootstrappo (2)\Magnetizzazione.txt", unpack= True)

m_tmp=np.abs(m_tmp)

# Definizione del beta critico (valore numerico da trovare con plot di binder in funzione di L)
beta_C = 0.44

# Codice per riscrivere gli array ordinati
indici = np.argsort(beta_tmp)   # funzione che restituisce gli indici dell'array vecchio in quello riordinato

beta=np.zeros(len(indici)-1)
m=np.zeros(len(indici)-1)
err_mag=np.zeros(len(indici)-1)

for i in range(0,len(indici)-1):
    print(i)
    beta[i] = beta_tmp[indici[i]]
    m[i] = m_tmp[indici[i]]
    err_mag[i] = err_mag_tmp[indici[i]]
    
t = (beta - beta_C)    # temperatura ridotta

'''
# Tagliamo le variabili per fare il fit (quindi prendendo da quando scende la curva)
inizio = np.argmax(C)

t = (np.abs(beta - beta_C))    # temperatura ridotta

inizio = inizio+2
x=t[inizio:]
y=C[inizio:]
dy=C_err[inizio:]
dx = np.zeros(len(x))

x=np.delete(x,8)
y=np.delete(y,8)
dy=np.delete(dy,8)
dx=np.delete(dx,8)

#x=np.delete(x,7)
#y=np.delete(y,7)
#dy=np.delete(dy,7)
#dx=np.delete(dx,7)
'''
x=t[31:]
y=m[31:]
dy=err_mag[31:]
dx=np.zeros(len(x))
''''
x=np.delete(x,1)
x=np.delete(x,8)
y=np.delete(y,1)
y=np.delete(y,8)
dx=np.delete(dx,1)
dx=np.delete(dx,8)
dy=np.delete(dy,1)
dy=np.delete(dy,8)
'''
def f(x,a,b):
    return a*x**(b)

init=(1,0.125)
popt, pcov=curve_fit(f, x, y, init, dy)
# m_fit, q_fit=popt
# dm_fit, dq_fit=np.sqrt(pcov.diagonal())
# print('primo fit m=%f +- %f' %(m_fit, dm_fit ) )


ndof=len(x)-1
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
pylab.figure(2)
pylab.subplot(211)
pylab.errorbar(x, y, dy, dx,linestyle = '', color = 'black', marker = '.')
pylab.plot( xx, f(xx, *popt), color='red')
plt.ylabel('Calore Specifico')
plt.title('MODELLO DI ISING 2D \n Calore Specifico' )
pylab.grid(color='gray')
pylab.minorticks_on()


#residui normalizzati
pylab.subplot(212)
r = (y-f(x,*popt))/dxy
pylab.errorbar( x,r, linestyle='', marker='o')
pylab.title('Residui normalizzati')
plt.xlabel(r'$ |t| = \beta-\beta_C$')
plt.ylabel('(dati - modello)/errore')
pylab.grid(color='gray')

pylab.show()
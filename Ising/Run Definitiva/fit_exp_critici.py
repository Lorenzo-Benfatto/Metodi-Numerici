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

# Definizione del beta critico (valore numerico da trovare con plot di binder in funzione di L)
beta_C = 0.435

############################## CALORE SPECIFICO ####################################

C_tmp, C_err_tmp, beta_tmp=np.loadtxt(rf"/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Run Definitiva/Nlatt={Nlatt}/Bootstrappo/calo(Nlatt={Nlatt}).txt", unpack= True)


# Codice per riscrivere gli array ordinati
indici = np.argsort(beta_tmp)   # funzione che restituisce gli indici dell'array vecchio in quello riordinato

C=np.zeros(len(indici))
C_err=np.zeros(len(indici))
beta=np.zeros(len(indici))

for i in range(len(indici)):
    beta[i] = beta_tmp[indici[i]]
    C[i] = C_tmp[indici[i]]
    C_err[i] = C_err_tmp[indici[i]]


# Tagliamo le variabili per fare il fit (quindi prendendo da quando scende la curva)
inizio1 = np.argmax(C)

t = (np.abs(beta - beta_C))    # temperatura ridotta

inizio1 = inizio1 + 4
x=t[inizio1:]
y1=C[inizio1:]
dy1=C_err[inizio1:]
dx = np.zeros(len(x))
'''
outlier=1,4,5,6
x=np.delete(x,outlier)
y=np.delete(y,outlier)
dy=np.delete(dy,outlier)
dx=np.delete(dx,outlier)
'''
def f(x,a,b,c):
    return a*x**(b)+c


init=(1,0,0)
popt, pcov=curve_fit(f, x, y1, init, dy1)
# m_fit, q_fit=popt
# dm_fit, dq_fit=np.sqrt(pcov.diagonal())
# print('primo fit m=%f +- %f' %(m_fit, dm_fit ) )


ndof=len(x)-len(init)
chi2=(((y1-f(x, *popt))/dy1)**2).sum()
print('************** CALORE SPECIFICO *************')
print('passo zero')
print('popt:', popt)
print('dpopt:', np.sqrt(pcov.diagonal()))
print('chi2=%f, \nndof=%f' %(chi2, ndof))
dxy=dy1
for i in range(0, 3, 1):
    dxy=np.sqrt(dxy**2+dx**2)
    popt, pcov=curve_fit(f, x, y1, popt, dxy)
    chi2=(((y1-f(x, *popt))/dxy)**2).sum()
    print('passo %d' % i)
    print('popt:', popt)
    print('dpopt:', np.sqrt(pcov.diagonal()))
    print('chi2=%f, \nndof=%f' %(chi2, ndof))


print('\n\n\n')


# Grafico della funzione di fit

xx=np.linspace(min(x), max(x)+0.0005, 1000)
plt.figure(1)
plt.subplot(211)
plt.errorbar(x, y1, dy1, dx,linestyle = '', color = 'black', marker = '.')
plt.plot( xx, f(xx, *popt), color='red')
plt.xscale('log')
plt.ylabel('Calore specifico')
plt.title('MODELLO DI ISING 2D \n Calore specifico dopo il beta critico' )
plt.grid(color='gray')
plt.minorticks_on()


#residui normalizzati
plt.subplot(212)
r = (y1-f(x,*popt))/dxy
plt.errorbar( x,r, linestyle='', marker='o')
plt.title('Residui normalizzati')
plt.xlabel(r'$ |t| = \beta-\beta_C$')
plt.ylabel('(dati - modello)/errore')
plt.grid(color='gray')



############################## SUSCETTIVITA ####################################


sushi_tmp, sushi_err_tmp, binder, binder_err, beta_tmp=np.loadtxt(rf"/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Run Definitiva/Nlatt={Nlatt}/Bootstrappo/sushi(Nlatt={Nlatt}).txt", unpack= True)


# Codice per riscrivere gli array ordinati
indici = np.argsort(beta_tmp)   # funzione che restituisce gli indici dell'array vecchio in quello riordinato

sushi=np.zeros(len(indici))
sushi_err=np.zeros(len(indici))
beta=np.zeros(len(indici))

for i in range(len(indici)):
    beta[i] = beta_tmp[indici[i]]
    sushi[i] = sushi_tmp[indici[i]]
    sushi_err[i] = sushi_err_tmp[indici[i]]


# Tagliamo le variabili per fare il fit (quindi prendendo da quando scende la curva)
inizio = np.argmax(sushi)

#t = (np.abs(beta - beta_C))    # temperatura ridotta

inizio = inizio + 4
x=t[inizio:]
y=sushi[inizio:]
dy=sushi_err[inizio:]
dx = np.zeros(len(x))
'''
outlier=1,4,5,6
x=np.delete(x,outlier)
y=np.delete(y,outlier)
dy=np.delete(dy,outlier)
dx=np.delete(dx,outlier)
'''
def f(x,a,b,c):
    return a*x**(b)+c


init=(1,0,0)
popt, pcov=curve_fit(f, x, y, init, dy)
# m_fit, q_fit=popt
# dm_fit, dq_fit=np.sqrt(pcov.diagonal())
# print('primo fit m=%f +- %f' %(m_fit, dm_fit ) )


ndof=len(x)-len(init)
chi2=(((y-f(x, *popt))/dy)**2).sum()
chi2=(((y-f(x, *popt))/dy)**2).sum()
print('************** SUSCETTIVITA *************')
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
plt.ylabel('Suscettività')
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


############################## MAGNETIZZAZIONE ####################################

magne_tmp, magne_err_tmp, beta_tmp=np.loadtxt(rf"/Users/aministratore/Documents/Università/Magistrale/Metodi Numerici/Modulo-1/Programmi_C/Ising/Run Definitiva/Nlatt={Nlatt}/Bootstrappo/magne(Nlatt={Nlatt}).txt", unpack= True)


# Codice per riscrivere gli array ordinati
indici = np.argsort(beta_tmp)   # funzione che restituisce gli indici dell'array vecchio in quello riordinato

magne=np.zeros(len(indici))
magne_err=np.zeros(len(indici))
beta=np.zeros(len(indici))

for i in range(len(indici)):
    beta[i] = beta_tmp[indici[i]]
    magne[i] = magne_tmp[indici[i]]
    magne_err[i] = magne_err_tmp[indici[i]]


# Tagliamo le variabili per fare il fit (quindi prendendo da quando scende la curva)
#inizio2 = np.argmax(magne)

t = (np.abs(beta - beta_C))    # temperatura ridotta

inizio2=16
#inizio2 = inizio2 + 6

x=t[inizio2:]
y2=magne[inizio2:]
dy2=magne_err[inizio2:]
dx = np.zeros(len(x))
'''
outlier=1,4,5,6
x=np.delete(x,outlier)
y=np.delete(y,outlier)
dy=np.delete(dy,outlier)
dx=np.delete(dx,outlier)
'''
def f(x,a,b,c):
    return a*x**(b)+c


init=(-1,-1,0)
popt, pcov=curve_fit(f, x, y2, init, dy2)
# m_fit, q_fit=popt
# dm_fit, dq_fit=np.sqrt(pcov.diagonal())
# print('primo fit m=%f +- %f' %(m_fit, dm_fit ) )


ndof=len(x)-len(init)
chi2=(((y2-f(x, *popt))/dy2)**2).sum()
chi2=(((y2-f(x, *popt))/dy2)**2).sum()
print('************** MAGNETIZZAZIONE *************')
print('passo zero')
print('popt:', popt)
print('dpopt:', np.sqrt(pcov.diagonal()))
print('chi2=%f, \nndof=%f' %(chi2, ndof))
dxy=dy2
for i in range(0, 3, 1):
    dxy=np.sqrt(dxy**2+dx**2)
    popt, pcov=curve_fit(f, x, y2, popt, dxy)
    chi2=(((y2-f(x, *popt))/dxy)**2).sum()
    print('passo %d' % i)
    print('popt:', popt)
    print('dpopt:', np.sqrt(pcov.diagonal()))
    print('chi2=%f, \nndof=%f' %(chi2, ndof))


print('\n\n\n')


# Grafico della funzione di fit

xx=np.linspace(min(x), max(x)+0.0005, 1000)
plt.figure(3)
plt.subplot(211)
plt.errorbar(x, y2, dy2, dx,linestyle = '', color = 'black', marker = '.')
plt.plot( xx, f(xx, *popt), color='red')
plt.xscale('log')
plt.ylabel('Magnetizzazione')
plt.title('MODELLO DI ISING 2D \n Magnetizzazione dopo il beta critico' )
plt.grid(color='gray')
plt.minorticks_on()


#residui normalizzati
plt.subplot(212)
r = (y2-f(x,*popt))/dxy
plt.errorbar( x,r, linestyle='', marker='o')
plt.title('Residui normalizzati')
plt.xlabel(r'$ |t| = \beta-\beta_C$')
plt.ylabel('(dati - modello)/errore')
plt.grid(color='gray')




plt.show()

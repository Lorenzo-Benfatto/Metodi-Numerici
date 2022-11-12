# A program to plot the C(k) for different beta values for a given lattice lenght

import pylab
import numpy as np
from datetime import datetime
import matplotlib.pyplot as plt
import os

from datetime import datetime

startTime = datetime.now()

# assign directory
directory = r"\\wsl.localhost\Ubuntu\home\dario\Documents\Metodi\Modulo1\Ising\Bootstrap\Nlatt=20\CK"

pylab.figure(1)
pylab.title('MODELLO DI ISING 2D \n C(k) funzione di autocorrelazione a 2 punti')
pylab.xlabel('k')
pylab.ylabel(r'$C(k)$')

# iterate over files in that directory
for filename in os.listdir(directory):
    f = os.path.join(directory, filename)
    # checking if it is a file
    if os.path.isfile(f):
        if f.find('txt')!=-1:
            print(f)
            # nella prima colonna ho il numero di k e nella seconda il c(k) corrispondente
            n, ck =np.loadtxt(f, unpack=True)
            inizio = f.find('eta)')
            if(inizio!=-1):
                inizio = inizio + 4
                fine = inizio + 4

            beta=float(f[inizio:fine])

            pylab.errorbar(n, ck, linestyle = '--', marker ='.', label = fr'$\beta$ = {beta}')
            print(f'beta = {beta}')
            pylab.legend()
            pylab.minorticks_on()

pylab.show()

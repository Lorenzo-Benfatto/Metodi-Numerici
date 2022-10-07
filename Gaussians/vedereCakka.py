import pylab
import numpy as np
from datetime import datetime

startTime = datetime.now()

x,y=np.loadtxt('Cakka.txt', unpack=True)

pylab.figure(1)
pylab.rc('font',size=10)
pylab.title('C(k) vs k')
pylab.xlabel('k')
pylab.ylabel('C(k)')
pylab.errorbar(y,x, linestyle = '-', color = 'red', marker ='.', markersize=1)
pylab.minorticks_on()

print(datetime.now() - startTime)
pylab.show()

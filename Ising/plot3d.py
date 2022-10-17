from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt
import pylab

N=10  #lato reticolo

fig = plt.figure()
ax = plt.axes(projection='3d')

z=np.loadtxt('lattice', unpack=False)
#z=np.loadtxt('beta3.txt', unpack=False)

x=np.arange(N)
y=np.arange(N)
X,Y=np.meshgrid(x,y)

ax.scatter3D(X,Y,z,cmap='Greens',marker=".");
pylab.show()

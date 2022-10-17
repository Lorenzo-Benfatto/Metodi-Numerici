from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt
import pylab

N=50  #lato reticolo

l=np.loadtxt("beta.txt",unpack=True)

for i in range (0,len(l)):
    beta=np.loadtxt(f'beta{i}.txt', unpack=True)
    fig = plt.figure(i)
    ax = plt.axes(projection='3d')
    x=np.arange(N)
    y=np.arange(N)
    X,Y=np.meshgrid(x,y)
    ax.scatter3D(X,Y,beta,cmap='Greens',marker=".")
    plt.title(f"Beta: {l[i]}")

pylab.show()

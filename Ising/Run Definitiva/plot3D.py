from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt
import pylab

N=40  #lato reticolo

l=np.loadtxt(r"\\wsl.localhost\Ubuntu\home\dario\Documents\Metodi\Modulo1\Ising\beta_init.txt",unpack=True)

for i in range (0,len(l)):
    beta=np.loadtxt(fr"\\wsl.localhost\Ubuntu\home\dario\Documents\Metodi\Modulo1\Ising\reticolo40\beta{i}.txt", unpack=True)
    fig = plt.figure(i)
    ax = plt.axes(projection='3d')
    x=np.arange(N)
    y=np.arange(N)
    X,Y=np.meshgrid(x,y)
    ax.scatter3D(X,Y,beta,cmap='Greens',marker=".")
    plt.title(f"Beta: {l[i]}")

pylab.show()
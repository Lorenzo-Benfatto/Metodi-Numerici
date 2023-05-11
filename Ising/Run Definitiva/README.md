In this folder there are all the files necessary to complete the Module.
\\
One needs to (in order):
\\
modify simulatore_boot.h for the different lattice measures
\\
simulatore_boot.c (it needs beta.txt and MODIFY init.txt)
\\
ising_bootstrappato.c
\\
analisiboot.py (optional, just to watch the energy plot)
\\
Plot_C-Chi.py
\\
stima_Beta_C
\\
fit_exp_crit
\\
for_plot.py (to plot different lattice data in the same figure)
\\
FSS.py
\\
storiapontecarlo.py to look at the Markov chain history
\\
In order to look at the physical situation of polarized spins, one can also run Plot3d.py
\\
boot_diversi_resamp.c and ResamplingBootPlot.py to look at the optimal number of resamplings in the binned bootstrap
\\
for_plot.py now fits -\gamma/\nu taking the maximum of the suceptivity for the various lattices and fitting the whatever function 
\\
Run C(k).c and C(k)_view.py to look at correlations (remember to change ene in mag according to the interesting stuff)
\\
All the others .h are necessary for the programs to work

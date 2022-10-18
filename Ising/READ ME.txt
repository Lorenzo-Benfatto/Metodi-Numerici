This page contains simulatore.c which is a program that does a Metropolis Montecarlo simulation on a 2d lattice of spins to simulate the Ising Model.
The program itself is very simple since most of the functions used are stored in the simulazione.h library and some in the funzioni.h one.
Large use is made of lists and pointers altough their true Nature still remains a mistery to unfold, they are therefore treated as magical objects with a will of their own.
To run it one needs an "input.txt" program that lists the starting state (hot or cold), the number of measures to make on a single spin, the number of iterations and 
the magnetic field.
In the file "beta.txt" one must store the different temperatures at which the algorithm is ran.
The python files are there to analize the results of the simulations, plotting a 3d version of the lattice and looking at the order parameter.
Regarding the Ising model itself: It undergoes a phase transition around beta=0.43.
As a theoretical physicist who studied both QFTs and Statistical Physics I can safely say that a phase transition is what happens when a system does something no
good christian should ever do. 
What is sometimes called a Second Order phase transition, moreover, is what happens when the Devil itself starts wroathing havoc on your system.
Critical exponents, non-linear symmetry breaking and group theory are all words you will not find in the Bible.
The program itself works, I'm currently simulating it for various dimensions of the lattice, to see how many iterations it takes to summon a Maxwell Demon.

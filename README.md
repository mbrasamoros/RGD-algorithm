# RGD-algorithm
The right-generators descendant of a numerical semigroup
AUTHORS: M. Bras-Amoros and J. Fernandez-Gonzalez

This repository contains the codes to compute the number of numerical semigroups of a given genus explained in 
M. Bras-Amorós, J. Fernández-González: The Right-Generators Descendant of a Numerical Semigroup, Mathematics of Computation, American Mathematical Society, 2020.

See the entry http://oeis.org/A007323 in the On-line Encyclopedia of Integer Sequences.

One can compile the non-parallelized version:

> gcc -Wall -o rgd.out RGD.cpp

and then execute it to compute the number of numerical semigroups of a given genus. 
For instance, to obtain the number of semigroups of genus 35:

> ./rgd.out 35

and the output will be

> n35=66687201
> time taken 2 (without parallelization)

Alternatively, one can compile the Cilk++ parallelized version:

> g++ -std=c++11 -fcilkplus -g -Wall -O3 -o rgdcilk.out RGD_cilk.cpp

and then execute it to compute the number of numerical semigroups of a given genus. 
For instance, to obtain the number of semigroups of genus 40:

> ./rgdcilk.out 40

and the output will be:

> n40=774614284
> time taken 3 (8 workers)

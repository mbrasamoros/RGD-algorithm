# RGD-algorithm

This repository hosts the implementation of an algorithm to compute the number of numerical semigroups of each given genus, as it is presented in the following article:

M. Bras-Amorós, J. Fernández-González: "The right-generators descendant of a numerical semigroup", Mathematics of Computation, American Mathematical Society, 2020. See https://www.ams.org/journals/mcom/0000-000-00/S0025-5718-2020-03502-9/

For a quick survey on the sequence of the number of numerical semigroups of each given genus and the conjectures related to it see the entry http://oeis.org/A007323 in the On-line Encyclopedia of Integer Sequences.

One can compile the non-parallelized version as follows.

> gcc -Wall -o rgd.out RGD.cpp

and then execute it to compute the number of numerical semigroups of a given genus.
For instance, to obtain the number of semigroups of genus 35 the command is

> ./rgd.out 35

and the output will be

> n35=66687201
> time taken 2 (without parallelization)


Alternatively, one can compile the Cilk++ parallelized version as follows.

> g++ -std=c++11 -fcilkplus -g -Wall -O3 -o rgdcilk.out RGD_cilk.cpp

and then execute it to compute the number of numerical semigroups of a given genus.
For instance, to obtain the number of semigroups of genus 40 the command is

> ./rgdcilk.out 40

and the output will be

> n40=774614284
> time taken 3 (8 workers)


The next values were first computed using this code:

n71 = 2604033182682582

n72 = 4218309716540814

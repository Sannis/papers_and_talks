#!/bin/bash

clear

mpirun -np 1 ./wdk-runge-kutta-4.e -n 256 --npx=1 --npy=1 -l 5 --impulse-file=../../data/gauss_n256_l5.cpl --results-root=./results/Home \
       -r 0 -z 0.001 -s 1000 -d 10

mpirun -np 2 ./wdk-runge-kutta-4.e -n 256 --npx=1 --npy=2 -l 5 --impulse-file=../../data/gauss_n256_l5.cpl --results-root=./results/Home \
       -r 0 -z 0.001 -s 1000 -d 10

mpirun -np 2 ./wdk-runge-kutta-4.e -n 256 --npx=2 --npy=1 -l 5 --impulse-file=../../data/gauss_n256_l5.cpl --results-root=./results/Home \
       -r 0 -z 0.001 -s 1000 -d 10

mpirun -np 4 ./wdk-runge-kutta-4.e -n 256 --npx=2 --npy=2 -l 5 --impulse-file=../../data/gauss_n256_l5.cpl --results-root=./results/Home \
       -r 0 -z 0.001 -s 1000 -d 10


#!/bin/bash

queue=test

npxy_list=
npxy_list="$npxy_list 1=1x1 2=1x2 2=2x1"
npxy_list="$npxy_list 4=1x4 4=4x1 4=2x2"
npxy_list="$npxy_list 8=1x8 8=2x4 8=4x2 8=8x1"
npxy_list="$npxy_list 16=1x16 16=2x8 16=4x4 16=8x2 16=16x1"
npxy_list="$npxy_list 32=1x32 32=2x16 32=4x8 32=8x4 32=16x2 32=32x1"
npxy_list="$npxy_list 64=1x64 128=1x128"

n_list="512,0.0001 1024,0.0001 2048,0.0001 4096,0.0001 8192,0.0001 16384,0.0001"



clear

DIRNAME=

#run_skif_job $np $npx $npy $n $dz $save_steps
run_skif_job()
{	
	run="mpirun -np ${1} -q ${queue} ./wdk-runge-kutta-4.e -n ${4} --npx=${2} --npy=${3} -l 5 \
	       --impulse-file=../../data/gauss_n${4}_l5.cpl --results-root=./results/Skif/${DIRNAME} \
	       -r 5 -z ${5} -d ${6}"
	
	if [ -e "./results/Skif/${DIRNAME}/N${4}_NP${1}=${2}x${3}_R5.000000" ]
	then
		echo "Task for np=${1}(${2}x${3}), N=${4}, dz=${5}, save_steps=${6} already running."
	else
		echo "Running for np=${1}(${2}x${3}), N=${4}, dz=${5}, save_steps=${6}"
	
		$run
	fi
}


for save_steps in -1 10
do
	if [ "$save_steps" = "10" ]
	then
		DIRNAME="Time_with_save"
	else
		DIRNAME="Time_no_save"
	fi
	
	for ndz in $n_list
	do
		n=`echo $ndz | sed -e 's/\([0-9]*\),[.0-9]*/\1/'`
		dz=`echo $ndz | sed -e 's/[0-9]*,\([.0-9]*\)/\1/'`
		
		for npxy in `echo $npxy_list`
		do
			np=`echo $npxy | sed -e 's/\([0-9]*\)=[0-9]*x[0-9]*/\1/'`
			npx=`echo $npxy | sed -e 's/[0-9]*=\([0-9]*\)x[0-9]*/\1/'`
			npy=`echo $npxy | sed -e 's/[0-9]*=[0-9]*x\([0-9]*\)/\1/'`
		
			run_skif_job $np $npx $npy $n $dz $save_steps
		done
	done
done

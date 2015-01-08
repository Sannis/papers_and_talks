#!/bin/bash

if [ "$1" = "" ]
then
	queue="bigmem"
else
	queue="$1"
fi

maxtime="300"

np_list="1 2 4 8 16 24 32 48 64"

n_list="512 1024 2048 4096 8192 16384"

dz="0.01"

clear

DIRNAME="debug"

#run_skif_job $np $n $dz $save_steps
run_skif_job()
{	
	run="mpirun -np ${1} -q ${queue} -maxtime ${maxtime} ./wdk-fftw.e -n ${2} -l 5 --results-root=./results/Skif/${DIRNAME} --fftw-no-buffer -r 5 -z ${3} -d ${4}"
	
	if [ -e "./results/Skif/${DIRNAME}/N${2}_NP${1}=1x${1}_R5.000000" ]
	then
	    echo "" > /dev/null
	else
		echo $run
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
	
	for n in $n_list
	do
		for np in $np_list
		do
			run_skif_job $np $n $dz $save_steps
		done
	done
done


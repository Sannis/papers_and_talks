#!/bin/bash

if [ "$1" = "" ]
then
	queue="bigmem"
else
	queue="$1"
fi

maxtime="300"

np_list="4 8 16 32 64"

n_list="512 1024 2048 4096 8192 16384"

clear

#run_skif_job $np $n $dz $save_steps
run_skif_job()
{	
	run="mpirun -np ${1} -q ${queue} -maxtime ${maxtime} ./wdk-sweep.e ${2} 5 ${3}"
	
	if [ "$3" = "s" ]
	then
		DIRNAME="Time_with_save"
	else
		DIRNAME="Time_no_save"
	fi

	if [ -e "./results/Skif/${DIRNAME}/N${2}_NP${1}_R5.000000" ]
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
		MODE="s"
	else
		MODE="n"
	fi
	
	for n in $n_list
	do
		for np in $np_list
		do
			run_skif_job $np $n $MODE
		done
	done
done


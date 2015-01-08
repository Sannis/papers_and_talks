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

#echo_skif_job $np $n $nobuffer_flag $transpose_flag $measure_flag
echo_skif_job()
{
	flags=
	dirname="./results/Skif/Compare/"
	
	if [ "$3" = "1" ]
	then
		flags="$flags --fftw-no-buffer"
		dirname="${dirname}nobuffer_"
	else
		dirname="${dirname}withbuffer_"
	fi
	
	if [ "$4" = "1" ]
	then
		flags="$flags --fftw-do-transpose"
		dirname="${dirname}normalorder_"
	else
		dirname="${dirname}transposeorder_"
	fi
	
	if [ "$5" = "1" ]
	then
		flags="$flags --fftw-do-measure"
		dirname="${dirname}withmeasure"
	else
		dirname="${dirname}nomeasure"
	fi
	
	cmd="mpirun -np ${1} -q ${queue} -maxtime ${maxtime} ./wdk-fftw.e -n ${2} -l 10 -r 0 -d 100 --results-root=${dirname} -z ${dz} ${flags}"
	
	if [ -e "${dirname}/N${2}_NP${1}=1x${1}_R0.000000" ]
	then
		echo "#" > /dev/null
	else
		echo "$cmd"
	fi
}


for n in $n_list
do
	for np in $np_list
	do
		echo_skif_job $np $n 0 0 0
		echo_skif_job $np $n 1 0 0
		echo_skif_job $np $n 0 1 0
		echo_skif_job $np $n 1 1 0
		echo_skif_job $np $n 0 0 1
		echo_skif_job $np $n 1 0 1
		echo_skif_job $np $n 0 1 1
		echo_skif_job $np $n 1 1 1
	done
done


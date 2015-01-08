#!/bin/bash

np_list="1 2 4 8 16 24 32 48 64"
n_list="512 1024 2048 4096 8192 16384"

mkdir -p ../../docs/results/tables/Bluegene/
mkdir -p ../../docs/results/tables/Skif/

NULLS="00000.00    00.000  000.00   "

#write_to_log_file $np $n $subdirname $filename
strip_log_file()
{
	dirname=`ls -1 ./results/Skif/$3 | grep N${2}_NP${1} | head -n 1`
	
	logfilename="./results/Skif/$3/${dirname}/log.txt"
	
	if [ -e $logfilename ]
	then
		if [ "`tail -n 2 $logfilename | head -n 1 | grep Exec | sed -e 's/Exec.*/Exec/'`" = "Exec" ]
        then
            new_log_head=`head -n 16 $logfilename`
            new_log_tail=`tail -n 4 $logfilename`
            
            mv $logfilename $logfilename.bak
            
            echo "${new_log_head}" > $logfilename
            echo "${new_log_tail}" >> $logfilename
        fi
	fi
}

# Main loop

mkdir -p ./tmp

for save_steps in -1 10
do
	if [ "$save_steps" = "10" ]
	then
		subdirname="Time_with_save"
	else
		subdirname="Time_no_save"
	fi

	echo -n "Processing ${subdirname}...  "

	for np in $np_list
	do
		echo -n "np=$np...  "
	
		for n in $n_list
		do
			strip_log_file $np $n $subdirname $datfilename
		done
	done

	echo "[Finished]"
done


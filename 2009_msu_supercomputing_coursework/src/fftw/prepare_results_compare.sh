#!/bin/bash

np_list_2k="1 2 4 8 16 32 64"
np_list_all="1 2 4 8 16 24 32 48 64"
n_list="512 1024 2048 4096 8192 16384"

# Create directories for tables

mkdir -p ../../docs/results/tables/Skif/
mkdir -p ../../docs/results/tables/Bluegene/

#write_to_log_file $np $n $nobuffer_flag $transpose_flag $measure_flag $filename
write_to_log_file()
{
	dirname="./results/Skif/Compare/"
	
	if [ "$3" = "1" ]
	then
		dirname="${dirname}nobuffer_"
		prefix="nobuff_"
	else
		dirname="${dirname}withbuffer_"
		prefix="buff_"
	fi
	
	if [ "$4" = "1" ]
	then
		dirname="${dirname}normalorder_"
		prefix="${prefix}normal"
	else
		dirname="${dirname}transposeorder_"
		prefix="${prefix}transpose"
	fi
	
	if [ "$5" = "1" ]
	then
		dirname="${dirname}withmeasure"
		prefix="${prefix}_measure"
	else
		dirname="${dirname}nomeasure"
		prefix="${prefix}"
	fi
	
	logfilename="${dirname}/N${2}_NP${1}=1x${1}_R0.000000/log.txt"
	
	if [ -e $logfilename ]
	then
		exec_time=`tail -n 1 $logfilename | sed -e 's/00[0-9][0-9][0-9].*//' | sed -e 's/n.*//'`
		
		if [ "$exec_time" != "" ]
		then
			exec_time=`perl -e "printf(\"%10.6f\", ${exec_time}/100)"`
			
			echo -n "${exec_time}      " >> $6
		else
			echo -n "0              " >> $6
		fi
	else
		echo -n "0              " >> $6
	fi
}

# Main loop

for n in $n_list
do
	echo "Processing N=$n...  "
	
	datfilename_start="../../docs/results/tables/Skif/FFTW_compare_N${n}"
	
	for type in 2k all
	do
		datfilename="${datfilename_start}_${type}.dat"
		
		rm -f $datfilename
		touch $datfilename
		#chmod 644 $datfilename
		# For debugging
		#datfilename="/dev/tty"
		
		echo "N = $n" >> $datfilename
		echo "np   buff_trans   nobuff_trans      buff_norm     nobuff_norm    buff_trans_meas  nobuff_trans_meas    buff_norm_meas    nobuff_norm_meas" >> $datfilename
		
		echo -n "$type: "
		
		for np in $np_list_all
		do
			if [ "$type" = "2k" ]
			then
				flag=`echo " $np_list_2k " | grep " $np "`
				
				if [ "$flag" = "" ]
				then
					continue
				fi
			fi
			
			echo -n "np=$np...  "
			
			printf "%2d   " $np >> $datfilename
			write_to_log_file $np $n 0 0 0 $datfilename
			write_to_log_file $np $n 1 0 0 $datfilename
			write_to_log_file $np $n 0 1 0 $datfilename
			write_to_log_file $np $n 1 1 0 $datfilename
			write_to_log_file $np $n 0 0 1 $datfilename
			write_to_log_file $np $n 1 0 1 $datfilename
			write_to_log_file $np $n 0 1 1 $datfilename
			write_to_log_file $np $n 1 1 1 $datfilename
			echo "" >> $datfilename
		done
		
		echo ""
		
		if [ "$1" != "" ]
		then
			echo "Result:"
			echo "============="
			cat $datfilename
			echo "============="
		fi
	done
	
done

echo "[Finished]"


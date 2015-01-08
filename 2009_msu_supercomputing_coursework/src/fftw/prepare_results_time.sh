#!/bin/bash

np_list_2k="1 2 4 8 16 32 64"
np_list_all="1 2 4 8 16 24 32 48 64"
n_list="512 1024 2048 4096 8192 16384"

# Create directories for tables

mkdir -p ../../docs/results/tables/Skif/
mkdir -p ../../docs/results/tables/Bluegene/

NULLS="     -        -        -     "

#write_to_log_file $np $n $subdirname $filename $type
write_to_log_file()
{
	dirname="./results/Skif/$3"
	
	logfilename="${dirname}/N${2}_NP${1}=1x${1}_R5.000000/log.txt"
	
	if [ -e $logfilename ]
	then
		if [ "`tail -n 2 $logfilename | head -n 1 | grep Exec | sed -e 's/Exec.*/Exec/'`" = "Exec" ]
		then
			exec_time=`tail -n 1 $logfilename`
			
			if [ "${1}" = "1" ]
			then
				echo ${exec_time} > ./tmp/${2}.time
			fi
			
			if [ -e "./tmp/${2}.time" ]
			then
				exec_time_1=`cat ./tmp/${2}.time`
			else
				exec_time_1="1"
			fi
			
			if [ "$exec_time" != "" ]
			then
				exec_time=`echo "$exec_time" | sed -e 's/\(.*\)\.\([0-9][0-9]\).*/\1.\2/'`
				exec_time=`perl -e "printf(\"%8.2lf\", ${exec_time})"`
				acceleration=`perl -e "printf(\"%6.2lf\", ${exec_time_1}/${exec_time})"`
				efficacy=`perl -e "printf(\"%6.2lf\", ${acceleration}/${1}*100)"`
				
				printf "%s   %s   %s   " "${exec_time}"    "${acceleration}"    "${efficacy}" >> $4
			else
				echo -n "$NULLS" >> $4
			fi
		else
			echo -n "$NULLS" >> $4
		fi
	else
		echo -n "$NULLS" >> $4
	fi
}

# Main loop

mkdir ./tmp

for save_steps in -1 10
do
	if [ "$save_steps" = "10" ]
	then
		subdirname="Time_with_save"
		postfix="withsave"
		graphtitle="(с сохранением на каждом 10 шаге)"
	else
		subdirname="Time_no_save"
		postfix="nosave"
		graphtitle="(без сохранения результатов)"
	fi
	
	echo "Processing ${subdirname}...  "
	
	datfilename_start="../../docs/results/tables/Skif/FFTW_time_${postfix}"
	
	for type in 2k all
	do
		datfilename="${datfilename_start}_${type}.dat"
		
		rm -f $datfilename
		touch $datfilename
		#chmod 644 $datfilename
		# For debugging
		#datfilename="/dev/tty"
		
		echo -n "np   " >> $datfilename
		
		for n in $n_list
		do
			printf "    %s %6d                " "N=" ${n} >> $datfilename
		done
		
		echo "" >> $datfilename
		
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
			
			for n in $n_list
			do
				write_to_log_file $np $n $subdirname $datfilename
			done
			
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

rm -rf ./tmp

echo "[Finished]"


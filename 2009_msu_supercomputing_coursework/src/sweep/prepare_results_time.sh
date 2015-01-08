#!/bin/bash

np_standard="4"
np_list_all="4 8 16 32 64"
n_list="512 1024 2048 4096 8192 16384"

# Create directories for tables

mkdir -p ../../docs/results/tables/Bluegene/
mkdir -p ../../docs/results/tables/Skif/

NULLS="     -        -        -     "

#write_to_log_file $np $n $subdirname $filename
write_to_log_file()
{
	dirname=`ls -1 ./results/Skif/$3 | grep N${2}_NP${1} | head -n 1`
	
	logfilename="./results/Skif/$3/${dirname}/log.txt"
	
	if [ -e $logfilename ]
	then
		if [ "`tail -n 2 $logfilename | head -n 1 | grep Exec | sed -e 's/Exec.*/Exec/'`" = "Exec" ]
		then
			exec_time=`tail -n 1 $logfilename | sed -e 's/\(.*\)\.\([0-9][0-9]\).*/\1.\2/'`
			
			if [ "${1}" = "${np_standard}" ]
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
				acceleration=`perl -e "printf(\"%6.2lf\", ${np_standard}*${exec_time_1}/${exec_time})"`
				exec_time=`perl -e "printf(\"%8.2lf\", ${exec_time})"`
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

mkdir -p ./tmp

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

	echo -n "Processing ${subdirname}...  "

	datfilename="../../docs/results/tables/Skif/Sweep_time_${postfix}.dat"

	rm -f $datfilename

	# For debugging
	#datfilename="/dev/tty"

	echo -n "np   " >> $datfilename

	for n in $n_list
	do
		printf "    %s %6d                " "N=" ${n} >> $datfilename
	done

	echo "" >> $datfilename

	for np in $np_list_all
	do
		echo -n "np=$np...  "
	
		printf "%2d   " $np >> $datfilename
	
		for n in $n_list
		do
			write_to_log_file $np $n $subdirname $datfilename
		done

		echo "" >> $datfilename
	done

	echo "[Finished]"
	
	if [ "$1" != "" ]
	then
		echo "Result:"
		echo "============="
		cat $datfilename
		echo "============="
	fi
done

rm -rf ./tmp


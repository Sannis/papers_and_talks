#!/bin/bash

NP=4

n_list="256 512 1024 2048 4096 8192 16384"

# Host specific variables
HOSTNAME=`hostname`

if [ "$HOSTNAME" = "ktg1.phys.msu.ru" ] # ILC MSU cluster
then
	MPIRUN=qsub
else
	if [ "$HOSTNAME" = "oleg-pc" ] # Oleg Efimov's home PC
	then
		MPIRUN=mpiexec
		n_list=256
	else
		if [ "$HOSTNAME" = "fen1" ] # CS MSU IBM Bluegene/P cluster
		then
			MPIRUN="mpisubmit.bg"
		else # SRCC MSU SKIF cluster
			QUEUETEST=`tasks -q $1 2>&1 | grep "not exists"`
			if [ "$QUEUETEST" = "" ] # Queue exists
			then
				MPIRUN="mpirun -q $1"
			else
				MPIRUN="mpirun -q test"
			fi
				
		fi
	fi
fi

if [ "$1" = "cli" ]
then
    RUN=
    n_list=256
else
    RUN="$MPIRUN -np $NP"
fi

# Make folder for data
mkdir ./data 2> /dev/null

# Generate impulse files
for n in $n_list
do
	$RUN ./progs/create_2d_func/create_2d_func -n $n -f gauss --a0=1 --r0=1 -l 5 ./data/gauss_n${n}_l5.cpl
	$RUN ./progs/create_2d_func/create_2d_func -n $n -f gauss --a0=1 --r0=1 -l 5 -z 1 ./data/gauss_n${n}_l5_z1.cpl
done

# We can visualize data now, if we are at home
if [ "$HOSTNAME" = "oleg-pc" ]
then
	rm -f ./data/*.gif
	./progs/bin2gif/bin2gif -a 1 -t complex -f norm ./data/*.cpl
	./progs/bin2gif/bin2gif -t complex -f real ./data/*.cpl
fi

# Or in "cli" mode
if [ "$1" = "cli" ]
then
	rm -f ./data/*.gif
	./progs/bin2gif/bin2gif -a 1 -t complex -f norm ./data/*.cpl
	./progs/bin2gif/bin2gif -t complex -f real ./data/*.cpl
fi

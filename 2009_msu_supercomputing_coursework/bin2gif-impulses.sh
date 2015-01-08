#!/bin/bash

QUEUE=$1

# Host specific variables
HOSTNAME=`hostname`

if [ "$HOSTNAME" = "ktg1.phys.msu.ru" ] # ILC MSU cluster
then
	RUN=mpiexec
else
	if [ "$HOSTNAME" = "oleg-pc" ] # Oleg Efimov's home PC
	then
		RUN=
	else
		if [ "$HOSTNAME" = "fen1" ] # CS MSU IBM Bluegene/P cluster
		then
			RUN="mpisubmit.bg -n 1"
			SHOWTASKS="llq"
		else # SRCC MSU SKIF cluster
			QUEUETEST=`tasks -q $QUEUE 2>&1 | grep "not exists"`
			
			if [ "$QUEUETEST" != "" ] # Queue exists
			then
				QUEUE="bigmem"
			fi
			
			RUN="mpirun -q $QUEUE -as single -np 1"
			SHOWTASKS="tasks -q $QUEUE"
		fi
	fi
fi

if [ "$1" = "cli" ]
then
	RUN=
	SHOWTASKS=
fi

rm -f ./data/*.gif
$RUN ./progs/bin2gif/bin2gif-static --resize=1024 -t complex -f norm -a 1 ./data/*.cpl
$RUN ./progs/bin2gif/bin2gif-static --resize=1024 -t complex -f real ./data/*.cpl
$SHOWTASK

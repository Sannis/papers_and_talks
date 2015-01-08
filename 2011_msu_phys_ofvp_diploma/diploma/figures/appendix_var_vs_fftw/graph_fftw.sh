#!/bin/bash

np_list_2k="1 2 4 8 16 32 64"
np_list_all="1 2 4 8 16 24 32 48 64"

n_list=(512 1024 2048 4096 8192 16384)
yrange1_list=(0.01  0  0.05  0 1.0  0)
yrange2_list=(0.10  0  3.00  0 100  0)
keypos_list=(b  0  t  0 t  0)

# Create directories for graphs

mkdir -p ./skif/

# Options comparison

echo "FFTW: Making graphs for options comparison"

for ix in {0..5}
do
	n=${n_list[$ix]}
	
	echo "Processing N=$n..."
	

	yrange1=${yrange1_list[$ix]}
	yrange2=${yrange2_list[$ix]}
	keypos=${keypos_list[$ix]}
	
	if [ "$yrange1" = "0" ]
	then
		yrange1="\*"
	fi
	
	if [ "$yrange2" = "0" ]
	then
		yrange2="\*"
	fi
	
	if [ "$keypos" = "b" ]
	then
		keypos="right bottom"
	else
		keypos="right top"
	fi

	for type in 2k all
	do
		cat ./graph_fftw_compare.tpl.gp | \
										sed -e "s/#N#/${n}/" | \
										sed -e "s/#TYPE#/${type}/" | \
										sed -e "s/#YRANGE_1#/${yrange1}/" | \
										sed -e "s/#YRANGE_2#/${yrange2}/" | \
										sed -e "s/#KEYPOS#/${keypos}/" | \
										gnuplot
	done
done

# Time

echo "FFTW: Making graphs for time measures"

for save_steps in -1 10
do
	if [ "$save_steps" = "10" ]
	then
		subdirname="Time_with_save"
		postfix="withsave"
	else
		subdirname="Time_no_save"
		postfix="nosave"
	fi

	echo "Processing ${subdirname}..."
	
	for type in 2k all
	do
		cat ./graph_fftw_time.tpl.gp | sed -e "s/#PFX#/${postfix}/" | sed -e "s/#TYPE#/${type}/" | gnuplot
	done
done

echo "[Finished]"


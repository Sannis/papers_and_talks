#!/bin/bash

for cmd in compare time
do
	echo "Running prepare_results_${cmd}.sh:"
	./prepare_results_$cmd.sh $1
done


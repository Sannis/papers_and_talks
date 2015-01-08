#!/bin/bash

sizes="0.125 0.25 0.5 1 2 4 8 16 32 64 128 256 512 1024"

for size in $sizes
do
    rm -f ./${size}K.bin
    bytes=`echo ${size}*1024 | bc -l | awk -F '.' '{ print $1; exit; }'`
    if [ "$bytes" -lt "1024" ]
    then
        dd if=/dev/urandom of=./${size}K.bin bs=${bytes} count=1
    else
        dd if=/dev/urandom of=./${size}K.bin bs=${size}K count=1
    fi
done


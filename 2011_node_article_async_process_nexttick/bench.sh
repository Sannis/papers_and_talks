#!/bin/bash

ab_c=100
ab_n=1000

AB=ab2

sizes="0.125 0.25 0.5 1 2 4 8 16 32 64 128 256 512 1024"

echo -n "" > rps.dat
echo -n "" > log.txt

for size in $sizes
do
    echo "$size..."
    
    
    echo -n "\"$size\"" >> rps.dat
    
    for script in `ls ./*.js`
    do
        echo "$script"
        ./$script >> /dev/null &
        pid=$!
        echo "PID: $pid"
        
        sleep 10s
        
        output=`$AB -c $ab_c -n $ab_n http://127.0.0.1:8666/${size}K.bin`
        
        rps=`echo "$output" | grep "Requests per second:" | sed "s/.*:\\s*\(.*\)\\s*\[.*/\\1/"`
        echo -n "   $rps" >> rps.dat
        
        echo "$script" >> log.txt
        echo "$output" >> log.txt
        
        kill $pid
    done
    
    echo "" >> rps.dat
    
    ./graph.sh
    
    echo ""
done


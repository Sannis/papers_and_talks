#!/bin/bash

xrange21="30:1000"
yrange21="1e-10:1e-1"

xrange="1e12:1e15"
yrange="1:1e16"

gnuplot <<END
	load "./gnuplot_styles.gp"
	
	set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small
	set output "$1/O2_21fs.eps"
	
	set xlabel "I, 10^{11} Вт/см^2" @font_big
	set xrange [$xrange21]
	set ylabel "R(I)" @font_big
	set yrange [$yrange21]
	set border 3
	set xtics nomirror
	set ytics nomirror
	set key center top
	
	set logscale xy
	set format x "10^{%L}"
	set format y "10^{%L}"
	set mxtics 10
	set mytics 10
	set grid mxtics ytics
	
	set multiplot
	
	plot \
		"$1/O2.dat" using 0:1 title "O_2" with lines linestyle 1
	
	unset multiplot
	
	set output "$1/N2_21fs.eps"
	
	set xlabel "I, 10^{11} Вт/см^2" @font_big
	set xrange [$xrange21]
	set ylabel "R(I)" @font_big
	set yrange [$yrange21]
	set border 3
	set xtics nomirror
	set ytics nomirror
	set key center top
	
	set logscale xy
	set format x "10^{%L}"
	set format y "10^{%L}"
	set mxtics 10
	set mytics 10
	set grid mxtics ytics
	
	set multiplot
	
	plot \
		"$1/N2.dat" using 0:1 title "N_2" with lines linestyle 2
	
	unset multiplot
	
	set output "$1/All_21fs.eps"
	
	set xlabel "I, 10^{11} Вт/см^2" @font_big
	set xrange [$xrange21]
	set ylabel "R(I), (21 фс)^{-1}" @font_big
	set yrange [$yrange21]
	set border 3
	set xtics nomirror
	set ytics nomirror
	set key center top
	
	set logscale xy
	set format x "10^{%L}"
	set format y "10^{%L}"
	set mxtics 10
	set mytics 10
	set grid mxtics ytics
	
	set multiplot
	
	plot \
		"$1/O2.dat" using 0:1 title "O_2" with lines linestyle 1, \
		"$1/N2.dat" using 0:1 title "N_2" with lines linestyle 2
	
	unset multiplot
	
	set output "$1/All.eps"
	
	set xlabel "I, Вт/см^2" offset 0,-0.5 @font_big
	set xrange [$xrange]
	set ylabel "R(I), с^{-1}" offset -2,0 @font_big
	set yrange [$yrange]
	set border 3
	set xtics nomirror
	set ytics nomirror
	set key center top
	
	set logscale xy
	set format x "10^{%L}"
	set format y "10^{%L}"
	set mxtics 10
	set mytics 10
	unset grid
	set grid xtics ytics
	
	set multiplot
	
	plot \
		"$1/O2.dat" using (\$0*10**11):(\$1/(21*10**-15)) title "O_2" with lines linestyle 1, \
		"$1/N2.dat" using (\$0*10**11):(\$1/(21*10**-15)) title "N_2" with lines linestyle 2
	
	unset multiplot
	
	exit
END


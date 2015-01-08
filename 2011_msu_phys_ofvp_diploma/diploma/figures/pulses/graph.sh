#!/bin/bash


log_txt=$1/log.txt

ldiff=12.27
x_min=11.0
x_max=13.0

gnuplot <<END
	load "./gnuplot_styles.gp"
	
	set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small
	set output "$1/intensity_z09.eps"
	
	set xlabel "z, м" @font_big
	set xrange [$x_min:$x_max]
	set ylabel "Интенсивность, отн. ед." @font_big
	set yrange [0:*]
	set border 3
	set xtics nomirror
	set ytics nomirror
	set key center top
	
	set nologscale y
	
	set multiplot
	
	plot \
		"$log_txt" using (\$3*$ldiff):4 title "" with lines linestyle 1
	
	unset multiplot
	
	set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small
	set output "$1/intensity_log_z09.eps"
	
	set xlabel "z, м" @font_big
	set xrange [$x_min:$x_max]
	set ylabel "Интенсивность, отн. ед." @font_big
	set yrange [10:*]
	set border 3
	set xtics nomirror
	set ytics nomirror
	set key center top
	
	set logscale y
	
	set multiplot
	
	plot \
		"$log_txt" using (\$3*$ldiff):4 title "Пиковая интенсивность" with lines linestyle 1
	
	unset multiplot
	
	set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small
	set output "$1/r_fil_z09.eps"
	
	set xlabel "z, м" @font_big
	set xrange [$x_min:$x_max]
	set ylabel "R_{fil}, мкм" @font_big
	set yrange [0:*]
	set border 3
	set xtics nomirror
	set ytics nomirror
	set nokey
	
	set nologscale y
	
	set multiplot
	
	plot \
		"$log_txt" using (\$3*$ldiff):8 with lines linestyle 1
	
	unset multiplot
	
	set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small
	set output "$1/r_pl_z09.eps"
	
	set xlabel "z, м" @font_big
	set xrange [$x_min:$x_max]
	set ylabel "R_{pl}, мкм" @font_big
	set yrange [0:*]
	set border 3
	set xtics nomirror
	set ytics nomirror
	set nokey
	
	set nologscale y
	
	set multiplot
	
	plot \
		"$log_txt" using (\$3*$ldiff):10 with lines linestyle 1
	
	unset multiplot
	
	set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small
	set output "$1/plasma_z09.eps"
	
	set xlabel "z, м" @font_big
	set xrange [$x_min:$x_max]
	set ylabel "Концентрация ионов, отн. ед." @font_big
	set yrange [0:*]
	set border 3
	set xtics nomirror
	set ytics nomirror
	set key right center
	
	set nologscale y
	set format y "%.0l×10^{%L}"
	#set ytics 0, 1e-6, 5e-6
	
	set multiplot
	
	plot \
		"$log_txt" using (\$3*$ldiff):15 title "Полная" with lines linestyle 1, \
		"$log_txt" using (\$3*$ldiff):13 title "O2"     with lines linestyle 2, \
		"$log_txt" using (\$3*$ldiff):14 title "N2"     with lines linestyle 3
	
	unset multiplot
	
	exit
END


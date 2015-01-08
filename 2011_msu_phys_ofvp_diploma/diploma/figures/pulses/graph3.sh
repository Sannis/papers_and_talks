#!/bin/bash

lw_max=5
lw_0=2.5

ldiff=12.27
x_min=11.0
x_max=13.0

gnuplot <<END
	load "./gnuplot_styles.gp"
	
	set size ratio 0.7
	set size 0.8, 0.8
	set bmargin 3.5
	set tmargin 1.5
	set rmargin 1
	set lmargin 8
	
	set style line 1 lc rgbcolor '#ff0000' lt -1 lw 2.5
	set style line 2 lc rgbcolor '#0000ff' lt -1 lw 2.5
	
	set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small
	set output "intensity_cut.eps"
	
	set xlabel "z, м" @font_big
	set xrange [$x_min:$x_max]
	set ylabel "I^{max}, 10^{13} Вт/см^2" @font_big
	set yrange [0:*]
	set border 3
	set xtics nomirror
	set ytics nomirror
	set key right top
	
	set nologscale y
	set format y "%.0f"
	
	set multiplot
	
	plot \
		"$1" using (\$3*$ldiff):(\$4*0.3056/100) title "800 нм" with lines linestyle 2, \
		"$2" using (\$3*$ldiff):(\$4*3.8136/100) title "10 мкм" with lines linestyle 1
	
	unset multiplot
	
	set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small
	set output "plasma_cut.eps"
	
	set xlabel "z, м" @font_big
	set xrange [$x_min:$x_max]
	set ylabel "N_e, отн. ед." offset -1,0 @font_big
	set yrange [*:*]
	set border 3
	set xtics nomirror
	set ytics nomirror
	set key right center
	
	set logscale y
	set format y "10^{%L}"
	
	set multiplot
	
	plot \
		"$1" using (\$3*$ldiff):(\$15) title "800 нм" with lines linestyle 2, \
		"$2" using (\$3*$ldiff):(\$15) title "10 мкм" with lines linestyle 1
	
	unset multiplot
	
	set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small
	set output "r_fil_cut.eps"
	
	set xlabel "z, м" @font_big
	set xrange [$x_min:$x_max]
	set ylabel "R_{fil}, мкм" offset -1,0 @font_big
	set yrange [*:*]
	set border 3
	set xtics nomirror
	set ytics nomirror
	set key right center
	
	set nologscale y
	set format y "%.0f"
	
	set multiplot
	
	plot \
		"$1" using (\$3*$ldiff):(\$8) title "800 нм" with lines linestyle 2, \
		"$2" using (\$3*$ldiff):(\$8) title "10 мкм" with lines linestyle 1
	
	unset multiplot
	
	set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small
	set output "r_pl_cut.eps"
	
	set xlabel "z, м" @font_big
	set xrange [$x_min:$x_max]
	set ylabel "R_{pl}, мкм" offset -1,0 @font_big
	set yrange [*:*]
	set border 3
	set xtics nomirror
	set ytics nomirror
	set key right center
	
	set nologscale y
	set format y "%.0f"
	
	set multiplot
	
	plot \
		"$1" using (\$3*$ldiff):(\$10) title "800 нм" with lines linestyle 2, \
		"$2" using (\$3*$ldiff):(\$10) title "10 мкм" with lines linestyle 1
	
	unset multiplot
	
	exit
END


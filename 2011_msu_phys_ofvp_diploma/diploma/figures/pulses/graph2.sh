#!/bin/bash

y_m="*"
if [ "$4" != "" ]; then
	y_m="$4"
fi

lw_max=5
lw_0=2.5

ldiff=12.27
x_min=11.0
x_max=12.0


gnuplot <<END
	load "./gnuplot_styles.gp"
	
	set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small
	set output "intensity_cut.eps"
	
	set xlabel "z, м" @font_big
	set xrange [$x_min:$x_max]
	set ylabel "Интенсивность, отн. ед." offset -1,0 @font_big
	set yrange [0:$y_m]
	set border 3
	set xtics nomirror
	set ytics nomirror
	set key left top
	
	set multiplot
	
	plot \
		"$1" using (\$3*$ldiff):4 title "  С дисперсией" with lines linestyle 1 lw $lw_max, \
		"$2" using (\$3*$ldiff):4 title "  Без дисперсии" with lines linestyle 2 lw $lw_max
	
	unset multiplot
	
	set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small
	set output "dz.cut.log.eps"
	
	set xlabel "z/L_{diff}" @font_big
	set xrange [$cut:1.0]
	set ylabel "Шаг по оси z" offset -1,0 @font_big
	set yrange [2e-6:2e-3]
	set border 3
	set xtics nomirror
	set ytics nomirror
	set key right
	
	set logscale y
	set format y "10^{%L}"
	
	set multiplot
	
	plot \
		"$1" using 3:2 title "С дисперсией" with lines linestyle 1 lw $lw_max, \
		"$2" using 3:2 title "Без дисперсии" with lines linestyle 2 lw $lw_max
	
	unset multiplot
	
	unset logscale y
	
	exit
END


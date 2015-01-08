load "gnuplot_styles.gp"

set terminal postscript eps enhanced size 8in,5.5in @linewidth @dashlength @font_small

set output "./Skif/FFTW_compare_N#N#_nomeasure_#TYPE#.eps"

set xlabel "Количество процессов" @font_big
set xrange [0:65]
set ylabel "Время, с\n" @font_big
set yrange [#YRANGE_1#:#YRANGE_2#]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key #KEYPOS#

set logscale y 10
set format y "10^{%L}"

set multiplot

plot \
	"../tables/Skif/FFTW_compare_N#N#_#TYPE#.dat" using 1:3 title "Без буфера, TRANSPOSE" with linespoints linestyle 8, \
	"../tables/Skif/FFTW_compare_N#N#_#TYPE#.dat" using 1:2 title "С буфером, TRANSPOSE" with linespoints linestyle 9, \
	"../tables/Skif/FFTW_compare_N#N#_#TYPE#.dat" using 1:5 title "Без буфера, NORMAL" with linespoints linestyle 10, \
	"../tables/Skif/FFTW_compare_N#N#_#TYPE#.dat" using 1:4 title "С буфером, NORMAL" with linespoints linestyle 11

unset multiplot

set nologscale y
unset format


set output "./Skif/FFTW_compare_N#N#_measure_#TYPE#.eps"

set xlabel "Количество процессов" @font_big
set xrange [0:65]
set ylabel "Время, с\n" @font_big
set yrange [#YRANGE_1#:#YRANGE_2#]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key #KEYPOS#

set logscale y 10
set format y "10^{%L}"

set multiplot

plot \
	"../tables/Skif/FFTW_compare_N#N#_#TYPE#.dat" using 1:7 title "Без буфера, TRANSPOSE" with linespoints linestyle 8, \
	"../tables/Skif/FFTW_compare_N#N#_#TYPE#.dat" using 1:6 title "С буфером, TRANSPOSE" with linespoints linestyle 9, \
	"../tables/Skif/FFTW_compare_N#N#_#TYPE#.dat" using 1:9 title "Без буфера, NORMAL" with linespoints linestyle 10, \
	"../tables/Skif/FFTW_compare_N#N#_#TYPE#.dat" using 1:8 title "С буфером, NORMAL" with linespoints linestyle 11

unset multiplot

set nologscale y
unset format


exit


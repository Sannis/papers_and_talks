load "gnuplot_styles.gp"

set terminal postscript eps enhanced size 8in,6in @linewidth @dashlength @font_big

set output "./Bluegene/Sweep_time_#PFX#_all.eps"

set xlabel "Количество процессов" @font_big
set xrange [0:33]
set ylabel "Время, с" @font_big
set yrange [10:10000]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key right top

set logscale y 10
set format y "10^{%L}"

set multiplot

plot \
	"../tables/Bluegene/Sweep_time_#PFX#.dat" using 1:2 title "N=512" with linespoints linestyle 1, \
	"../tables/Bluegene/Sweep_time_#PFX#.dat" using 1:5 title "N=1024" with linespoints linestyle 2, \
	"../tables/Bluegene/Sweep_time_#PFX#.dat" using 1:8 title "N=2048" with linespoints linestyle 4, \
	"../tables/Bluegene/Sweep_time_#PFX#.dat" using 1:11 title "N=4096" with linespoints linestyle 5

unset multiplot

set nologscale y
unset format


set output "./Bluegene/Sweep_acceleration_#PFX#_all.eps"

set xlabel "Количество процессов" @font_big
set xrange [0:33]
set ylabel "Ускорение, разы" @font_big
set yrange [0:33]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key left top

set multiplot

plot \
	x title "Идеал" with lines linestyle 7, \
	"../tables/Bluegene/Sweep_time_#PFX#.dat" using 1:3 title "N=512" with linespoints linestyle 1, \
	"../tables/Bluegene/Sweep_time_#PFX#.dat" using 1:6 title "N=1024" with linespoints linestyle 2, \
	"../tables/Bluegene/Sweep_time_#PFX#.dat" using 1:9 title "N=2048" with linespoints linestyle 4, \
	"../tables/Bluegene/Sweep_time_#PFX#.dat" using 1:12 title "N=4096" with linespoints linestyle 5

unset multiplot


set output "./Bluegene/Sweep_efficiency_#PFX#_all.eps"

set xlabel "Количество процессов" @font_big
set xrange [0:33]
set ylabel "Эффективность, %" @font_big
set yrange [0:100]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key right top

set multiplot

plot \
	"../tables/Bluegene/Sweep_time_#PFX#.dat" using 1:4 title "N=512" with linespoints linestyle 1, \
	"../tables/Bluegene/Sweep_time_#PFX#.dat" using 1:7 title "N=1024" with linespoints linestyle 2, \
	"../tables/Bluegene/Sweep_time_#PFX#.dat" using 1:10 title "N=2048" with linespoints linestyle 4, \
	"../tables/Bluegene/Sweep_time_#PFX#.dat" using 1:13 title "N=4096" with linespoints linestyle 5

unset multiplot


exit


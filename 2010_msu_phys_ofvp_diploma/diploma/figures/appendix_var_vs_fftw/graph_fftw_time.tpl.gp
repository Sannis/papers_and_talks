load "./gnuplot_styles.gp"

set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small

set output "./skif/FFTW_time_#PFX#_#TYPE#.eps"

set xlabel "Количество процессов" @font_big
set xrange [0:65]
set ylabel "Время, с\n" @font_big
set yrange [0.1:200000]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key right top

set logscale y 10
set format y "10^{%L}"

set multiplot

plot \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:2 title "N=512" with linespoints linestyle 1, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:5 title "N=1024" with linespoints linestyle 2, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:8 title "N=2048" with linespoints linestyle 3, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:11 title "N=4096" with linespoints linestyle 4, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:14 title "N=8192" with linespoints linestyle 5, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:17 title "N=16384" with linespoints linestyle 6

unset multiplot

set nologscale y
unset format


set output "./skif/FFTW_acceleration_#PFX#_#TYPE#.eps"

set xlabel "Количество процессов" @font_big
set xrange [0:65]
set ylabel "Ускорение, разы" @font_big
set yrange [0:65]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key left top

set multiplot

plot \
	x title "Идеал" with lines linestyle 7, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:3 title "N=512" with linespoints linestyle 1, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:6 title "N=1024" with linespoints linestyle 2, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:9 title "N=2048" with linespoints linestyle 3, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:12 title "N=4096" with linespoints linestyle 4, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:15 title "N=8192" with linespoints linestyle 5, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:18 title "N=16384" with linespoints linestyle 6
	

unset multiplot


set output "./skif/FFTW_efficiency_#PFX#_#TYPE#.eps"

set xlabel "Количество процессов" @font_big
set xrange [0:65]
set ylabel "Эффективность, %" @font_big
set yrange [0:100]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key right top

set multiplot

plot \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:4 title "N=512" with linespoints linestyle 1, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:7 title "N=1024" with linespoints linestyle 2, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:10 title "N=2048" with linespoints linestyle 3, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:13 title "N=4096" with linespoints linestyle 4, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:16 title "N=8192" with linespoints linestyle 5, \
	"./tables/skif/FFTW_time_#PFX#_#TYPE#.dat" using 1:19 title "N=16384" with linespoints linestyle 6

unset multiplot


exit


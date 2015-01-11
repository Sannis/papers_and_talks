load "./gnuplot_styles.gp"

FIT_LIMIT = 1e-6

set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small

set output "./plasma.eps"

set xlabel "Длина волны, нм" @font_big
set xrange [200:11000]
set ylabel "N_e, 10^{16} см^{-3}" offset 2,0 @font_big
set yrange [*:*]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key top center

set logscale xy

set multiplot

plot \
	"data.txt" using 1:2 with points pt 5 ps 2.5 title "", \
	"data.txt" using 4:5 with points pt 4 ps 2.5 title "", \
	1.23135e+07/x/x with lines linestyle 1 title ""

fit a/x/x "data.txt" using 1:2 via a

unset multiplot

set output "./r_pl.eps"

set xlabel "Длина волны, нм" @font_big
set xrange [200:11000]
set ylabel "r_{pl}, мкм" offset -2,0 @font_big
set yrange [*:*]
set border 3
set xtics nomirror @font_big
set xtics (800, 10000)
set ytics nomirror @font_big
set key top center

set nologscale xy

set multiplot

plot \
	"data.txt" using 1:3 with points pt 5 ps 2.5 title "", \
	"data.txt" using 4:6 with points pt 4 ps 2.5 title "", \
	0.015216*x with lines linestyle 1 title ""

fit c*x "data.txt" using 1:3 via c

unset multiplot

exit


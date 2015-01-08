load "./gnuplot_styles.gp"

set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small

set output "./graph.eps"

set xlabel "z/L_{diff}" @font_big
set xrange [0:1.0]
set ylabel "Энергия импульса, отн. ед." @font_big
set yrange [1.0:1.00003]
set ytics (1.0, 1.00001, 1.00002, 1.00003)
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key top left
set nokey

set multiplot

plot \
	"log.txt" using 1:($2/1.00002530) with lines linestyle 9 title "Энергия импульса"

unset multiplot

exit


load "./gnuplot_styles.gp"

set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small

set output "./graph.eps"

set xlabel "R/R_{cr}" @font_big
set xrange [3:20]
set ylabel "z_{fil}/L_{diff}" @font_big
set yrange [0:6]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key top right

set multiplot

plot \
	"data.txt" using 1:3 with points linestyle 9 title "Вычислительный эксперимент", \
	"data.txt" using 1:2 with lines  linestyle 9 title "Формула Марбургера"

unset multiplot

exit


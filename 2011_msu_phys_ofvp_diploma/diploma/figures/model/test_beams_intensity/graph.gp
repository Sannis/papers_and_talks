load "./gnuplot_styles.gp"

set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small

set output "./graph.eps"

set xlabel "z/L_{diff}" @font_big
set xrange [0:10]
set ylabel "Интенсивность, отн. ед." @font_big
set yrange [0:1.05]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key top right

set multiplot

plot \
	"log.txt" using 1:2 with lines linestyle 12 title "Вычислительный эксперимент", \
	"log.txt" using 1:3 with lines linestyle 8  title "Аналитическая формула"

unset multiplot

exit


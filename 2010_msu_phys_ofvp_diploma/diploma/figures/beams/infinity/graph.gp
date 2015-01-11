load "./gnuplot_styles.gp"

set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small

set output "./graph.eps"

set xlabel "z/L_{diff}" @font_big
set xrange [0:0.13]
set ylabel "Интенсивность, отн. ед." @font_big
set yrange [0:30]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key top center

#set logscale y

set multiplot

plot \
	"outphase.txt" using 3:4 with lines linestyle  8 title "Противофазная модуляция", \
	"inphase.txt"  using 3:4 with lines linestyle 12 title "Синфазная модуляция"

set dummy t
set parametric
set trange [0:2.074486091328]
plot 0.104888, t with lines linestyle 14 title ""
set trange [0:3.424385802520]
plot 0.117279, t with lines linestyle 14 title ""
set trange [0:22.395229998286]
plot 0.126499, t with lines linestyle 14 title ""

unset multiplot

exit


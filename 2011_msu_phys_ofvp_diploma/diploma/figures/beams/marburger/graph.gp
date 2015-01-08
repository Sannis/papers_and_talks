load "./gnuplot_styles.gp"

f(x) = 0.367/sqrt((sqrt(x/a)-0.852)**2 - 0.0219)
FIT_LIMIT = 1e-6

set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small

set output "./graph_inphase.eps"

set xlabel "R" @font_big
set xrange [0:325]
set ylabel "z_{fil}/L_{diff}" @font_big
set yrange [0:0.6]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key top right


fit f(x) "inphase.txt" using 3:4 via a


set multiplot

plot \
	"inphase.txt" using 3:4 with points linestyle  9 title "Вычислительный эксперимент", \
	"inphase.txt" using 1:($2*1.15) with lines linestyle 9 title "Аппроксимация"

unset multiplot


set output "./graph_outphase.eps"

set xlabel "R" @font_big
set xrange [75:325]
set ylabel "z_{fil}/L_{diff}" @font_big
set yrange [0:0.6]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key top right


fit f(x) "outphase.txt" using 3:4 via a


set multiplot

plot \
	"outphase.txt" using 3:4 with points linestyle  9 title "Вычислительный эксперимент", \
	"outphase.txt" using 1:($2*1.1) with lines linestyle 9 title "Аппроксимация"

unset multiplot

exit


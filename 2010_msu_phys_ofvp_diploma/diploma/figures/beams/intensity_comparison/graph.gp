load "./gnuplot_styles.gp"

set terminal postscript eps enhanced size @eps_size @linewidth @dashlength @font_small

set output "./graph_start.eps"

set xlabel "z/L_{diff}" @font_big
set xrange [0:0.15]
set ylabel "Интенсивность, отн. ед." @font_big
set yrange [0:1.6]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key top right

set macros
z_talbot = "0.0311"
x_st = "0.008"
y_st = "0.15"
x_dd = "0.0001"
y_dd = "0.001"
y_d = "0.04"

set object 1 rect from first @x_st, first @y_st to first @x_st+@z_talbot, first @y_st+@y_dd fs solid lw 5
set object 2 rect from first @x_st, first @y_st-@y_d to first @x_st+@x_dd, first @y_st+@y_d fs solid lw 5
set object 3 rect from first @x_st+@z_talbot, first @y_st-@y_d to first @x_st+@z_talbot+@x_dd, first @y_st+@y_d fs solid lw 5

set multiplot

plot \
	"outphase.txt" using 1:2 with lines linestyle 12 title "Противофазная модуляция", \
	"inphase.txt" using 1:2 with lines linestyle 8 title "Синфазная модуляция"

unset multiplot

set output "./graph_all.eps"

set xlabel "z/L_{diff}" @font_big
set xrange [0:0.6]
set ylabel "Интенсивность, отн. ед." @font_big
set yrange [0:40]
set border 3
set xtics nomirror @font_big
set ytics nomirror @font_big
set key top right

set object 1 rect from first 0, first 0 to first 0.15, first 1.6 fs solid lw 5

set multiplot

plot \
	"outphase.txt" using 1:2 with lines linestyle 12 title "Противофазная модуляция", \
	"inphase.txt" using 1:2 with lines linestyle 8 title "Синфазная модуляция"

unset multiplot

exit


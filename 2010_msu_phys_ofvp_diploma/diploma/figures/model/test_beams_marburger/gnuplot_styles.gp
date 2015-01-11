set encoding utf8

# Point types:
#  0 none
#  1 +
#  2 x
#  3 x & +
#  4 square empty
#  5 square filled
#  6 circle empty
#  7 circle filled
#  8 triangle up empty
#  9 triangle up filled
# 10 triangle down empty
# 11 triangle down filled
# 12 romb empty
# 13 romb filled
# 14 penta empty
# 15 penta filled
# 16-19, 20-23, 24-27, 38-31 circle with filled part
# 32-35, 36-39, 40-43, 44-47 square with filled part

# Line types
# -1 solid
#  0 dotted
#  1 solid, linewidth*0.5
#  2 dashed x:1 |----  ----  |
#  3 dashed     |--  --  |
#  4 dashed     |- - - - |
#  5 dashed     |--- - |
#  6 dashed     |--  -  |

set style line 1  lc rgbcolor '#000000' lt  2 lw   5 pt 4 ps 4
set style line 2  lc rgbcolor '#000000' lt  2 lw   5 pt 6 ps 4
set style line 3  lc rgbcolor '#000000' lt  2 lw   5 pt 8 ps 4
set style line 4  lc rgbcolor '#000000' lt -1 lw 2.5 pt 5 ps 4
set style line 5  lc rgbcolor '#000000' lt -1 lw 2.5 pt 7 ps 4
set style line 6  lc rgbcolor '#000000' lt -1 lw 2.5 pt 9 ps 4

set style line 7  lc rgbcolor '#000000' lt  2 lw   5

set style line 8  lc rgbcolor '#000000' lt -1 lw 2.5 pt 5 ps 4
set style line 9  lc rgbcolor '#000000' lt -1 lw 2.5 pt 7 ps 4
set style line 10 lc rgbcolor '#000000' lt  2 lw   5 pt 4 ps 4
set style line 11 lc rgbcolor '#000000' lt  2 lw   5 pt 6 ps 4

set style line 12 lc rgbcolor '#000000' lt  4 lw 10 pt 5 ps 4
set style line 13 lc rgbcolor '#000000' lt  4 lw 10 pt 7 ps 4

set macros
eps_size = "8in,5.4in"
font_big = "font \"DejaVuSans,40\""
font_small = "font \"DejaVuSans,34\""
linewidth = "linewidth 1.5"
dashlength = "dashlength 13"


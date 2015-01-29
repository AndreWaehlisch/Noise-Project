# external variables: maxValue, Temp, Time, numCols

set terminal png

set size square

# convert floating point to string
outputfile = sprintf("./snapshots/%g/snapshot%g.png", Temp, Time)

set output outputfile
set title sprintf("Temp=%g", Temp)
set xlabel 'X'
set ylabel 'Y'
unset key

#set xrange [0:maxValue]
#set yrange [0:maxValue]

#set format x ""
#set format y ""

unset xtics
unset ytics

# plot filled circles in same color
plot for[i=1:numCols:4] 'plot.tmp' u (column(i)):(column(i+1)):((column(i+2))*0.01):((column(i+3))*0.01) w vec lc 1 lw 1

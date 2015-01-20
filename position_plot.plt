# external variables: maxValue, Temp, Time, numCols

set terminal png

# convert floating point to string
outputfile = sprintf("./snapshots/%g/snapshot%g.png", Temp, Time)

set output outputfile
set title sprintf("Temp=%g", Temp)
set xlabel 'X'
set ylabel 'Y'
unset key

minValue = -maxValue

set xrange [0:maxValue]
set yrange [0:maxValue]

# plot filled circles in same color
plot for[i=1:numCols:2] 'plot.tmp' u (column(i)):(column(i+1)) lc 1 pt 7

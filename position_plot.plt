# external variables: maxValue, Temp, Time, numCols

set terminal png size 1000,1000

if (maxValue > 0) {
	set xrange [0:maxValue]
	set yrange [0:maxValue]
	outputfile = sprintf("./snapshots/%g/fixed/snapshot%g.png", Temp, Time)
} else {
	outputfile = sprintf("./snapshots/%g/moving/snapshot%g.png", Temp, Time)
}

set output outputfile
set title sprintf("D=%g", Temp) font ",50"
set xlabel 'X' offset 0,-2 font ",50"
set ylabel 'Y' font ",50"
unset key

set tmargin 6

unset xtics
unset ytics

set size square

if (maxValue > 0) {
	plot for[i=1:numCols:4] 'plot.tmp' u (column(i)):(column(i+1)) lc 1 pt 7 ps 8
} else {
	plot for[i=1:numCols:4] 'plot.tmp' u (column(i)):(column(i+1)):((column(i+2))*0.01):((column(i+3))*0.01) w vec lc 1 lw 3
}

# external variables Temp

outputfile = sprintf("./results/%g/AngularMomentum.png", Temp)
inputfile = sprintf("output/momentum_%g.dat", Temp)
mytitle = sprintf("D=%g", Temp)

n=40

#stats 'output/momentum.dat' u 1 nooutput
#if ( STATS_max > abs(STATS_min) ) max=STATS_max*1.5; else max=abs(STATS_min)*1.5

max=0.25
min=-max
width=(max-min)/n
hist(x,width)=width*floor(x/width)+width/2.0

set terminal png
set output outputfile

set xrange[min:max]

set xlabel 'L'
set ylabel 'N(L)' #'P(|L|)'
set title mytitle
unset key

plot inputfile u (hist($1,width)):(1.0) smooth freq w boxes

#http://gnuplot-surprising.blogspot.de/2011/09/statistic-analysis-and-histogram.html

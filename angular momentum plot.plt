stats 'output/momentum.dat' u 1

n=100
#if ( STATS_max > abs(STATS_min) ) max=STATS_max*1.5; else max=abs(STATS_min)*1.5
max=1.3
min=-max
width=(max-min)/n
hist(x,width)=width*floor(x/width)+width/2.0


set terminal png
set output 'output/AngularMomentum.png'

set xrange[min:max]

set xlabel '|L|'
set ylabel 'N' #'P(|L|)'
set title 'angular momentum'
unset key

plot "output/momentumLUA.dat" u (hist($1,width)):(1.0) smooth freq w boxes

#http://gnuplot-surprising.blogspot.de/2011/09/statistic-analysis-and-histogram.html

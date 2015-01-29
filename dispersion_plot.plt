set terminal png
set output 'results/dispersion.png'

set size square

set xlabel 'D'
set ylabel 'dispersion'

plot 'output/dispersion/senkrechtSorted.dat' w l, 'output/dispersion/parallelSorted.dat' w l

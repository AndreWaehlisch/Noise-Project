!sort -n ./output/dispersion/senkrecht.dat > ./output/dispersion/senkrechtSorted.dat
!sort -n ./output/dispersion/parallel.dat > ./output/dispersion/parallelSorted.dat

set terminal png
set output 'output/dispersion.png'

set xlabel 'D'
set ylabel 'dispersion'

plot 'output/dispersion/senkrechtSorted.dat' w l, 'output/dispersion/parallelSorted.dat' w l

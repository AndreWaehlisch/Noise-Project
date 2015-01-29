set terminal png
set xlabel 'D'

set size square

set ylabel 'Varianz'
set output './results/variance.png'
plot './output/variance_sorted.dat' w l


set ylabel 'Schwerpunktsabstand'
set output './results/dR_mean.png'
plot './output/dR_mean_sorted.dat' w l

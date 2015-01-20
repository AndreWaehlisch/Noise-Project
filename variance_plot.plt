set terminal png

set xlabel 'D'

set ylabel 'Varianz'
set output './results/variance.png'
plot './output/variance.dat' w l


set ylabel 'Schwerpunktsabstand'
set output './results/dR_mean.png'
plot './output/dR_mean.dat' w l

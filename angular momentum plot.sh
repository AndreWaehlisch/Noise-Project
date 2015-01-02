gnuplot './angular momentum plot.plt'
avconv -y -framerate 10 -i './snapshots/snapshot%d0.png' -pix_fmt yuv420p ./snapshots/out.mp4

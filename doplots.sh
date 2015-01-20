#!/bin/bash

doCompile=false
doNewRun=false
positionsMovies=true
dispersion=true
angularmomentumPlots=true

mkdir -p ./results/
mkdir -p ./output/dispersion/
mkdir -p ./snapshots/

# compile
if [ "$doCompile" = true ]
then
	g++ -Wall -Wextra -O3 f_calc.cpp f_lua.cpp f_measure.cpp f_metrik.cpp f_integrate.cpp main.cpp f_forces.cpp -lm -lgsl -lgslcblas -llua5.2 -fopenmp -std=c++11
fi

# generate new output
if [ "$doNewRun" = true ]
then
	./a.out
fi

#####################################
#### Position pictures and movies
#####################################

if [ "$positionsMovies" = true ]
then
	#get number of columns (just use first position.dat here, they are all the same)
	numCols="$(awk '{print NF; exit}' ./output/position_0.dat)"

	for positionFile in ./output/position_*.dat
	do
		#extract temperatures from file names
		echo $positionFile | egrep -o "[0-9]+(\.[0-9]+)?" >> "Temp.tmp"
	done

	while read Temp
	do
		Time=0

		#get highest number for current temperature
		maxValue=$(LC_ALL=C cat "./output/position_${Temp}.dat" | sed 's!\s!\n!g' | sort -rg | head -1)

		#create sub-dir for Temp
		mkdir -p "./snapshots/${Temp}/"

		while read PositionLine
		do
			Time=$((${Time}+1))

			#get the current positions data and write it to plot.tmp
			echo "${PositionLine}" > plot.tmp

			#plot that data
			gnuplot -e "maxValue=${maxValue}; Temp=${Temp}; Time=${Time}; numCols=${numCols}" './position_plot.plt'
		done < "./output/position_${Temp}.dat"

		mkdir -p "./results/${Temp}/"

		avconv -y -framerate 25 -i "./snapshots/${Temp}/snapshot%d.png" -pix_fmt yuv420p "./results/${Temp}/${Temp}.mp4"

		echo "Temp done: ${Temp}"

	done < "Temp.tmp"

	rm *.tmp

	echo "Position plots done."

fi

#####################################
#### Dispersion Plots
#####################################

if [ "$dispersion" = true ]
then

	sort -n ./output/dispersion/senkrecht.dat > ./output/dispersion/senkrechtSorted.dat
	sort -n ./output/dispersion/parallel.dat > ./output/dispersion/parallelSorted.dat
	gnuplot dispersion_plot.plt

	echo "Dispersion plots done."
fi

#####################################
#### Angular momentum plots
#####################################

if [ "$angularmomentumPlots" = true ]
then
	for momentumFile in ./output/momentum_*.dat
	do
		#extract temperatures from file names
		echo $momentumFile | egrep -o "[0-9]+(\.[0-9]+)?" >> "Temp.tmp"
	done

	while read Temp
	do
		mkdir -p "./results/${Temp}/"
		gnuplot -e "Temp=${Temp}" angular_momentum_plot.plt
	done < "Temp.tmp"

	rm *.tmp

	echo "Angular momentum plots done."
fi

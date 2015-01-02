#!/usr/bin/env python
import sys
from math import *
from numpy import *
import matplotlib.pyplot as p
import os

############ run or evaluate ###################
run=1 ## 1 heisst simulation starten

movie=1 ## snapshots machen?

############		COMPILE	   AND 	RUN	PROGRAM		#################
if run==1:
	os.system("make")
	os.system("./a.out")

####################		MAKING	MOVIE	 		#################
if movie==1:
	os.system("mkdir snapshots")

	# Read in data from an ASCII data table
	data = genfromtxt('./output/position.dat')
	frames = data[:,0]
	particlenumber= data[1,:]

	finished = False
	L_min_x = 5.0
	L_max_x = 6.0
	L_min_y = 5.0
	L_max_y = 6.0

	f=0
	while not finished:
		f=f+10
		i=1
		fin = False
		grow = 0

		while not fin:
			p.axes().set_aspect('equal')
			s='snapshots/snapshot%g.png' %f
			j=i+1
			x=data[f,i]
			y=data[f,j]

			if L_min_x > x:
				L_min_x = L_min_x - 2

			if L_min_y > y:
				L_min_y = L_min_y - 2

			if L_max_x < x:
				L_max_x = L_max_x + 2

			if L_max_y < y:
				L_max_y = L_max_y + 2

			p.plot(x,y,marker='o',color='b',markersize=2 )
			i=j+1
			if j == len(particlenumber)-1:
				fin=True
			else:
				fin=False
		p.grid(True)
		p.xlim([L_min_x,L_max_x])
		p.ylim([L_min_y,L_max_y])
		p.axes().set_aspect('equal')
		p.xlabel('X')
		p.ylabel('Y')
		p.grid(True)
		p.savefig(s)
		p.clf() #clearing the figure. kein additives plotten!

		if f == len(frames)-1:
			finished=True
		else:
			finished=False




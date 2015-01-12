#include <iostream>
#include "head.h"
#include <vector>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <omp.h>
#include <ctime>
#include <complex>
#include <string.h>

using namespace std;
extern int i,j,colMax,h,seed;
extern double lx,ly,volume,dpm;

void distance(particle &a,particle &b, double &dist, double &dx, double &dy)
{
	dx=(a.X)-(b.X);
	dy=(a.Y)-(b.Y);
	dist=sqrt((dx*dx)+(dy*dy));
}

void initial(particle col[])
{
	double x,y;
	for(i=0; i<colMax; i++)
	{
		x= 0;//rand() % 10;	//Zufallspositionenen
		y= 0;//rand() % 10;
		col[i].setposition(x,y);
		col[i].VX=1.0;	//Einheitsgeschwindigkeiten
		col[i].VY=1.0;
	};
}

void particle::setposition(double &x, double &y)
{
	X=x;
	Y=y;
}


void particle::getposition(double &x, double &y)
{
	x=X;
	y=Y;
}

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
extern double dist,dx,dy;
extern int i,j,colMax;

void calc_forces(particle a[], double fx[], double fy[])
{	
	double dfx=0.0; //dummy
	double dfy=0.0;
	double fscale=100.0; //federkonstante
	for(i=0; i<(colMax-1); i++)
	{		
		for(j=i+1; j<colMax; j++)
		{	
			// distances between particles i and j
			distance(a[i],a[j],dist,dx,dy); 	
			
			dfx=-fscale/colMax*dx;	
			dfy=-fscale/colMax*dy;


			//ACTIO==REACTIO	
			fx[i] += dfx;
			fy[i] += dfy;
			fx[j] += -dfx;
			fy[j] += -dfy;
		};
	};
}

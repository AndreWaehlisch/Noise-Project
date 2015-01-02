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
extern int i,colMax;
extern double h,sqvarianz,varianz,friction;


//Integrationsroutine inkl self propulsion
void integrate(particle a[],double fx[], double fy[], int &seed)
{
	for(i=0; i<colMax; i++)
	{		
		fx[i]=(fx[i]+(1.0-(a[i].VX*a[i].VX+a[i].VY*a[i].VY))*a[i].VX)*h/friction+sqvarianz*gauss(1.0,seed);seed++;
		fy[i]=(fy[i]+(1.0-(a[i].VX*a[i].VX+a[i].VY*a[i].VY))*a[i].VY)*h/friction+sqvarianz*gauss(1.0,seed);seed++;

		a[i].VX=a[i].VX+fx[i];
		a[i].VY=a[i].VY+fy[i];	
		
		a[i].X=a[i].X+a[i].VX*h;
		a[i].Y=a[i].Y+a[i].VY*h;

		fx[i]=0.0; //reset forces
		fy[i]=0.0;
	};
}


//Gaussche Zufallszahlen
double gauss(double varianz, int &seed)
{
	gsl_rng *r;
	r = gsl_rng_alloc(gsl_rng_mt19937);
	seed++;
	gsl_rng_set(r,seed);
	double gn=gsl_ran_gaussian(r,varianz);
	gsl_rng_free (r);
	return(gn);
};


#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <omp.h>
#include <fstream>
#include "head.h"

using namespace std;

extern int colMax;
extern double h, friction;

//Gaussche Zufallszahlen (Varianz = 1)
double gauss(gsl_rng *myRNG)
{
	return gsl_ran_gaussian(myRNG, 1.0);
}

//Integrationsroutine inkl self propulsion
void integrate(particle a[], double fx[], double fy[], const double sqvarianz, gsl_rng *myRNG)
{
	for(int i=0; i<colMax; i++)
	{
		fx[i]=(fx[i]+(1.0-(a[i].VX*a[i].VX+a[i].VY*a[i].VY))*a[i].VX)*h/friction+sqvarianz*gauss(myRNG);
		fy[i]=(fy[i]+(1.0-(a[i].VX*a[i].VX+a[i].VY*a[i].VY))*a[i].VY)*h/friction+sqvarianz*gauss(myRNG);

		a[i].VX=a[i].VX+fx[i];
		a[i].VY=a[i].VY+fy[i];

		a[i].X=a[i].X+a[i].VX*h;
		a[i].Y=a[i].Y+a[i].VY*h;

		fx[i]=0.0; //reset forces
		fy[i]=0.0;
	}
}

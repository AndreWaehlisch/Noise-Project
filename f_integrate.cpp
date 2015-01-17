#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <omp.h>
#include "head.h"

using namespace std;

extern int colMax;
extern double h, friction;
extern double sqvarianz;
#pragma omp threadprivate(sqvarianz)
extern gsl_rng *myRNG;

	//#pragma omp parallel private(temp, D, varianz)
	////{
		//#ifdef _OPENMP
			//#pragma omp single
				//cout << "Num Threads: " << omp_get_num_threads() << endl;
		//#endif

		//#pragma omp for schedule(auto)

//Integrationsroutine inkl self propulsion
void integrate(particle a[], double fx[], double fy[])
{
	for(int i=0; i<colMax; i++)
	{
		fx[i]=(fx[i]+(1.0-(a[i].VX*a[i].VX+a[i].VY*a[i].VY))*a[i].VX)*h/friction+sqvarianz*gauss();
		fy[i]=(fy[i]+(1.0-(a[i].VX*a[i].VX+a[i].VY*a[i].VY))*a[i].VY)*h/friction+sqvarianz*gauss();

		a[i].VX=a[i].VX+fx[i];
		a[i].VY=a[i].VY+fy[i];

		a[i].X=a[i].X+a[i].VX*h;
		a[i].Y=a[i].Y+a[i].VY*h;

		fx[i]=0.0; //reset forces
		fy[i]=0.0;
	};
}

//Gaussche Zufallszahlen (Varianz = 1)
double gauss()
{
	return gsl_ran_gaussian(myRNG, 1.0);
};

#include "head.h"

using namespace std;

extern int colMax;

const double fscale = 100.0; //federkonstante

void calc_forces(particle a[], double fx[], double fy[], double &dx, double &dy)
{
	double dfx = 0, dfy = 0; //dummy

	for(int i=0; i < (colMax-1); i++)
	{
		for(int j=i+1; j<colMax; j++)
		{
			// distances between particles i and j
			distance(a[i],a[j],dx,dy);

			dfx = -fscale/colMax*dx;
			dfy = -fscale/colMax*dy;

			//ACTIO==REACTIO
			fx[i] += dfx;
			fy[i] += dfy;
			fx[j] += -dfx;
			fy[j] += -dfy;
		};
	};
}

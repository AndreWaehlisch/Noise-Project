#include <cstdlib>
#include <cmath>
#include "head.h"

using namespace std;

extern int colMax;

void distance(particle &a,particle &b, double &dx, double &dy)
{
	dx=(a.X)-(b.X);
	dy=(a.Y)-(b.Y);
}

void initial(particle col[])
{
	double x,y;
	for(int i=0; i<colMax; i++)
	{
		x= rand() % 10;	//Zufallspositionenen
		y= rand() % 10;
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

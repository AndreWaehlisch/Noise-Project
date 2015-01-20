#include <cstdlib>
#include <cmath>
#include "head.h"

using namespace std;

extern int colMax;
extern double rndInit;

void distance(particle &a,particle &b, double &dx, double &dy)
{
	dx = (a.X)-(b.X);
	dy = (a.Y)-(b.Y);
}

void initial(particle col[])
{
	for(int i=0; i<colMax; i++)
	{
		if ( rndInit )
		{
			const double x = rand()%10, y = rand()%10;
			col[i].setposition(x, y);
		} else
			col[i].setposition(0, 0);

		col[i].VX = 1.0;	//Einheitsgeschwindigkeiten
		col[i].VY = 1.0;
	};
}

void particle::setposition(const double x, const double y)
{
	X=x;
	Y=y;
}

void particle::getposition(double &x, double &y)
{
	x=X;
	y=Y;
}

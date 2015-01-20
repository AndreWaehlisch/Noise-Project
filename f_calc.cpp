#include "head.h"
#include <omp.h>

using namespace std;

extern int colMax;

void calcDispersionAndAngMomentum(particle col[], double& Sp_output, double& Ss_output, double L_output[])
{
	// Massen-Schwerpunkt R und mean-velocity V zum aktuellen Zeitpunkt
	double Rx=0, Ry=0, Vx=0, Vy=0;

	for(int i=0; i<colMax; i++)
	{
		Rx += col[i].X;
		Ry += col[i].Y;
		Vx += col[i].VX;
		Vy += col[i].VY;
	}
	Rx /= colMax;
	Ry /= colMax;
	Vx /= colMax;
	Vy /= colMax;

	// Parallele, senkrechte Dispersion und Drehimpuls berechnen
	double Sp=0, Ss=0;

	#pragma omp parallel shared(Sp, Ss)
	{
		double S_i = 0;

		#pragma omp for schedule(auto)
		for(int i=0; i<colMax; i++)
		{
			// parallele Dispersion
			S_i = (col[i].X-Rx)*Vx + (col[i].Y-Ry)*Vy;
			S_i *= S_i;
			#pragma omp critical(Sp)
				Sp += S_i;

			// senkrechte Dispersion
			S_i = (col[i].X-Rx)*Vy - (col[i].Y-Ry)*Vx;
			S_i *= S_i;
			#pragma omp critical(Ss)
				Ss += S_i;

			// Drehimpuls (output wird einfach aufaddiert)
			L_output[i] += (col[i].X - Rx)*(col[i].VY - Vy) - (col[i].Y - Ry)*(col[i].VX - Vx);
		}
	}

	// Normierung der Dispersion
	Sp /= ( colMax*(Vx*Vx+Vy*Vy) );
	Ss /= ( colMax*(Vx*Vx+Vy*Vy) );

	// Output wird einfach aufaddiert
	Sp_output += Sp;
	Ss_output += Ss;
}

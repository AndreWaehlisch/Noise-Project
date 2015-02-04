#include "head.h"
#include <omp.h>
#include <cmath>

using namespace std;

extern int colMax;

double CorrectedVariance(const double dR_mean, double dR_i[])
{
	double partA=0, partB=0;

	for(int i=0; i<colMax; i++)
	{
		partA += ( (dR_i[i]-dR_mean)*(dR_i[i]-dR_mean) );
		partB += ( dR_i[i] - dR_mean );
	}

	partB *= partB/colMax;

	return (partA - partB) / (colMax-1);
}

void calcDispersionAndAngMomentum(particle col[], double& Sp_output, double& Ss_output, double& dR_output, double& Var_output, double& V_output, ofstream& momentum_file)
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

	#pragma omp critical(V_output)
		V_output += sqrt(Vx*Vx + Vy*Vy);

	// Parallele, senkrechte Dispersion, Mittlerer Abstand zum Schwerpunkt und Varianz davon
	double Sp=0, Ss=0, dRx, dRy, dR=0, Var=0;
	double dR_i[colMax];

	#pragma omp parallel shared(Sp, Ss, dR, dR_i) private(dRx, dRy)
	{
		double S_i = 0;

		#pragma omp for schedule(auto)
		for(int i=0; i<colMax; i++)
		{
			// Abstände zum Schwerpunkt
			dRx = ( col[i].X - Rx);
			dRy = ( col[i].Y - Ry);
			dR_i[i] = sqrt( dRx*dRx + dRy*dRy );
			#pragma omp critical(dR)
				dR += dR_i[i];

			// parallele Dispersion
			S_i = dRx*Vx + dRy*Vy;
			S_i *= S_i;
			#pragma omp critical(Sp)
				Sp += S_i;

			// senkrechte Dispersion
			S_i = dRx*Vy - dRy*Vx;
			S_i *= S_i;
			#pragma omp critical(Ss)
				Ss += S_i;

			// Drehimpuls
			#pragma omp critical(momentum_file)
				momentum_file << dRx*(col[i].VY - Vy) - dRy*(col[i].VX - Vx) << endl;
		}
	}

	// Normierung
	Sp /= ( colMax*(Vx*Vx+Vy*Vy) );
	Ss /= ( colMax*(Vx*Vx+Vy*Vy) );
	dR /= colMax;

	// Varianz berechnen
	Var = CorrectedVariance(dR, dR_i);

	// Output wird einfach aufaddiert
	Sp_output += Sp;
	Ss_output += Ss;
	dR_output += dR;
	Var_output += Var;
}

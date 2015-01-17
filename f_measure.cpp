#include <iostream>
#include <cstdio>
#include "head.h"

using namespace std;

//integration parameter
extern int Time;
extern double h;

//Simulationbox
extern int colMax;
extern FILE *pos_file;

//control of printing events
int qt=100;
double part=0.0;

void printpos(particle a[], const int t)
{
	if ( t>=Time*part && t%qt==0 )
	{
		for(int i=0; i<colMax; i++)
		{
			if ( !i )
				fprintf(pos_file,"%lf",t*h);

			fprintf(pos_file,"	%lf	%lf",a[i].X,a[i].Y);
		};

		fprintf(pos_file,"\n");
	}
}

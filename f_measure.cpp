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

/**************************************************************************************
*************************************************************************************** 
 Variablen
*************************************************************************************** 
***************************************************************************************/

extern int i,j,l,k,n,t;
extern double PI;
extern double PI2;
//integration parameter
extern int Time;
extern double h;
extern double sh;

//Simulationbox
extern int colMax;
extern FILE *pos_file;

//control of printing events
int qt=100;
double part=0.0;

void printpos(particle a[])
{
	for(i=0; i<colMax; i++)
	{
		
		if(t>=Time*part && t%qt==0 && i==0){fprintf(pos_file,"%lf",t*h);};
		if(t>=Time*part && t%qt==0)
		{
			fprintf(pos_file,"	%lf	%lf",a[i].X,a[i].Y);
			if(i==0)
			{
				cout << "-------------" << endl;
				cout << t*h << "...von..." << Time*h << endl;
			};
		};
		if(t>=Time*part && t%qt==0 && i==colMax-1){fprintf(pos_file,"\n");};
	};
}









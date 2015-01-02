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

#include <lua5.2/lua.hpp>


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
extern FILE *momentum_file;

//control of printing events
int qt=100;
double part=0.0;

//Lua state
extern lua_State *Lua;

void printpos(particle a[], double L[])
{
	if ( t>=Time*part && t%qt==0 )
	{

		lua_checkstack(Lua, colMax); // ensure enough space in lua stack

		for(i=0; i<colMax; i++)
		{
			if ( !i )
				fprintf(pos_file,"%lf",t*h);

			fprintf(pos_file,"	%lf	%lf",a[i].X,a[i].Y);
			fprintf(momentum_file,"%lf\n",L[i]);
			lua_pushnumber(Lua, L[i]);

			if ( !i )
				cout << t*h << "...von..." << Time*h << endl;
		};

		fprintf(pos_file,"\n");
	}
}

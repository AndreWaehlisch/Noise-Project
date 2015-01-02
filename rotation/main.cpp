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

//main buffer variablen
int i,j,l,k,n;
time_t tstart,tend,tseed;
double PI=M_PI;
double PI2=PI*0.5;
//integration parameter
int t=0;
int Time=100000;	//Anzahl Integrationsschritte
double h=0.01;	//Integrationschrittweite
double sh=sqrt(h);

//Simulationbox
int colMax=100; //Anzahl Teilchen
double density=0.5;
double friction=1.0;
double volume=colMax/density;
double lx=sqrt(volume)/2.0;
double ly=lx;
double temp=0.07;
double D=temp/friction;
double varianz=2.0*D*h;
double sqvarianz=sqrt(varianz);
int seed=1;
double fcc=(lx)/(sqrt((colMax)/2.0));

//Fileschreiberei
FILE *pos_file;
FILE *momentum_file;
const char *LUAFILENAME = "angularMomentum.lua";

//distance
double dx,dy;//Komponenten aus distance
double dist;//Abstandsbetrag aus distance

int main()
{
	//Systemzeit ist seed fuer den Zufallsgenerator
	time(&tstart);
	seed=int(tstart*h);

	//LUA INIT
	lua_State *Lua = luaL_newstate();
	luaL_openlibs(Lua);
	if ( luaL_loadfile(Lua, LUAFILENAME) )
		LUAerror(Lua, "Cannot load config: %s\n", lua_tostring(Lua, -1));
	lua_pushnumber(Lua, colMax); // put number of particles on top of stack

	//open files
	pos_file=fopen("output/position.dat","w");
	momentum_file=fopen("output/momentum.dat","w");

	//print simulation parameter in bash
	//cout << "--------------------------------------------------------------------------" << endl;
	//cout << "--------------------------------------------------------------------------" << endl;
	//cout << "		density	" << density << endl;
	//cout << "		Temperature	" << temp << endl;
	//cout << "		Number of Particles	" << colMax << endl;
	//cout << "		Number of Integrationsteps	" << Time << endl;
	//cout << "		Stepwidth	" << h << endl;
	//cout << "		System Size	" << 2.0*lx << endl;
	//cout << "		minimum distance (diagonal)	" << sqrt(2.0)*fcc << endl;
	//cout << "		friction	" << friction << endl;
	//cout << "		seed	" << seed << "	start	" << tstart << endl;
	//cout << "--------------------------------------------------------------------------" << endl;
	//cout << "--------------------------------------------------------------------------" << endl;

	//Definition von Arrays fuer die Krafte auf jedes teilchen
	double fx[colMax];
	double fy[colMax];
	for(i=0; i<colMax; i++)
	{
		fx[i]=0.0;
		fy[i]=0.0;
	};

	//Definition von Arrays fuer den Drehimpuls jedes teilchens
	double L[colMax];
	for(i=0; i<colMax; i++)
	{
		L[i]=0.0;
	};

	//Initialize particles
	vector<particle> col(colMax);	 //initialisiere die particle
	initial(&col[0]);		//Gib den Teilchen eine Intitial Position

	//Start Simulation
	for(t=0;t<=Time;t++)  		//Schleife der Integrationsschritte
	{
		//calculate forces
		calc_forces(&col[0],fx,fy);

		//apply forces and move particles including stochastic displacement
		integrate(&col[0],fx,fy,seed,L);

		//measurements
		printpos(&col[0], L, Lua);
	};

	// Calc angular momentum stuff in LUA
	LUACalcAngularMomentum(Lua, "angularMomentum.lua");

	// CLOSE STUFF
	fclose(momentum_file);
	fclose(pos_file); //schliesse files
	time(&tend);
	lua_close(Lua); // LUA schließen

	//print simulation parameter in bash
	//cout << "--------------------------------------------------------------------------" << endl;
	//cout << "--------------------------------------------------------------------------" << endl;
	//cout << "		density	" << density << endl;
	//cout << "		Temperature	" << temp << endl;
	//cout << "		Number of Particles	" << colMax << endl;
	//cout << "		Number of Integrationsteps	" << Time << endl;
	//cout << "		Stepwidth	" << h << endl;
	//cout << "		System Size	" << lx << endl;
	//cout << "		minimum distance (diagonal)	" << sqrt(2.0)*fcc << endl;
	//cout << "		friction	" << friction << endl;
	//cout << "		duration	" << tend-tstart << endl;
	//cout << "--------------------------------------------------------------------------" << endl;
	//cout << "--------------------------------------------------------------------------" << endl;

	return(0);
};

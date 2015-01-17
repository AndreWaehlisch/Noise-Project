#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <ctime>

#include <lua5.2/lua.hpp>
#include "head.h"
#include <omp.h>

using namespace std;

// Variablen (Setup in config.lua)
int t=0;
int Time;	//Anzahl Integrationsschritte
double h;	//Integrationschrittweite
int colMax; //Anzahl Teilchen
double density = 0.5;
double friction = 1.0;
double volume, lx, ly, fcc;
double dx,dy;//Komponenten aus distance

int temp_N, Mittelung, angularMomentum_DO, dispersion_DO;
double temp_start, temp_d, temp, D, varianz, initPos;
double sqvarianz;
#pragma omp threadprivate(sqvarianz)

//RNG
int seed=1;
gsl_rng *myRNG;

//Fileschreiberei
FILE *pos_file, *momentum_file, *dispersion_parallel_file, *dispersion_senkrecht_file;
const char *LUAFILENAME = "config.lua";

//LUA instance
lua_State *Lua = luaL_newstate();

int main()
{
	// Setup OpenMP
	#ifdef _OPENMP
		omp_set_num_threads( omp_get_num_procs() );
		cout << "OMP LOADED" << endl;
	#endif

	//Systemzeit ist seed fuer den Zufallsgenerator
	time_t tstart,tend,tseed;
	time(&tstart);
	seed=int(tstart*h);
	myRNG = gsl_rng_alloc(gsl_rng_mt19937);
	gsl_rng_set(myRNG,seed);

	//LUA INIT
	luaL_openlibs(Lua); // open libs in lua
	if ( luaL_loadfile(Lua, LUAFILENAME) ) // try to load the script (and check for syntax errors)
		LUAerror("Cannot load config: %s\n", lua_tostring(Lua, -1));

	LUAExecute(LUAFILENAME); // execute the script

	// LOAD THE CONFIGS FROM LUA
	lua_getglobal(Lua, "config"); // put table 'config' on stack

	if (!lua_istable(Lua, -1))
		LUAerror("'config' is not a table.\n");

	colMax			=	LUAGetConfigValue("colMax");
	Time			= 	LUAGetConfigValue("Time"); //Anzahl Integrationsschritte
	h				= 	LUAGetConfigValue("h"); //Integrationschrittweite
	Mittelung		= 	LUAGetConfigValue("Mittelung");
	initPos			= 	LUAGetConfigValue("initPos");
	temp_N			= 	LUAGetConfigValue("temp_N");
	temp_start		= 	LUAGetConfigValue("temp_start");
	temp_d			= 	LUAGetConfigValue("temp_d");
	angularMomentum_DO	= 	LUAGetConfigValue("angularMomentum_DO");
	dispersion_DO	= 	LUAGetConfigValue("dispersion_DO");

	// Update Variables with config input
	volume = colMax/density;
	lx = sqrt(volume)/2.0;
	ly = lx;
	fcc = (lx)/(sqrt((colMax)/2.0));

	//open output files
	pos_file = fopen("output/position.dat","w");
	momentum_file = fopen("output/momentum.dat","w");
	dispersion_senkrecht_file = fopen("output/dispersion/senkrecht.dat","w");
	dispersion_parallel_file = fopen("output/dispersion/parallel.dat","w");

	// Table for all temperatures we want
	double temps[temp_N];

	for(int i=0; i<temp_N; i++)
	{
		temps[i] = temp_start + (i*temp_d);
	}

	//Definition von Arrays fuer den Drehimpuls jedes teilchens
	double L[colMax];
	for(int i=0; i<colMax; i++)
	{
		L[i] = 0.0;
	};

	for(int T=0; T<N_temp; T++)
	{
		temp = temps[T];
		D=temp/friction;
		varianz=2.0*D*h;
		sqvarianz=sqrt(varianz);

		//Definition von Arrays fuer die Krafte auf jedes teilchen
		double fx[colMax];
		double fy[colMax];
		for(int i=0; i<colMax; i++)
		{
			fx[i]=0.0;
			fy[i]=0.0;
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
			integrate(&col[0],fx,fy);

			//measurements (write data to file)
			printpos(&col[0],t);
		};

		// Massen-Schwerpunkt R und mean-velocity V zum letzten Zeitpunkt
		double Rx=0, Ry=0, Vx=0, Vy=0;
		for (int i=0; i<colMax; i++)
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

		// DISPERSION
		double Sp=0, Ss=0, S_i=0;
		for(int i=0; i<colMax; i++)
		{
			S_i = (col[i].X-Rx)*Vx + (col[i].Y-Ry)*Vy;
			S_i *= S_i;
			Sp += S_i;

			S_i = (col[i].X-Rx)*Vy - (col[i].Y-Ry)*Vx;
			S_i *= S_i;
			Ss += S_i;
		}

		Sp /= ( colMax*(Vx*Vx+Vy*Vy) );
		Ss /= ( colMax*(Vx*Vx+Vy*Vy) );

		fprintf(dispersion_parallel_file, "%lf %lf\n", temp, Sp);
		fprintf(dispersion_senkrecht_file, "%lf %lf\n", temp, Ss);
		cout << "Temp done: " << temp << endl;
	}

	// Drehimpuls zum letzten Zeitpunkt
	for(int i=0; i<colMax; i++)
	{
		L[i] = (col[i].X - Rx)*(col[i].VY - Vy) - (col[i].Y - Ry)*(col[i].VX - Vx);
		fprintf(momentum_file,"%lf\n",L[i]);
		lua_pushnumber(Lua, L[i]);
	}

	// Calc angular momentum stuff in LUA
	LUACalcAngularMomentum(LUAFILENAME);

	// CLOSE STUFF
	fclose(momentum_file);
	fclose(dispersion_senkrecht_file);
	fclose(dispersion_parallel_file);
	fclose(pos_file); //schliesse files
	time(&tend);
	lua_close(Lua); // LUA schließen
	gsl_rng_free(myRNG); // RNG
};

#include <iostream>
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
#include <string>

#include <lua5.2/lua.hpp>
#include "head.h"
#include <omp.h>

using namespace std;

//main buffer variablen
time_t tstart,tend,tseed;
double PI=M_PI;
double PI2=PI*0.5;
//integration parameter
int t=0;
int Time=100000;	//Anzahl Integrationsschritte
double h=0.001;	//Integrationschrittweite
double sh=sqrt(h);

//Simulationbox
int colMax=300; //Anzahl Teilchen
double density=0.5;
double friction=1.0;
double volume=colMax/density;
double lx=sqrt(volume)/2.0;
double ly=lx;

double temp=0.067;
double D=temp/friction;
double varianz=2.0*D*h;

double sqvarianz;//=sqrt(varianz);
#pragma omp threadprivate(sqvarianz)

double fcc=(lx)/(sqrt((colMax)/2.0));

//RNG
int seed=1;
gsl_rng *myRNG;

//Fileschreiberei
FILE *pos_file;
FILE *momentum_file;
FILE *dispersion_parallel_file;
FILE *dispersion_senkrecht_file;
const char *LUAFILENAME = "angularMomentum.lua";

//LUA instance
lua_State *Lua = luaL_newstate();

//distance
double dx,dy;//Komponenten aus distance
double dist;//Abstandsbetrag aus distance

//OUTPUT toggle
const int printParams = 0;

int main()
{
	#ifdef _OPENMP
		omp_set_num_threads( omp_get_num_procs() );
		cout << "OMP LOADED" << endl;
	#endif

	//Systemzeit ist seed fuer den Zufallsgenerator
	time(&tstart);
	seed=int(tstart*h);
	myRNG = gsl_rng_alloc(gsl_rng_mt19937);
	gsl_rng_set(myRNG,seed);

	//LUA INIT
	luaL_openlibs(Lua);
	if ( luaL_loadfile(Lua, LUAFILENAME) )
		LUAerror("Cannot load config: %s\n", lua_tostring(Lua, -1));
	lua_pushnumber(Lua, colMax); // put number of particles on top of stack

	//open files
	pos_file=fopen("output/position.dat","w");
	momentum_file=fopen("output/momentum.dat","w");
	dispersion_senkrecht_file=fopen("output/dispersion/senkrecht.dat","w");
	dispersion_parallel_file=fopen("output/dispersion/parallel.dat","w");

	// Table for temperatures we want
	const double temp_Start = 0.000;
	const double temp_End = 0.090;
	const int N_temp = 22;
	const double d_temp = (temp_End-temp_Start)/N_temp;
	double temps[N_temp];

	for(int i=0; i<N_temp; i++)
	{
		temps[i] = temp_Start + (i*d_temp);
	}

	//print simulation parameter in bash
	if ( printParams )
	{
		cout << "--------------------------------------------------------------------------" << endl;
		cout << "--------------------------------------------------------------------------" << endl;
		cout << "		density	" << density << endl;
		cout << "		Temperature	" << temp << endl;
		cout << "		Number of Particles	" << colMax << endl;
		cout << "		Number of Integrationsteps	" << Time << endl;
		cout << "		Stepwidth	" << h << endl;
		cout << "		System Size	" << 2.0*lx << endl;
		cout << "		minimum distance (diagonal)	" << sqrt(2.0)*fcc << endl;
		cout << "		friction	" << friction << endl;
		cout << "		seed	" << seed << "	start	" << tstart << endl;
		cout << "--------------------------------------------------------------------------" << endl;
		cout << "--------------------------------------------------------------------------" << endl;
	}

	//Definition von Arrays fuer den Drehimpuls jedes teilchens
	double L[colMax];
	for(int i=0; i<colMax; i++)
	{
		L[i] = 0.0;
	};

	/////////////////////PARALLELIZATION /////////////////////////
	#pragma omp parallel private(temp, D, varianz)
	{
		#ifdef _OPENMP
			#pragma omp single
				cout << "Num Threads: " << omp_get_num_threads() << endl;
		#endif

		#pragma omp for schedule(auto)
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

				//measurements
				//printpos(&col[0],t);
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

			#pragma omp critical(dispersion_parallel_file_dat)
				fprintf(dispersion_parallel_file,"%lf %lf\n",temp,Sp);

			#pragma omp critical(dispersion_senkrecht_file_dat)
				fprintf(dispersion_senkrecht_file,"%lf %lf\n",temp,Ss);

			#pragma omp critical(cout)
				cout << "Temp done: " << temp << endl;
		}
	}

	// Drehimpuls zum letzten Zeitpunkt
	for(int i=0; i<colMax; i++)
	{
	//	L[i] = (col[i].X - Rx)*(col[i].VY - Vy) - (col[i].Y - Ry)*(col[i].VX - Vx);
	//	fprintf(momentum_file,"%lf\n",L[i]);
	//	lua_pushnumber(Lua, L[i]);
	}

	// Calc angular momentum stuff in LUA
	//LUACalcAngularMomentum(LUAFILENAME);

	// CLOSE STUFF
	fclose(momentum_file);
	fclose(dispersion_senkrecht_file);
	fclose(dispersion_parallel_file);
	fclose(pos_file); //schliesse files
	time(&tend);
	lua_close(Lua); // LUA schließen
	gsl_rng_free(myRNG); // RNG

	//print simulation parameter in bash
	if ( printParams )
	{
		cout << "--------------------------------------------------------------------------" << endl;
		cout << "--------------------------------------------------------------------------" << endl;
		cout << "		density	" << density << endl;
		cout << "		Temperature	" << temp << endl;
		cout << "		Number of Particles	" << colMax << endl;
		cout << "		Number of Integrationsteps	" << Time << endl;
		cout << "		Stepwidth	" << h << endl;
		cout << "		System Size	" << lx << endl;
		cout << "		minimum distance (diagonal)	" << sqrt(2.0)*fcc << endl;
		cout << "		friction	" << friction << endl;
		cout << "		duration	" << tend-tstart << endl;
		cout << "--------------------------------------------------------------------------" << endl;
		cout << "--------------------------------------------------------------------------" << endl;
	}

	return(0);
};

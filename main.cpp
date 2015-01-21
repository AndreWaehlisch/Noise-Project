#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <ctime>
#include <sstream>

#include <lua5.2/lua.hpp>
#include "head.h"
#include <omp.h>

using namespace std;

// Globals
double density = 0.5, friction = 1.;

// Variablen (Setup in config.lua)
int Time, colMax, temp_N, Mittelung;
double h, temp_start, temp_d, temp, D, varianz, sqvarianz, rndInit, plotResolution;

// Fileschreiberei
ofstream dispersion_parallel_file, dispersion_senkrecht_file, variance_file, dR_mean_file;
const char *LUAFILENAME = "config.lua";

// LUA instance
lua_State *Lua = luaL_newstate();

// Close files, lua state, RNG, etc.
void CloseStuff()
{
	dispersion_senkrecht_file.close();
	dispersion_parallel_file.close();
	dR_mean_file.close();
	variance_file.close();
	lua_close(Lua); // LUA schließen
}

int main()
{
	// Setup OpenMP
	#ifdef _OPENMP
		omp_set_num_threads( 7 ); //omp_get_num_procs()
		cout << "OMP LOADED" << endl;
	#endif

	// System-Zeit ist seed für den RNG
	time_t tstart;
	time(&tstart);

	//open output files
	dispersion_senkrecht_file.open("output/dispersion/senkrecht.dat", ios::trunc);
	dispersion_parallel_file.open("output/dispersion/parallel.dat", ios::trunc);
	variance_file.open("output/variance.dat", ios::trunc);
	dR_mean_file.open("output/dR_mean.dat", ios::trunc);

	//LUA INIT
	luaL_openlibs(Lua); // open libs in lua
	if ( luaL_loadfile(Lua, LUAFILENAME) ) // try to load the script (and check for syntax errors)
		LUAerror("Cannot load config: %s\n", lua_tostring(Lua, -1));

	LUAExecute(); // execute the script

	// LOAD THE CONFIGS FROM LUA
	lua_getglobal(Lua, "config"); // put table 'config' on stack

	if (!lua_istable(Lua, -1))
		LUAerror("'config' is not a table.\n");

	colMax			=	LUAGetConfigValue("colMax");
	Time			= 	LUAGetConfigValue("Time"); //Anzahl Integrationsschritte
	h				= 	LUAGetConfigValue("h"); //Integrationschrittweite
	Mittelung		= 	LUAGetConfigValue("Mittelung");
	rndInit			= 	LUAGetConfigValue("rndInit");
	temp_N			= 	LUAGetConfigValue("temp_N");
	temp_start		= 	LUAGetConfigValue("temp_start");
	temp_d			= 	LUAGetConfigValue("temp_d");
	plotResolution	=	LUAGetConfigValue("plotResolution");

	// Table for all temperatures we want
	double temps[temp_N];

	for(int i=0; i<temp_N; i++)
	{
		temps[i] = temp_start + (i*temp_d);
	}

	#pragma omp parallel private(temp, D, varianz, sqvarianz)
	{
		//Definition von Arrays fuer den Drehimpuls jedes teilchens
		double L[colMax];

		for(int i=0; i<colMax; i++)
		{
			L[i] = 0.0;
		};

		#pragma omp for schedule(auto)
		for(int T=0; T < temp_N; T++)
		{
			// rng stuff (per temperature!)
			gsl_rng *myRNG;
			const int seed = int(tstart*h);
			myRNG = gsl_rng_alloc(gsl_rng_mt19937);
			gsl_rng_set(myRNG,seed);

			double dx=0, dy=0;
			temp = temps[T];
			D=temp/friction;
			varianz=2.0*D*h;
			sqvarianz=sqrt(varianz);

			ostringstream posFileName;
			posFileName << "output/position_" << temp << ".dat";
			ofstream pos_file(posFileName.str(), ios::trunc);

			ostringstream momentumFileName;
			momentumFileName << "output/momentum_" << temp << ".dat";
			ofstream momentum_file(momentumFileName.str(), ios::trunc);

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

			// Dispersions-Werte, mittlerer Schwerpunktsabstand und Varianz davon
			double Sp = 0, Ss = 0, dR_mean=0, Var=0;

			//Start Simulation
			for(int t=0; t<Time; t++)  		//Schleife der Integrationsschritte
			{
				//calculate forces
				calc_forces(&col[0], fx, fy, dx, dy);

				//apply forces and move particles including stochastic displacement
				integrate(&col[0], fx, fy, sqvarianz, myRNG);

				//write position data to file
				printpos(&col[0], t, pos_file);

				//Dispersion im quasi-Gleichgewicht (wird über die letzten n=Mittelung Werte gemittelt)
				if ( t > (Time-Mittelung) )
					calcDispersionAndAngMomentum(&col[0], Sp, Ss, L, dR_mean, Var);
			};

			// Gemittelte Werte sind bisher nur aufsummiert, hier noch normieren
			Sp /= Mittelung;
			Ss /= Mittelung;
			dR_mean /= Mittelung;
			Var /= Mittelung;

			for(int i=0; i<colMax; i++)
				momentum_file << (L[i] / Mittelung) << endl;

			// put calculated stuff in output files
			#pragma omp critical(dispersion_parallel_file)
				dispersion_parallel_file << temp << '\t' << Sp << endl;
			#pragma omp critical(dispersion_senkrecht_file)
				dispersion_senkrecht_file << temp << '\t' << Ss << endl;
			#pragma omp critical(dR_mean_file)
				dR_mean_file << temp << '\t' << dR_mean << endl;
			#pragma omp critical(variance_file)
				variance_file << temp << '\t' << Var << endl;
			#pragma omp critical(cout)
				cout << "Temp done: " << temp << endl;

			pos_file.close();
			momentum_file.close();
			gsl_rng_free(myRNG); // RNG
		}
	}

	CloseStuff(); // close files, lua state, rng, etc.
};

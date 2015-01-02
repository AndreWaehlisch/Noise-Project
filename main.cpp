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
double temp=0.5; 
double D=temp/friction;
double varianz=2.0*D*h;
double sqvarianz=sqrt(varianz);
int seed=1;
double fcc=(lx)/(sqrt((colMax)/2.0));

//Fileschreiberei
FILE *pos_file;	

//distance
double dx,dy;//Komponenten aus distance
double dist;//Abstandsbetrag aus distance

int main()
{	
	//Systemzeit ist seed fuer den Zufallsgenerator
	time(&tstart);
	seed=int(tstart*h);
	
	//open files
	pos_file=fopen("position.dat","w");
	
	//print simulation parameter in bash
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

	//Definition von Arrays fuer die Krafte auf jedes teilchen
	double fx[colMax];
	double fy[colMax];
	for(i=0; i<colMax; i++)
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
		integrate(&col[0],fx,fy,seed);

		//measurements	
		printpos(&col[0]);	
	};
	
	
	fclose(pos_file); //schliesse files
	time(&tend);  
	
	//print simulation parameter in bash
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

	return(0);
};
























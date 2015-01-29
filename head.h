#ifndef MYHEADER_H
#define MYHEADER_H

#include <fstream>
#include <gsl/gsl_rng.h>

class particle //: public box
{
	public:
	double X;
	double Y;
	double VX;
	double VY;
	void getposition(double &x, double &y);
	void setposition(const double x, const double y);
};

// f_measure.cpp
void printpos(particle a[], const int t, std::ofstream& pos_file);

// f_metrik.cpp
void initial(particle a[]);
void distance(particle &a, particle &b, double &dx, double &dy);

// f_integrate.cp
void integrate(particle a[], double fx[], double fy[], const double sqvarianz, gsl_rng *myRNG);
double gauss();

//f_forces.cpp
void calc_forces(particle a[], double fx[], double fy[], double &dx, double &dy);

//f_lua.cpp
void LUAerror(const char *fmt, ...);
void LUAExecute();
double LUAGetConfigValue(const char *key);

//f_calc.cpp
void calcDispersionAndAngMomentum(particle col[], double& Sp_output, double& Ss_output, double L_output[], double& dR_output, double& Var_output, double& V_mean);

//f_main.cpp
void CloseStuff();

#endif /* MYHEADER_H */

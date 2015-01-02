#ifndef MYHEADER_H
#define MYHEADER_H

class particle //: public box
{
	public:
	double X;
	double Y;
	double VX;
	double VY;
	void getposition(double &x, double &y);
	void setposition(double &x, double &y);
};

// f_measure.cpp
void printpos(particle a[], double L[]);

// f_metrik.cpp
void initial(particle a[]);
void distance(particle &a, particle &b, double &dist, double &dx, double &dy);

// f_integrate.cp
void integrate(particle a[], double fx[], double fy[], double L[]);
double gauss();

//f_forces.cpp
void calc_forces(particle a[],double fx[],double fy[]);

//f_lua.cpp
void LUAerror (const char *fmt, ...);
void LUACalcAngularMomentum (const char *fname);
#endif /* MYHEADER_H */


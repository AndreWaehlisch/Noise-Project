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
void printpos(particle a[]);

// f_metrik.cpp
void initial(particle a[]);
void distance(particle &a, particle &b, double &dist, double &dx, double &dy);

// f_integrate.cp
void integrate(particle a[], double fx[], double fy[], int &seed);
double gauss(const double varianz, int &seed);

//f_forces.cpp
void calc_forces(particle a[],double fx[],double fy[]);
#endif /* MYHEADER_H */


#include <iostream>
#include <cstdio>
#include <fstream>
#include "head.h"

using namespace std;

extern int colMax;
extern double h, plotResolution;

void printpos(particle a[], const int t, ofstream& pos_file)
{
	if ( !(t % int(plotResolution)) )
	{
		for(int i=0; i<colMax; i++)
			pos_file << '\t' << a[i].X << '\t' << a[i].Y;

		pos_file << endl;
	}
}

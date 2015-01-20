myplot.png: main.cpp f_calc.cpp f_lua.cpp f_measure.cpp f_metrik.cpp f_integrate.cpp head.h f_forces.cpp
	g++ -Wall -Wextra -O3 f_calc.cpp f_lua.cpp f_measure.cpp f_metrik.cpp f_integrate.cpp main.cpp f_forces.cpp -lm -lgsl -lgslcblas -llua5.2 -fopenmp -std=c++11

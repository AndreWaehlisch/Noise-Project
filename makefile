myplot.png: main.cpp f_measure.cpp f_metrik.cpp f_integrate.cpp head.h f_forces.cpp
	g++ f_measure.cpp f_metrik.cpp f_integrate.cpp main.cpp f_forces.cpp -lm -lgsl -lgslcblas
	

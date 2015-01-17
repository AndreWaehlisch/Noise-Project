-- config table storing all setup values for C++ program
config = {
	colMax = 100, --Anzahl Teilchen
	Time = 100000, --Anzahl Integrationsschritte
	h = 0.001, --Integrationschrittweite
	Mittelung = 50000, -- Mittelung über letzten X Werte
	initPos = 0; -- initial Pos. der Teilchen

	temp_N = 25,
	temp_start = 0,
	temp_end = 0.9,
	temp_d = (temp_end - temp_start) / (temp_N - 1),

	angularMomentum_Do = 1,
	dispersion_Do = 1,
}

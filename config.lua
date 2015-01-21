local temp_end = 0.9
local temp_start = 0
local temp_N = 21
local temp_d = (temp_end - temp_start) / (temp_N - 1)

-- config table storing all setup values for C++ program
config = {
	colMax = 300, --Anzahl Teilchen
	Time = 250000, --Anzahl Integrationsschritte
	h = 0.001, --Integrationschrittweite
	Mittelung = 50000, -- Mittelung über letzten X Werte
	rndInit = 1, -- initial Pos. der Teilchen (0 => 0, 1=>random 2=> kreis)
	plotResolution = 2500,

	["temp_N"] = temp_N,
	["temp_start"] = temp_start,
	["temp_d"] = temp_d,
}

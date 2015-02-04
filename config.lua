local temp_start = 0.028
local temp_end = 0.07
local temp_N = 2
local temp_d = (temp_N>1) and (temp_end - temp_start) / (temp_N - 1) or 0

-- config table storing all setup values for C++ program
config = {
	colMax = 300, --Anzahl Teilchen
	Time = 250000, --Anzahl Integrationsschritte
	h = 0.001, --Integrationschrittweite
	Mittelung = 2, -- Mittelung über letzten X Werte
	rndInit = 0, -- initial Pos. der Teilchen (0 => 0, 1=>random 2=> kreis)
	plotResolution = 1500,

	["temp_N"] = temp_N,
	["temp_start"] = temp_start,
	["temp_d"] = temp_d,
}

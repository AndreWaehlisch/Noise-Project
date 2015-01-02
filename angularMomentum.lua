local data = { ... };

local colMax = data[1];	-- first return value is always the particle number

local steps = math.fmod(#data-1, colMax);

if ( (steps>0) or (colMax <= 0) ) then
	error("Fehler in den an LUA gelieferte Daten für die AngularMomentum-Berechechnung: Entweder ist Teilchzahl <=0 oder die Anzahl der Daten ist kein Vielfaches der Teilchenzahl.");
	return 1;
else
	steps = (#data-1)/colMax; -- time steps
end;

-- put result data in file momentumLUA.dat (overwrite old data)
local FILE = assert(io.open("./output/momentumLUA.dat","w+"));

local maxVal = 0;

-- find max value
for i = 2, #data do
	if ( math.abs(data[i]) > maxVal ) then
		maxVal = math.abs(data[i]);
	end;
end;

for pointInTime = 0, steps-1 do
	local offset = pointInTime * colMax + 1;

	--local maxVal = 0;

	---- find max value
	--for i = 1, colMax do
		--if ( math.abs(data[offset+i]) > maxVal ) then
			--maxVal = math.abs(data[offset+i]);
		--end;
	--end;

	-- add (normalized) values to result data
	for i = 1, colMax do
		assert(FILE:write(data[offset+i]/maxVal,'\n'));
	end;
end;

FILE:close();

return 0;

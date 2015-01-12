#include <iostream>
#include <cstdlib>
#include <lua5.2/lua.hpp>

using namespace std;

extern int colMax; //Teilchenzahl
extern lua_State *Lua; //Lua state

void LUAerror (const char *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(Lua);
	exit(EXIT_FAILURE);
}

void LUACalcAngularMomentum (const char *fname)
{
	// execute lua file "fname"
	if ( lua_pcall(Lua, lua_gettop(Lua)-1, LUA_MULTRET, 0) ) //LUA_MULTRET=push all return values
		LUAerror("Cannot run config: %s\n", lua_tostring(Lua, -1));
}

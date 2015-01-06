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
	//luaL_dofile(Lua, fname); // loads script and executes it

	// execute lua file "fname"
	if ( lua_pcall(Lua, lua_gettop(Lua)-1, LUA_MULTRET, 0) ) //LUA_MULTRET=push all return values
		LUAerror("Cannot run config: %s\n", lua_tostring(Lua, -1));

	//lua_getglobal(Lua, "testint");
	//int i = lua_tointeger(Lua,-1);
	//cout << "i:" << i << endl;

	//lua_getglobal(L, "width");

	//if (!lua_isnumber(L,-1))
		//LUAerror(L, "'width' should be a number\n");

	//w = lua_tointeger(L,-1);

	//std::cout << "xx=" << w << std::endl;
}

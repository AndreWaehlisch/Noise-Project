#include <iostream>
#include <cstdlib>
#include <lua5.2/lua.hpp>
#include "head.h"

using namespace std;

extern int colMax; //Teilchenzahl
extern lua_State *Lua; //Lua state

void LUAerror (const char *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	CloseStuff();
	exit(EXIT_FAILURE);
}

void LUAExecute ()
{
	// execute lua file (LUA_MULTRET=push all return values)
	if ( lua_pcall(Lua, lua_gettop(Lua)-1, LUA_MULTRET, 0) )
		LUAerror("Cannot run config: %s\n", lua_tostring(Lua, -1));
}

double LUAGetConfigValue(const char *key)
{
	double result;
	int isnum = 0;

	lua_pushstring(Lua, key); // put the key name on the stack
	lua_gettable(Lua, -2); // pop the key name from the stack, and put the corresponding value on the stack (-2 is the location of the 'config' table in the stack)

	result = lua_tonumberx(Lua, -1, &isnum);

	if (!isnum)
		LUAerror("The following key in the 'config' table is not a number: %s\n", key);

	lua_pop(Lua, 1); // pops first element of stack (LIFO, last in first out)

	return result;
}

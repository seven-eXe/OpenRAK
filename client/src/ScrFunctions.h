#pragma once

extern int iScriptsRunning;

struct stScript
{
	char szScriptName[MAX_SCRIPTS][32];
	lua_State *scriptVM[MAX_SCRIPTS];
	lua_State *thread[MAX_SCRIPTS];
	int iSleepTime[MAX_SCRIPTS];
	bool bIsSleep[MAX_SCRIPTS];
};
extern struct stScript script;

void RegisterScriptingFunctions(lua_State *L);
bool LoadScript(const char *szScriptName, bool bFirstLoad);
void CleanUpScripting();
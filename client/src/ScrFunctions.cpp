#include "main.h"

int iScriptsRunning = 0;
struct stScript script;

extern char szWorkingDirectory[MAX_PATH];
char szScripts[512];

bool LoadScript(const char *szScriptName, bool bFirstLoad)
{
	char szTemp[256];
	sprintf_s(szTemp, sizeof(szTemp), "%s\\%s", szWorkingDirectory, szScriptName);

	for (int i = 0; i < MAX_SCRIPTS; i++)
	{
		if (strcmp(script.szScriptName[i], szScriptName) == 0)
		{
			Log("[OPENRAK] FAIL! (%s.lua) already loaded", szScriptName);
			return false;
		}
	}

	FILE *fExists = fopen(szTemp, "rb");
	if (!fExists)
	{
		Log("[OPENRAK] FAIL! (%s.lua) does not exist", szScriptName);
		return false;
	}

	strcpy_s(script.szScriptName[iScriptsRunning], 32, szScriptName);
	script.scriptVM[iScriptsRunning] = lua_open();
	if (script.scriptVM[iScriptsRunning] == NULL)
	{
		Log("[OPENRAK] FAIL! %s.lua LUA virtual machine", szScriptName);
		Log("  ");
		return false;
	}

	//script.thread[iScriptsRunning] = lua_newthread(script.scriptVM[iScriptsRunning]);
	//lua_resume(script.thread[iScriptsRunning], 0);
	luaL_openlibs(script.scriptVM[iScriptsRunning]);

	RegisterScriptingFunctions(script.scriptVM[iScriptsRunning]);

	int iCurrentScriptStatus = luaL_loadfile(script.scriptVM[iScriptsRunning], szTemp);
	if (bFirstLoad) Log("[OPENRAK] %s.lua %s", szScriptName, iCurrentScriptStatus ? "not loaded" : "loaded");
	if (iCurrentScriptStatus == 0)
	{
		//iCurrentScriptStatus = lua_pcall(script.scriptVM[iScriptsRunning], 0, LUA_MULTRET, 0);
		if (!bFirstLoad)
		{
			Log("[OPENRAK] %s loaded.", script.szScriptName[iScriptsRunning]);
		
			//luaEvent_main(script.thread[iScriptsRunning]);
		}

		iScriptsRunning++;
	}
	else
	{
		Log("  %s", lua_tostring(script.scriptVM[iScriptsRunning], -1));
		Log("  ");
		return false;
	}

	return true;
}

void CleanUpScripting()
{
	for (int i = 0; i < iScriptsRunning; i++)
	{
		if (script.scriptVM[iScriptsRunning] != NULL)
			lua_close(script.scriptVM[iScriptsRunning]);
	}

	return;
}
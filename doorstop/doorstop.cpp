#include <iostream>
#include <windows.h>
#include <Psapi.h>
#include <ShlObj.h>

#include <common/ITypes.h>
#include <common/IDebugLog.h>
#include <f4se/PluginAPI.h>
#include <f4se/GameAPI.h>
#include <f4se_common/f4se_version.h>
#include <f4se_common/SafeWrite.h>

#include "config.h"

extern "C"
{
	bool F4SEPlugin_Query(const F4SEInterface *, PluginInfo *);
	bool F4SEPlugin_Load(const F4SEInterface *);
}

IDebugLog               gLog;
void * g_moduleHandle;

PluginHandle            g_pluginHandle = kPluginHandle_Invalid;

char cbAnnounce[1024];

void DllStartup(void *address)
{
	g_moduleHandle = address;
    DWORD procId = GetCurrentProcessId();

	snprintf(cbAnnounce, sizeof(cbAnnounce),
		PLUGIN_NAME_SHORT "[%x %u] at %p query %p load %p\n",
        procId, procId, g_moduleHandle, F4SEPlugin_Query, F4SEPlugin_Load);
}

volatile int stopper = 0;

void LogFileInit()
{
	gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Fallout4\\F4SE\\doorstop.log");

	_MESSAGE(cbAnnounce);
}

bool Query(const F4SEInterface * f4se, PluginInfo * info)
{
	LogFileInit();
	    _MESSAGE("==================== DOORSTOP v%s ====================\n",
        PLUGIN_VERSION_STRING);



	// populate info structure
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = PLUGIN_NAME_SHORT;
	info->version = PLUGIN_VERSION;

	// store plugin handle so we can identify ourselves later
	g_pluginHandle = f4se->GetPluginHandle();

	// Check game version
	if (!COMPATIBLE(f4se->runtimeVersion)) {
		char str[512];
		sprintf_s(str, sizeof(str), "Your game version: v%d.%d.%d.%d\nExpected version: v%d.%d.%d.%d\n%s will be disabled.",
			GET_EXE_VERSION_MAJOR(f4se->runtimeVersion),
			GET_EXE_VERSION_MINOR(f4se->runtimeVersion),
			GET_EXE_VERSION_BUILD(f4se->runtimeVersion),
			GET_EXE_VERSION_SUB(f4se->runtimeVersion),
			GET_EXE_VERSION_MAJOR(SUPPORTED_RUNTIME_VERSION),
			GET_EXE_VERSION_MINOR(SUPPORTED_RUNTIME_VERSION),
			GET_EXE_VERSION_BUILD(SUPPORTED_RUNTIME_VERSION),
			GET_EXE_VERSION_SUB(SUPPORTED_RUNTIME_VERSION),
			PLUGIN_NAME_LONG
		);
	}

	if (f4se->runtimeVersion > SUPPORTED_RUNTIME_VERSION) {
		_MESSAGE("INFO: Newer game version (%08X) than target (%08X).", f4se->runtimeVersion, SUPPORTED_RUNTIME_VERSION);
	}

	return true;
}

bool Load(const F4SEInterface *f4se)
{
	_MESSAGE(PLUGIN_NAME_SHORT " load");

	return true;
}

/* Exported functions */

extern "C" {
	bool F4SEPlugin_Query(const F4SEInterface * f4se, PluginInfo * info)
	{
		return Query(f4se, info);
	}
	
	bool F4SEPlugin_Load(const F4SEInterface *f4se)
	{
		return Load(f4se);
	}

	BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved)
	{
		switch (dwReason)
		{
		case DLL_PROCESS_ATTACH:
			DllStartup((void *)hDllHandle);
			break;

		case DLL_PROCESS_DETACH:
			break;
		};

		return TRUE;
	}
};

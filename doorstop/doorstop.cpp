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

#ifdef MEMPLUGIN
#include <MemoryModule.h>

#define MEMPLUGIN_FILE MEMPLUGIN ".xxx"
#define MEMPLUGIN_PATH "Data\\F4SE\\Plugins\\" MEMPLUGIN_FILE

struct MyMemPlugin {
    LPVOID baseAddr;
    _F4SEPlugin_Query query;
    _F4SEPlugin_Load load;
};

MyMemPlugin g_MemPlugin;
#endif

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
		return false;
	}
	
	if (f4se->runtimeVersion > SUPPORTED_RUNTIME_VERSION) {
		_MESSAGE("INFO: Newer game version (%08X) than target (%08X).", f4se->runtimeVersion, SUPPORTED_RUNTIME_VERSION);
	}

#ifdef MEMPLUGIN
    HMEMORYMODULE hMemModule = NULL;

    INT_PTR loadAddr = (INT_PTR)GetModuleHandle(NULL);
    if (loadAddr)
    {
        std::string loadPath = GetRuntimeDirectory() + MEMPLUGIN_PATH;
        loadAddr += 0x10000000ULL;
        _MESSAGE("Attempting to load %s to %llX\n", loadPath.c_str(), loadAddr);
        hMemModule = MemoryLoadLibraryFile(loadPath.c_str(), (LPVOID)loadAddr);
        _MESSAGE("MemoryLoadLibraryFile returned %p\n", hMemModule);
    }

    if (!hMemModule)
    {
        _MESSAGE("Couldn't load memory module " MEMPLUGIN_FILE "\n");
		return false;
	}

	g_MemPlugin.baseAddr = MemoryGetBaseAddress(hMemModule);
	g_MemPlugin.query = (_F4SEPlugin_Query)MemoryGetProcAddress(hMemModule, "F4SEPlugin_Query");
	g_MemPlugin.load = (_F4SEPlugin_Load)MemoryGetProcAddress(hMemModule, "F4SEPlugin_Load");

	_MESSAGE("g_MemPlugin.handle = %p\n", hMemModule);
	_MESSAGE("g_MemPlugin.base   = %p\n", g_MemPlugin.baseAddr);
	_MESSAGE("g_MemPlugin.query  = %p\n", g_MemPlugin.query);
	_MESSAGE("g_MemPlugin.load   = %p\n", g_MemPlugin.load);

	_MESSAGE("Attempting to call g_MemPlugin.query...\n");

	if (!g_MemPlugin.query(f4se, info))
	{
		return false;
	}
#endif

	return true;
}

bool Load(const F4SEInterface *f4se)
{
	_MESSAGE(PLUGIN_NAME_SHORT " load");

#ifdef MEMPLUGIN
    if (g_MemPlugin.load)
    {
        return g_MemPlugin.load(f4se);
    }
#endif

	return false;
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

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

IDebugLog               gLog;
PluginHandle            g_pluginHandle = kPluginHandle_Invalid;

extern "C"
{
	__declspec(dllexport) bool F4SEPlugin_Query(const F4SEInterface * f4se, PluginInfo * info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Fallout4\\F4SE\\doorstop.log");

		_MESSAGE("%s v%s", PLUGIN_NAME_SHORT, PLUGIN_VERSION_STRING);
		_MESSAGE("%s query", PLUGIN_NAME_SHORT);

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

			MessageBox(NULL, str, PLUGIN_NAME_LONG, MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		if (f4se->runtimeVersion > SUPPORTED_RUNTIME_VERSION) {
			_MESSAGE("INFO: Newer game version (%08X) than target (%08X).", f4se->runtimeVersion, SUPPORTED_RUNTIME_VERSION);
		}

		return true;
	}

	__declspec(dllexport) bool F4SEPlugin_Load(const F4SEInterface *f4se)
	{
		_MESSAGE(PLUGIN_NAME_SHORT " load");

		return true;
	}

};

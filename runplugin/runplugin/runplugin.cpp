// runplugin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <Psapi.h>
#include <common/ITypes.h>
#include <common/IDebugLog.h>
#include <f4se/PluginManager.h>

#define LoadModule(x) (HMODULE)((UInt64)LoadLibraryExA(x, NULL,\
						 LOAD_LIBRARY_AS_IMAGE_RESOURCE) - 2)

const char *hFalloutName = "FakeFallout4.exe";
HMODULE hFallout = 0;
HMODULE hProgram = GetModuleHandle(NULL);

class MyPluginManager : public PluginManager {
protected:
	virtual const char * SafeCallQueryPlugin(LoadedPlugin * plugin, const F4SEInterface * f4se) {
		HMODULE *hModulePtr = (HMODULE *)((UInt64)(plugin->handle) + 0x75d00);
		if (*hModulePtr == hProgram) {
			_MESSAGE("Replacing hProgram with hFallout");
			*hModulePtr = hFallout;
		}
		return PluginManager::SafeCallQueryPlugin(plugin, f4se);
	}
};

MyPluginManager pluginManager;

int main(int argc, char **argv)
{
	if (argc > 1) {
		hFalloutName = argv[1];
	}
	_MESSAGE("Fallout4 program name: %s", hFalloutName);
	hFallout = LoadModule(hFalloutName);

	_MESSAGE("hProgram = %016llX", hProgram);
	_MESSAGE("hFallout = %016llX", hFallout);

	pluginManager.Init();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

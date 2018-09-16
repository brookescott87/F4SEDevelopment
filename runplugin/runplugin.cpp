// runplugin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <common/ITypes.h>
#include <common/IDebugLog.h>
#include <f4se/PluginAPI.h>
#include "ImageLoader.h"
#include "MyPluginManager.h"

INT_PTR qwFalloutBase;
INT_PTR qwProgramBase = (INT_PTR)GetModuleHandle(NULL);
INT_PTR qwModuleFixup = 0;  // Amount added to program references

int main(int argc, char **argv)
{
	qwFalloutBase = (INT_PTR)LoadBinaryImage((LPVOID)0x00007FF7E1350000ULL, 0x6D1F000,
		"Fallout4_00007FF7E1350000_6D1F000.bin");

	if (qwFalloutBase) {
		qwModuleFixup = qwFalloutBase - qwProgramBase;

		_MESSAGE("Program Base = %016llX", qwProgramBase);
		_MESSAGE("Fallout Base = %016llX", qwFalloutBase);
		_MESSAGE("Module Fixup = %016llX", qwModuleFixup);

		myPluginManager.Init(qwProgramBase, qwFalloutBase);
	}
}
// runplugin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <Psapi.h>
#include <common/ITypes.h>
#include <common/IDebugLog.h>
#include <f4se/PluginManager.h>

INT_PTR qwFalloutBase;
INT_PTR qwProgramBase = (INT_PTR)GetModuleHandle(NULL);
INT_PTR qwModuleFixup = 0;  // Amount added to program references

class MyPluginManager : public PluginManager {
protected:
	virtual const char * SafeCallQueryPlugin(LoadedPlugin * plugin, const F4SEInterface * f4se) {
		INT_PTR *qwModulePtr = (INT_PTR *)((INT_PTR)(plugin->handle) + 0x75d00);
		if (*qwModulePtr == qwProgramBase) {
			_MESSAGE("Applying fixup to program references");
			for (int i = 0; i < 6; ++i) {
				INT_PTR qwOldRef = qwModulePtr[i];
				INT_PTR qwNewRef = qwOldRef + qwModuleFixup;
				qwModulePtr[i] = qwNewRef;
				_MESSAGE("  %p: %p -> %p", &qwModulePtr[i], qwOldRef, qwNewRef);
			}
		}
		return PluginManager::SafeCallQueryPlugin(plugin, f4se);
	}
};

MyPluginManager pluginManager;

void *LoadBinaryImage(LPVOID lpBaseAddr, DWORD dwSize, LPCSTR lpszFileName)
{
	DWORD dwBytesRead = 0;
	BOOL bResult = FALSE;
	void *mem = VirtualAlloc(lpBaseAddr, dwSize,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (!mem) {
		_ERROR("Couldn't allocate the address space.  Error = %08X",
			GetLastError());
		return 0;
	}

	HANDLE hFile = CreateFileA(lpszFileName,
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL                   // no attr. template
	);
	
	if (hFile == INVALID_HANDLE_VALUE) {
		_ERROR("Couldn't open file: Error = %08X", GetLastError());
	}
	else {
		BOOL bResult = ReadFile(hFile, mem, dwSize, &dwBytesRead, NULL);

		CloseHandle(hFile);

		if (bResult) {
			if (dwBytesRead == dwSize) {
				return mem;
			}
			else {
				_ERROR("Requested %u bytes but read %u bytes", dwSize, dwBytesRead);
			}
		}
		else {
			_ERROR("Failed to read file: Error = %08X", GetLastError());
		}

		if (!VirtualFree(mem, dwSize, MEM_RELEASE)) {
			_ERROR("Failed to free virtual address space: Error = %08X", GetLastError());
		}
	}

	return NULL;
}

int main(int argc, char **argv)
{
	qwFalloutBase = (INT_PTR)LoadBinaryImage((LPVOID)0x00007FF7E1350000ULL, 0x6D1F000,
		"Fallout4_00007FF7E1350000_6D1F000.bin");

	if (qwFalloutBase) {
		qwModuleFixup = qwFalloutBase - qwProgramBase;

		_MESSAGE("Program Base = %016llX", qwProgramBase);
		_MESSAGE("Fallout Base = %016llX", qwFalloutBase);
		_MESSAGE("Module Fixup = %016llX", qwModuleFixup);

		pluginManager.Init();
	}
}
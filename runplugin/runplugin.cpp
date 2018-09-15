// runplugin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <Psapi.h>
#include <common/ITypes.h>
#include <common/IDebugLog.h>
#include <f4se/PluginManager.h>

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
	hFallout = (HMODULE)LoadBinaryImage((LPVOID)0x00007FF7E1350000ULL, 0x6D1F000,
		"Fallout4_00007FF7E1350000_6D1F000.bin");

	if (hFallout) {
		_MESSAGE("hProgram = %016llX", hProgram);
		_MESSAGE("hFallout = %016llX", hFallout);

		pluginManager.Init();
	}
}
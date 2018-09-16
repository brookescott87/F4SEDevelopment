#include <iostream>
#include <windows.h>
#include <common/ITypes.h>
#include <common/IDebugLog.h>
#include "ImageLoader.h"

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

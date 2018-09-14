#include <Windows.h>
#include <assert.h>

// Adapted from code published here:
// https://gist.github.com/GregLando113/df962f1f80182a0b8cf66de7f95d32b3

#ifndef ASSERT
#define ASSERT assert
#endif

// At least 12 bytes required for this jump
void* Detour64(void* source, void* destination, DWORD64 length) {
	DWORD dwOld;
	//mov rax,0xABABABABABABABAB
	//jmp rax

	ASSERT(length >= 0xC);
	BYTE payload[] = {
		0x48,0xB8, 0xAB,0xAB,0xAB,0xAB,0xAB,0xAB,0xAB,0xAB, // move rax,ABABABABABABABABh
		0xFF, 0xE0                                          // jmp rax
	};

	void* tramp = malloc(length + 12);

	VirtualProtect(tramp, length + 12, PAGE_EXECUTE_READWRITE, &dwOld);
	VirtualProtect(source, length, PAGE_EXECUTE_READWRITE, &dwOld);

	memcpy(tramp, source, length);

	BYTE* tramp_jmp = (BYTE*)tramp + length;
	*(void**)(payload + 2) = (void*)((BYTE*)source + length);
	memcpy(tramp_jmp, payload, 12);

	*(void**)(payload + 2) = destination;
	memcpy(source, payload, 12);
	memset((BYTE*)source + 12, 0x90, length - 12);

	VirtualProtect(source, length, dwOld, &dwOld);

	return tramp;
}


enum NTTHREAD_INFORMATION_CLASS
{
	ThreadBasicInformation,
	ThreadTimes,
	ThreadPriority,
	ThreadBasePriority,
	ThreadAffinityMask,
	ThreadImpersonationToken,
	ThreadDescriptorTableEntry,
	ThreadEnableAlignmentFaultFixup,
	ThreadEventPair,
	ThreadQuerySetWin32StartAddress,
	ThreadZeroTlsCell,
	ThreadPerformanceCount,
	ThreadAmILastThread,
	ThreadIdealProcessor,
	ThreadPriorityBoost,
	ThreadSetTlsArrayAddress,
	ThreadIsIoPending,
	ThreadHideFromDebugger
};

typedef NTSTATUS NTAPI  NtSetInformationThread_t(HANDLE hThread, NTTHREAD_INFORMATION_CLASS cls, LPVOID info, ULONG len);
NtSetInformationThread_t* g_hkNtSetInformationThread_return = nullptr;
NTSTATUS NTAPI hkNtSetInformationThread(HANDLE hThread, NTTHREAD_INFORMATION_CLASS cls, LPVOID info, ULONG len)
{
	// Setting ThreadHideFromDebugger is irreversible so we must hook to make sure it never is set
	// Will cause exceptions to not be catched by the debugger, and for breakpoints to crash if set
	if (cls == ThreadHideFromDebugger)
		// make sure the handle is valid
		// anti debug may send invalid handles to make sure its not patched to always return success
		if (WaitForSingleObject(hThread, 0) != WAIT_FAILED)
		{
			return 0; // NTSTATUS_SUCCESS
		}
	return g_hkNtSetInformationThread_return(hThread, cls, info, len);
}


void AntiDebugHook()
{
	NtSetInformationThread_t* src = (NtSetInformationThread_t*)GetProcAddress(GetModuleHandleA("ntdll.dll"), "ZwSetInformationThread");
	if (src)
	{
		g_hkNtSetInformationThread_return = (NtSetInformationThread_t*)Detour64(src, hkNtSetInformationThread, 0x20);
	}
}
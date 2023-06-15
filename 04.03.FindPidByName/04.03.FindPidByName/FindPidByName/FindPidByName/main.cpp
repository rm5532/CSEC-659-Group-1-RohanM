#define no_init_all
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#pragma comment (lib, "kernel32.lib") // process enum/ CreateToolhelp32Snapshot

// returns the pid for the process 
// returns -1 if not found
// returns -2 for invalid_handle_value

// References:
// PROCESSENTRY32W
// https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/ns-tlhelp32-processentry32w
//
// CreateToolhelp32Snapshot 
// ref: https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/nf-tlhelp32-createtoolhelp32snapshot
// Process32NextW
// ref: https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/nf-tlhelp32-process32nextw
int findPidByName(const wchar_t* name)
{
	HANDLE h = NULL;
	PROCESSENTRY32W procSnapshot = { 0 };
	h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);// TODO make a snapshot
	if (h == INVALID_HANDLE_VALUE)
	{
		return -2;
	}
	procSnapshot.dwSize = sizeof(PROCESSENTRY32W);
	int pid = -1;

	do
	{       
                // TODO compare against name
		
		if (_wcsicmp(procSnapshot.szExeFile, name) == 0)
		{
			// TODO
                        // (int)cast as well.
			pid = static_cast<int>(procSnapshot.th32ProcessID);// TODO;
#ifdef _DEBUG
			wprintf(L"[+] PID found: %ld\n", pid);
#endif
			break;
		}
	} while (Process32NextW(h, &procSnapshot));

	CloseHandle(h);
	return pid;
}

int main()
{
	wprintf(L"lookup: notepad.exe return: %d\n", findPidByName(L"notepad.exe"));
	wprintf(L"lookup: mspaint.exe return: %d\n", findPidByName(L"mspaint.exe"));
	
	wprintf(L"lookup: InvalidPid.exe return: %d\n", findPidByName(L"InvalidPid.exe"));
	
	return 0;
}